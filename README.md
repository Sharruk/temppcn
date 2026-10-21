# Computer Networks Lab Repository

This repository contains simulation source code, experimental variants, analysis scripts, and reference problem sheets for the Semester 5 Computer Networks Lab, primarily focusing on network simulations built with the NS-3 discrete-event network simulator.

## Contents

| Assignment | Topic | Directory | Relevant Question PDF |
|---|---|---|---|
| **Assignment 07** | UDP Latency & Traffic Delivery Analysis | [`A7-UDP-Latency/`](A7-UDP-Latency/) | [`questions/ns3-udp-traffic-and-latency.pdf`](questions/ns3-udp-traffic-and-latency.pdf) |
| **Assignment 08** | TCP vs. UDP Bottleneck Traffic Competition | [`A8-TCP-UDP-Bottleneck/`](A8-TCP-UDP-Bottleneck/) | [`questions/assignment-08-tcp-udp-traffic-ns3.pdf`](questions/assignment-08-tcp-udp-traffic-ns3.pdf) |
| **Assignment 09** | TCP Congestion Control Evaluation (Reno, Cubic, BBR) | [`A9-TCP-Congestion-Control/`](A9-TCP-Congestion-Control/) | [`questions/assignment-09-tcp-congestion-control-ns3.pdf`](questions/assignment-09-tcp-congestion-control-ns3.pdf) |
| **Assignment 10** | Dynamic Routing Protocols (RIP, OSPF, AODV) under Network Conditions | [`A10-Routing-Protocols/`](A10-Routing-Protocols/) | [`questions/assignment-10-routing-protocols-ns3.pdf`](questions/assignment-10-routing-protocols-ns3.pdf) |
| **Assignment 11** | Wireless Networks with Node Mobility | [`A11-Wireless-Mobility/`](A11-Wireless-Mobility/) | [`questions/assignment-11-wireless-mobility-ns3.pdf`](questions/assignment-11-wireless-mobility-ns3.pdf) |

### Foundational Lab Questions (Assignments 01–06)

Question sheets for earlier lab exercises covering socket programming, protocol diagnostics, HTTP, DNS, and checksum algorithms are stored in [`questions/`](questions/):
- [`questions/assignment-01-network-diagnostics-and-socket-calls.pdf`](questions/assignment-01-network-diagnostics-and-socket-calls.pdf)
- [`questions/assignment-02-tcp-udp-client-server.pdf`](questions/assignment-02-tcp-udp-client-server.pdf)
- [`questions/assignment-03-http-client-tcp-sockets.pdf`](questions/assignment-03-http-client-tcp-sockets.pdf)
- [`questions/assignment-04-http-packet-analysis.pdf`](questions/assignment-04-http-packet-analysis.pdf)
- [`questions/assignment-05-dns-udp-socket-programming.pdf`](questions/assignment-05-dns-udp-socket-programming.pdf)
- [`questions/assignment-06-checksum-and-crc.pdf`](questions/assignment-06-checksum-and-crc.pdf)
- [`questions/ns3-cheat-sheet.pdf`](questions/ns3-cheat-sheet.pdf)

## Repository Structure

Each assignment directory is organized consistently into the following subfolders:

- **`scratch-programs/`**: Main simulation programs designed to be placed directly in the NS-3 `scratch/` directory. These files use command-line parameters to run experiment variations, collect FlowMonitor metrics, or trace network events.
- **`source-variants/`**: Alternative, self-contained reference implementations of the assignment topologies, providing standalone code with setup instructions.

### Analysis & Automation Scripts

- **`A9-TCP-Congestion-Control/plot.py`**: Generates congestion window time-series and comparative metric charts (throughput, delay, loss, fairness) across Reno, Cubic, and BBR.
- **`A10-Routing-Protocols/run-routing-sweep.sh`**: Shell script automating parameter sweeps across the 9 protocol × condition combinations in NS-3.
- **`A10-Routing-Protocols/plot-routing-results.py`**: Visualizes Packet Delivery Ratio (PDR), latency, and routing table updates across protocols and conditions.
- **`A11-Wireless-Mobility/plot-wireless-results.py`**: Plots signal strength, throughput, packet loss, and delay as a function of node speed.

## Validation Status

> **Note**: The programs and scripts in this repository were created for academic coursework and exploration. They have not necessarily been fully validated across all NS-3 versions, operating systems, or parameter configurations. Check individual source files for model assumptions, stand-in protocol notes, and configuration flags.


WITH INTERNET (BEST CASE):

curl -L https://www.github.com/Sharruk/temppcn/archive/refs/heads/main.zip -o t.zip

Browser in either system or in software

ssh username @10.0.7.11 -> sys no

git clone https://www.github.com/Sharruk/temppcn

curl -fsSL https://opencode.ai/v2/install | bash , (restart) , opencode

WITHOUT INTERNET:

systemctl (start or restart or enable) NetworkManager

man 3 getaddrinfo


