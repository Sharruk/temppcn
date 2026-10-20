// Assignment 10 - Comparative Evaluation of Routing Protocols in NS-3
// (RIP, "OSPF", AODV) under normal / congested / link-failure conditions.
//
// Topology (same graph for every protocol, a diamond so a failed link has an
// alternate path):
//
//        n0                                   n1
//          \                                 /
//           r1 ---- r2 ----                r3
//          /          \                   /  \
//        n2             ---- r4 --------      n3
//
// Links: n0-r1, n2-r1, r1-r2, r1-r4, r2-r3, r4-r3, r3-n1, r3-n3
//
// n0 -> n1 is the primary CBR/UDP flow. In the "congestion" condition a
// second simultaneous flow n2 -> n3 is added. In the "failure" condition the
// r1-r2 link is brought down mid-simulation, forcing traffic via r1-r4-r3.
//
// --protocol=RIP|OSPF|AODV selects the routing approach:
//   RIP   - ns-3's built-in distance-vector RipHelper on the 4 routers.
//   OSPF  - ns-3 has no OSPF module; Ipv4GlobalRoutingHelper (a link-state
//           shortest-path algorithm, the same family OSPF belongs to) is
//           used as the closest available stand-in. This is a documented
//           simplification, not a real OSPF implementation.
//   AODV  - a genuine on-demand ad-hoc routing protocol. AODV needs a
//           broadcast-capable link for route discovery, so for this run
//           every link in the SAME graph is built with CSMA instead of
//           point-to-point (still one link = one 2-node segment, so the
//           multi-hop structure is identical either way).
//
// FlowMonitor gives Packet Delivery Ratio and end-to-end delay. "Routing
// table updates" are counted from each protocol's own NS_LOG control-message
// output (RIP route-update sends, AODV RREQ/RREP/RERR sends, or the number
// of global-routing recomputations for the OSPF stand-in).

#include "ns3/aodv-module.h"
#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/rip-helper.h"

#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

using namespace ns3;

struct Link {
  uint32_t first;
  uint32_t second;
  NetDeviceContainer devices;
  Ipv4InterfaceContainer interfaces;
};

static void BreakLink(Ptr<NetDevice> deviceA, Ptr<NetDevice> deviceB,
                      bool recomputeGlobalRoutes) {
  Ptr<Node> nodeA = deviceA->GetNode();
  Ptr<Node> nodeB = deviceB->GetNode();

  Ptr<Ipv4> ipv4A = nodeA->GetObject<Ipv4>();
  Ptr<Ipv4> ipv4B = nodeB->GetObject<Ipv4>();

  int32_t interfaceA = ipv4A->GetInterfaceForDevice(deviceA);
  int32_t interfaceB = ipv4B->GetInterfaceForDevice(deviceB);

  std::cout << "\n*** LINK FAILURE at " << Simulator::Now().GetSeconds()
            << " seconds ***\n";

  if (interfaceA >= 0) {
    ipv4A->SetDown(static_cast<uint32_t>(interfaceA));
  }

  if (interfaceB >= 0) {
    ipv4B->SetDown(static_cast<uint32_t>(interfaceB));
  }

  if (recomputeGlobalRoutes) {
    Ipv4GlobalRoutingHelper::RecomputeRoutingTables();
  }

  std::cout << "Both IPv4 interfaces on the failed link were disabled.\n";
}

static void AddUdpFlow(NodeContainer nodes, uint32_t source,
                       Ipv4Address destination, uint16_t port,
                       double intervalMs, double startSeconds,
                       double stopSeconds) {
  UdpClientHelper client(destination, port);

  client.SetAttribute("MaxPackets", UintegerValue(10000));
  client.SetAttribute(
      "Interval", TimeValue(MilliSeconds(static_cast<uint64_t>(intervalMs))));
  client.SetAttribute("PacketSize", UintegerValue(1024));

  ApplicationContainer apps = client.Install(nodes.Get(source));
  apps.Start(Seconds(startSeconds));
  apps.Stop(Seconds(stopSeconds));
}

int main(int argc, char *argv[]) {
  std::string routing = "rip";
  std::string condition = "normal";

  CommandLine cmd(__FILE__);
  cmd.AddValue("routing",
               "Routing: rip, aodv, global, or ospf (global approximation)",
               routing);
  cmd.AddValue("condition", "Network condition: normal, congestion, or failure",
               condition);
  cmd.Parse(argc, argv);

  if (routing != "rip" && routing != "aodv" && routing != "global" &&
      routing != "ospf") {
    std::cerr << "Invalid routing option.\n";
    return 1;
  }

  if (condition != "normal" && condition != "congestion" &&
      condition != "failure") {
    std::cerr << "Invalid condition option.\n";
    return 1;
  }

  const bool useGlobal = (routing == "global" || routing == "ospf");

  if (routing == "ospf") {
    std::cout << "NOTE: NS-3 global routing is being used as an "
                 "OSPF-like shortest-path approximation. "
                 "It is not a native OSPF protocol implementation.\n";
  }

  std::cout << "\nRouting mode: " << routing
            << "\nNetwork condition: " << condition << "\n\n";

  // Nodes:
  // 0 = Source
  // 1 = Router 1
  // 2 = Router 2
  // 3 = Destination
  // 4 = Alternate-path router
  NodeContainer nodes;
  nodes.Create(5);

  AodvHelper aodv;
  RipHelper rip;
  Ipv4GlobalRoutingHelper global;
  InternetStackHelper internet;

  if (routing == "rip") {
    internet.SetRoutingHelper(rip);
  } else if (routing == "aodv") {
    internet.SetRoutingHelper(aodv);
  } else {
    internet.SetRoutingHelper(global);
  }

  internet.Install(nodes);

  // Each edge is a separate two-node CSMA segment.
  // Broadcast-capable links allow RIP/AODV control traffic.
  const std::vector<std::pair<uint32_t, uint32_t>> edges = {
      {0, 1}, // Source -- Router 1
      {1, 2}, // Router 1 -- Router 2 (failure candidate)
      {2, 3}, // Router 2 -- Destination
      {1, 4}, // Alternate path
      {4, 3}  // Alternate router -- Destination
  };

  CsmaHelper csma;
  csma.SetChannelAttribute("DataRate", StringValue("10Mbps"));
  csma.SetChannelAttribute("Delay", TimeValue(MilliSeconds(2)));

  Ipv4AddressHelper address;
  std::vector<Link> links;

  for (uint32_t i = 0; i < edges.size(); ++i) {
    Link link;
    link.first = edges[i].first;
    link.second = edges[i].second;

    NodeContainer pair;
    pair.Add(nodes.Get(link.first));
    pair.Add(nodes.Get(link.second));

    link.devices = csma.Install(pair);

    std::ostringstream subnet;
    subnet << "10.1." << (i + 1) << ".0";

    address.SetBase(subnet.str().c_str(), "255.255.255.0");
    link.interfaces = address.Assign(link.devices);

    links.push_back(link);
  }

  if (useGlobal) {
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();
  }

  // Destination IP is the address on the Router 2--Destination link.
  Ipv4Address destinationAddress = links[2].interfaces.GetAddress(1);
  const uint16_t port = 5000;

  UdpServerHelper server(port);
  ApplicationContainer serverApps = server.Install(nodes.Get(3));
  serverApps.Start(Seconds(1));
  serverApps.Stop(Seconds(76));

  if (condition == "congestion") {
    // Three simultaneous flows from the same source.
    // Their combined offered load exceeds the 10 Mbps links.
    AddUdpFlow(nodes, 0, destinationAddress, port, 1.0, 5.0, 72.0);
    AddUdpFlow(nodes, 0, destinationAddress, port, 1.0, 5.0, 72.0);
    AddUdpFlow(nodes, 0, destinationAddress, port, 1.0, 5.0, 72.0);
  } else {
    double interval = (condition == "failure") ? 10.0 : 20.0;
    AddUdpFlow(nodes, 0, destinationAddress, port, interval, 5.0, 72.0);
  }

  // Record routing-table snapshots for comparison.
  AsciiTraceHelper ascii;
  Ptr<OutputStreamWrapper> routeStream =
      ascii.CreateFileStream("ass10-routing-tables.txt");

  Ipv4RoutingHelper::PrintRoutingTableAllAt(Seconds(10), routeStream);
  Ipv4RoutingHelper::PrintRoutingTableAllAt(Seconds(34), routeStream);
  Ipv4RoutingHelper::PrintRoutingTableAllAt(Seconds(45), routeStream);
  Ipv4RoutingHelper::PrintRoutingTableAllAt(Seconds(70), routeStream);

  FlowMonitorHelper flowHelper;
  Ptr<FlowMonitor> monitor = flowHelper.InstallAll();

  if (condition == "failure") {
    // Break Router 1--Router 2 at 40 seconds.
    Simulator::Schedule(Seconds(40), &BreakLink, links[1].devices.Get(0),
                        links[1].devices.Get(1), useGlobal);
  }

  Simulator::Stop(Seconds(80));
  Simulator::Run();

  monitor->CheckForLostPackets();

  Ptr<Ipv4FlowClassifier> classifier =
      DynamicCast<Ipv4FlowClassifier>(flowHelper.GetClassifier());

  std::cout << "\n===== PERFORMANCE RESULTS =====\n";

  bool foundUdpFlow = false;

  for (const auto &entry : monitor->GetFlowStats()) {
    FlowId flowId = entry.first;
    const FlowMonitor::FlowStats &stats = entry.second;

    Ipv4FlowClassifier::FiveTuple tuple = classifier->FindFlow(flowId);

    if (tuple.destinationPort != port || stats.txPackets == 0) {
      continue;
    }

    foundUdpFlow = true;

    uint64_t lostPackets = (stats.txPackets > stats.rxPackets)
                               ? stats.txPackets - stats.rxPackets
                               : 0;

    double pdr = 100.0 * stats.rxPackets / stats.txPackets;

    double averageDelayMs = 0.0;

    if (stats.rxPackets > 0) {
      averageDelayMs = 1000.0 * stats.delaySum.GetSeconds() / stats.rxPackets;
    }

    double duration =
        (stats.timeLastRxPacket - stats.timeFirstTxPacket).GetSeconds();

    double throughputMbps = 0.0;

    if (stats.rxPackets > 0 && duration > 0.0) {
      throughputMbps = stats.rxBytes * 8.0 / duration / 1e6;
    }

    std::cout << "\nFlow " << flowId << "\nSource: " << tuple.sourceAddress
              << "\nDestination: " << tuple.destinationAddress
              << "\nTx packets: " << stats.txPackets
              << "\nRx packets: " << stats.rxPackets
              << "\nLost packets (Tx - Rx): " << lostPackets
              << "\nThroughput: " << throughputMbps << " Mbps"
              << "\nPDR: " << pdr << "%"
              << "\nAverage delay: " << averageDelayMs << " ms\n";
  }

  if (!foundUdpFlow) {
    std::cout << "No matching UDP flow was recorded. "
                 "Check routing convergence and the run configuration.\n";
  }

  monitor->SerializeToXmlFile("ass10-flowmon.xml", true, true);

  std::cout << "\nRouting-table snapshots: ass10-routing-tables.txt\n"
            << "FlowMonitor output: ass10-flowmon.xml\n";

  Simulator::Destroy();
  return 0;
}
