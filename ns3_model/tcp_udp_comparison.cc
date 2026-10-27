#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("TcpUdpCompare");

int main(int argc, char *argv[])
{
    std::string transportProt = "TCP"; // Default is TCP, can pass --transportProt=UDP
    double simTime = 30.0;             // 30 seconds simulation duration
    std::string dataRate = "2Mbps";    // 2 Mbps application rate
    uint32_t packetSize = 1024;        // 1024 bytes packet size

    CommandLine cmd(__FILE__);
    cmd.AddValue("transportProt", "Transport protocol to use: TCP or UDP", transportProt);
    cmd.AddValue("simTime", "Simulation time in seconds", simTime);
    cmd.Parse(argc, argv);

    // Default queue configuration: DropTail, max queue size = 50 packets
    Config::SetDefault("ns3::DropTailQueue<Packet>::MaxSize", StringValue("50p"));

    // 1. Create 6 Nodes (0 to 5)
    NodeContainer nodes;
    nodes.Create(6);

    // 2. Point-to-Point Links Configuration
    // Link bandwidth = 5Mbps, Link delay = 5ms
    PointToPointHelper p2p;
    p2p.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p.SetChannelAttribute("Delay", StringValue("5ms"));
    p2p.SetQueue("ns3::DropTailQueue<Packet>", "MaxSize", StringValue("50p"));

    // Links:
    // Main path: n0 -> n2 -> n3 -> n5
    // Branching links: n1 -> n2, n4 -> n2
    NetDeviceContainer d0d2 = p2p.Install(nodes.Get(0), nodes.Get(2));
    NetDeviceContainer d1d2 = p2p.Install(nodes.Get(1), nodes.Get(2));
    NetDeviceContainer d4d2 = p2p.Install(nodes.Get(4), nodes.Get(2));
    NetDeviceContainer d2d3 = p2p.Install(nodes.Get(2), nodes.Get(3));
    NetDeviceContainer d3d5 = p2p.Install(nodes.Get(3), nodes.Get(5));

    // 3. Install Internet Stack
    InternetStackHelper stack;
    stack.Install(nodes);

    // 4. Assign IP Addresses to Each Subnet
    Ipv4AddressHelper address;

    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer i0i2 = address.Assign(d0d2);

    address.SetBase("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer i1i2 = address.Assign(d1d2);

    address.SetBase("10.1.3.0", "255.255.255.0");
    Ipv4InterfaceContainer i4i2 = address.Assign(d4d2);

    address.SetBase("10.1.4.0", "255.255.255.0");
    Ipv4InterfaceContainer i2i3 = address.Assign(d2d3);

    address.SetBase("10.1.5.0", "255.255.255.0");
    Ipv4InterfaceContainer i3i5 = address.Assign(d3d5);

    // Populate Global Routing Tables
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    // 5. Install Applications (Source: Node 0, Destination: Node 5)
    uint16_t port = 8080;
    Ipv4Address destAddress = i3i5.GetAddress(1); // Node 5's IP address on link d3d5

    if (transportProt == "TCP")
    {
        // Destination Sink (Node 5)
        PacketSinkHelper sink("ns3::TcpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), port));
        ApplicationContainer sinkApp = sink.Install(nodes.Get(5));
        sinkApp.Start(Seconds(1.0));
        sinkApp.Stop(Seconds(simTime));

        // Source OnOffApplication (Node 0)
        OnOffHelper client("ns3::TcpSocketFactory", InetSocketAddress(destAddress, port));
        client.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
        client.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
        client.SetAttribute("DataRate", DataRateValue(DataRate(dataRate)));
        client.SetAttribute("PacketSize", UintegerValue(packetSize));

        ApplicationContainer clientApp = client.Install(nodes.Get(0));
        clientApp.Start(Seconds(2.0));
        clientApp.Stop(Seconds(simTime));
    }
    else // UDP
    {
        // Destination Sink (Node 5)
        PacketSinkHelper sink("ns3::UdpSocketFactory", InetSocketAddress(Ipv4Address::GetAny(), port));
        ApplicationContainer sinkApp = sink.Install(nodes.Get(5));
        sinkApp.Start(Seconds(1.0));
        sinkApp.Stop(Seconds(simTime));

        // Source OnOffApplication (Node 0)
        OnOffHelper client("ns3::UdpSocketFactory", InetSocketAddress(destAddress, port));
        client.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
        client.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
        client.SetAttribute("DataRate", DataRateValue(DataRate(dataRate)));
        client.SetAttribute("PacketSize", UintegerValue(packetSize));

        ApplicationContainer clientApp = client.Install(nodes.Get(0));
        clientApp.Start(Seconds(2.0));
        clientApp.Stop(Seconds(simTime));
    }

    // 6. Install FlowMonitor to Measure Performance Metrics
    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor = flowmon.InstallAll();

    // 7. Run Simulation
    Simulator::Stop(Seconds(simTime));
    Simulator::Run();

    monitor->CheckForLostPackets();
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());
    std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats();

    uint64_t txPackets = 0;
    uint64_t rxPackets = 0;
    uint64_t lostPackets = 0;
    uint64_t rxBytes = 0;
    double totalDelayMs = 0.0;
    double throughputKbps = 0.0;

    for (auto i = stats.begin(); i != stats.end(); ++i)
    {
        Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(i->first);
        // Look for the flow from Node 0 to Node 5
        if (t.sourceAddress == i0i2.GetAddress(0) && t.destinationAddress == destAddress)
        {
            txPackets += i->second.txPackets;
            rxPackets += i->second.rxPackets;
            lostPackets += i->second.lostPackets;
            rxBytes += i->second.rxBytes;

            if (rxPackets > 0)
            {
                totalDelayMs = (i->second.delaySum.GetSeconds() / rxPackets) * 1000.0;
            }

            double activeTime = (simTime - 2.0); // Active transmission between 2s and 30s
            if (activeTime > 0)
            {
                throughputKbps = (rxBytes * 8.0) / (activeTime * 1000.0);
            }
        }
    }

    double pdr = 0.0;
    if (txPackets > 0)
    {
        pdr = ((double)rxPackets / (double)txPackets) * 100.0;
    }

    std::cout << "\n==========================================" << std::endl;
    std::cout << "  NS-3 Performance Results: " << transportProt << std::endl;
    std::cout << "==========================================" << std::endl;
    std::cout << "Protocol: " << transportProt << std::endl;
    std::cout << "Tx Packets: " << txPackets << std::endl;
    std::cout << "Rx Packets: " << rxPackets << std::endl;
    std::cout << "Lost Packets: " << lostPackets << std::endl;
    std::cout << "Throughput: " << throughputKbps << " Kbps (" << (throughputKbps / 1000.0) << " Mbps)" << std::endl;
    std::cout << "Packet Delivery Ratio (PDR): " << pdr << " %" << std::endl;
    std::cout << "Average End-to-End Delay: " << totalDelayMs << " ms" << std::endl;
    std::cout << "==========================================\n" << std::endl;

    Simulator::Destroy();
    return 0;
}
