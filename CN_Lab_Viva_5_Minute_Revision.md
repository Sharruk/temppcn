# Computer Networks Lab Viva — 5-Minute Rapid Revision Notes

**Course:** UCS3511 / Computer Networks Laboratory  
**Focus:** High-Yield Rapid Viva Revision for Practical End-Semester Examination  
**Scope:** Assignments 1, 2, 3, 5, 6, 7, 8, 9, 11 & NS-3 Quick Reference (Excludes Assignment 10 & ARP)

---

## Quick Navigation Index

- [Section A — NS-3 Fundamentals](#section-a--ns-3-fundamentals)
- [Section B — My NS-3 Assignments (7, 8, 9, 11)](#section-b--my-ns-3-assignments)
- [Section C — Socket Programming Fundamentals](#section-c--socket-programming-fundamentals)
- [Section D — TCP, UDP, Timers and Retransmissions](#section-d--tcp-udp-timers-and-retransmissions)
- [Section E — Error Detection: Checksum and CRC](#section-e--error-detection-checksum-and-crc)
- [Section F — Application Layer: DNS and HTTP File Download](#section-f--application-layer-dns-and-http-file-download)
- [Section G — Performance Metrics and Formulas](#section-g--performance-metrics-and-formulas)
- [Section H — Top 35 High-Yield Viva Questions and Short Answers](#section-h--top-35-high-yield-viva-questions-and-short-answers)
- [Section I — Final 5-Minute Memory Sheet](#section-i--final-5-minute-memory-sheet)

---

## Section A — NS-3 Fundamentals

### 1. What is NS-3 and What Happens in a Simulation?
- **NS-3 (Network Simulator 3)** is an open-source, discrete-event network simulator written in C++ with optional Python bindings.
- It models computer networks, packet exchanges, routing, and transport protocols entirely in software.
- **In a simulation**, virtual network nodes execute real protocol logic (IP, TCP, UDP, Wi-Fi, Ethernet). Packets are created as software byte buffers (`ns3::Packet`) and scheduled as discrete events on a virtual timeline. No physical cables, routers, or network hardware are used.

### 2. Core Concepts and Terminology
- **Node (`NodeContainer`):** Represents a computing device (host, server, router, mobile phone, or access point) in the virtual topology.
- **Channel / Link:** The transmission medium connecting nodes. Wired links use `PointToPointChannel`; wireless links use `YansWifiChannel`.
- **NetDevice (`NetDeviceContainer`):** The virtual Network Interface Card (NIC) installed on a node that connects it to a channel (e.g., `PointToPointNetDevice`, `WifiNetDevice`).
- **Packet:** A discrete unit of data transmitted across the network containing simulated protocol headers, payloads, and timestamps.
- **Application (`ApplicationContainer`):** User-level software running on a node that generates or consumes traffic (e.g., `UdpEchoClient`, `UdpEchoServer`, `BulkSendApplication`, `OnOffApplication`, `PacketSink`).
- **Point-to-Point Wired Network:** A private, dedicated channel connecting exactly two nodes with fixed bandwidth and fixed propagation delay.
- **Wireless Wi-Fi Network:** A shared broadcast radio medium where multiple stations communicate with an Access Point (or ad-hoc peers) via radio frequency signals using CSMA/CA contention, subject to path loss, fading, distance, and interference.
- **Wired vs. Wireless Differences:**
  - *Wired:* Dedicated capacity, deterministic delay, zero interference, fixed topology.
  - *Wireless:* Shared broadcast medium, collisions, distance-dependent signal attenuation, dynamic channel fading, node mobility.
- **Why Configure Network Parameters?**
  - *Bandwidth (`DataRate`):* Defines channel capacity in bits per second (e.g., `5Mbps`, `10Mbps`).
  - *Link Delay (`Delay`):* Simulates physical propagation time taken for signals to traverse the link (e.g., `2ms`, `20ms`).
  - *IP Addresses:* Identifies layer-3 endpoints for packet routing and forwarding across subnets.
  - *Traffic:* Models real-world data patterns (greedy bulk downloads, bursts, constant bit rate voice/video).
- **Simulation Time:** A discrete virtual clock managed by the simulation engine (`Simulator::Now()`). It advances in discrete jumps from one event timestamp to the next, completely independent of real-world wall-clock time.
- **Helper Classes:** High-level C++ abstraction wrappers that automate the creation, configuration, and aggregation of complex low-level simulation objects across multiple nodes with single-line commands.

### 3. Canonical 8-Step NS-3 Execution Workflow
Every NS-3 simulation program follows this exact execution sequence:
1. **Create Nodes:** Instantiate node containers (`NodeContainer nodes; nodes.Create(n);`).
2. **Configure Channel & Devices:** Set channel attributes (`DataRate`, `Delay`) and install NetDevices on nodes using `PointToPointHelper` or `WifiHelper`.
3. **Install Internet Stack:** Aggregate IPv4, TCP, and UDP protocol stacks onto nodes using `InternetStackHelper`.
4. **Assign IP Addresses:** Configure network subnets and assign IP addresses to NetDevices using `Ipv4AddressHelper`.
5. **Configure Applications & Traffic:** Install client/server applications (e.g., `PacketSinkHelper`, `BulkSendHelper`, `OnOffHelper`, `UdpClientHelper`, `UdpServerHelper`).
6. **Schedule Start and Stop Times:** Set execution intervals (`app.Start(Seconds(1.0)); app.Stop(Seconds(10.0));`).
7. **Run Simulator:** Schedule automatic termination (`Simulator::Stop(Seconds(10.0));`) and trigger the event engine with `Simulator::Run();`.
8. **Collect Metrics & Clean Up:** Extract flow statistics using `FlowMonitorHelper`, then invoke `Simulator::Destroy();` to reclaim resources.

### 4. Essential NS-3 Classes at a Glance

| Class Name | Primary Purpose in Simulation | Typical Code Pattern |
|---|---|---|
| `NodeContainer` | Holds and manages a collection of virtual network nodes | `NodeContainer nodes; nodes.Create(2);` |
| `PointToPointHelper` | Configures wired link attributes and creates point-to-point devices/channels | `p2p.SetDeviceAttribute("DataRate", StringValue("5Mbps"));` |
| `WifiHelper` | Configures 802.11 Wi-Fi standards and station managers | `wifi.SetStandard(WIFI_STANDARD_80211n);` |
| `InternetStackHelper` | Installs IPv4, ARP, TCP, and UDP layer-3/layer-4 stacks on nodes | `InternetStackHelper stack; stack.Install(nodes);` |
| `Ipv4AddressHelper` | Manages IP subnet allocations and assigns IPv4 addresses to NetDevices | `address.SetBase("10.1.1.0", "255.255.255.0");` |
| `UdpEchoServerHelper` | Sets up a UDP echo server listening on a specified port | `UdpEchoServerHelper server(9);` |
| `UdpEchoClientHelper` | Configures a UDP client to send packets and verify echoed responses | `UdpEchoClientHelper client(serverIp, 9);` |
| `MobilityHelper` | Assigns spatial coordinates and mobility models to nodes | `mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");` |
| `FlowMonitorHelper` | Probes network traffic to collect throughput, loss, delay, and packet counts | `Ptr<FlowMonitor> mon = flowmon.InstallAll();` |
| `Simulator::Run()` | Starts the discrete-event processing loop | `Simulator::Run();` |
| `Simulator::Destroy()` | Destroys the simulation engine and frees allocated heap memory | `Simulator::Destroy();` |

### 5. UDP Echo in Simple Terms
- **Client** generates a UDP packet containing a payload and sends it across the network to the server on port 9.
- **Server** listens on port 9, receives the packet, and immediately bounces (echoes) the exact same data back to the client's source IP and port.
- **Client** receives the echoed reply and measures the elapsed round-trip time.

### 6. Standard Execution Command (NS-3.47 Setup)
To compile and run any simulation in your installed NS-3 environment:
```bash
cd ~/ns3-lab/ns-3.47
./ns3 run scratch/program_name
```
> [!NOTE]
> `program_name` must be the name of your `.cc` file placed inside `scratch/`, written **without the `.cc` extension**.

---

## Section B — My NS-3 Assignments

### Summary Matrix of Lab Simulations

| Assgn | Topic | Topology & Nodes | Traffic Pattern | Core Metrics Evaluated | Source Location |
|:---:|:---|:---|:---|:---|:---|
| **7** | UDP Latency & Throughput | 2 Nodes (P2P link: 5 Mbps, 2 ms) | UDP Client/Server (1024 B packets, 10 ms interval, 1000 pkts) | Throughput (~0.84 Mbps), PDR (100%), Avg Latency (~3.68 ms) | `A7-UDP-Latency/source-variants/point-to-point-udp-latency.cc` |
| **8** | TCP vs UDP Bottleneck | 6-Node Dumbbell (2 Src, 2 Rtr, 2 Sink; bottleneck: 1 Mbps, 20 ms, Queue: 10p) | TCP BulkSend (port 8080) vs UDP OnOff CBR (800 Kbps, 512 B, port 9090) | Throughput, Packet Loss, DropTail Queue Starvation, PDR | `A8-TCP-UDP-Bottleneck/source-variants/tcp-udp-bottleneck.cc` |
| **9** | TCP Congestion Control | 2 Nodes P2P (10 Mbps, 10 ms) & 5-Flow VANET Sweep | TCP BulkSend comparing `TcpNewReno`, `TcpCubic`, `TcpBbr` | Throughput (~9.89 Mbps), Queue Delay (~48.8 ms), Cwnd evolution, Jain's Fairness Index | `A9-TCP-Congestion-Control/source-variants/tcp-congestion-control.cc` |
| **11** | Wireless Networks with Mobility | 1 AP + 4 Mobile STAs (802.11n, 50×50 m box, 2 m/s RandomWalk2d) | UDP OnOff (1 Mbps, 1024 B) from STAs to AP PacketSink | Signal Strength (dBm), Throughput, Packet Loss %, Delay vs Node Speed (1, 5, 10 m/s) | `A11-Wireless-Mobility/source-variants/wireless-mobility.cc` |

---

### Detailed Assignment Analysis

#### Assignment 7: UDP Traffic, Latency and Throughput
- **General Objective:** Study end-to-end packet delivery, latency, and port-based demultiplexing over point-to-point links.
- **Actual Code Implementation:**
  - *Baseline (`point-to-point-udp-latency.cc`):* 2 nodes (`n0`, `n1`) connected by a 5 Mbps, 2 ms delay link on subnet `10.1.1.0/24`. `n1` runs a `UdpServer` on port 9; `n0` runs a `UdpClient` sending 1000 packets of 1024 bytes at 10 ms intervals. `FlowMonitor` captures metrics.
  - *Scratch Variants:* `udp-latency-experiment-1.cc` models a 4-node topology (`n0-n1-n2` and `n1-n3`) measuring queue delay via custom Tx/Rx FIFO queues; `udp-latency-experiment-2.cc` tests port-based demultiplexing using two flows (port 5000 and 6000) over varying link delays (`--delay="10ms"`).
- **Observed Metrics:**
  - Transmitted: 800 packets | Received: 800 packets
  - Throughput: ~0.8416 Mbps
  - Packet Delivery Ratio (PDR): 100%
  - Average Latency / Delay: ~3.6864 ms
- **Implementation Limitations:** The baseline uses a simple, uncongested 2-node point-to-point link without packet drop or queue competition.

#### Assignment 8: TCP and UDP Traffic through a Bottleneck Link
- **General Objective:** Analyze performance, competition, and fairness between responsive TCP traffic and unresponsive UDP traffic sharing a constrained bottleneck link.
- **Actual Code Implementation:**
  - *Dumbbell Topology (6 Nodes):* `n0` (TCP Source) and `n1` (UDP Source) connect to router `r1` via 10 Mbps, 2 ms access links. Router `r2` connects to `n4` (TCP Sink) and `n5` (UDP Sink) via 10 Mbps, 2 ms access links.
  - *Bottleneck Link (`r1-r2`):* 1 Mbps bandwidth, 20 ms delay, with a strict `DropTailQueue<Packet>` limited to 10 packets (`MaxSize = "10p"`).
  - *Traffic:* Flow 1 is TCP BulkSend (port 8080); Flow 3 is UDP OnOff CBR (800 Kbps, 512-byte packets, port 9090); Flow 2 represents the reverse TCP ACK stream.
- **Observed Metrics & Dynamics:**
  - TCP Flow: Tx: 1399 | Rx: 1307 | Lost: 84 | PDR: ~93.4% | Throughput: ~0.68 Mbps
  - UDP Flow: Tx: 781 | Rx: 529 | Lost: 214 | PDR: ~67.7% | Throughput: ~0.25 Mbps
  - *Viva Key Insight:* Because UDP lacks congestion control, it sends traffic continuously at its fixed rate. As the 10-packet bottleneck buffer fills, packets from both flows are dropped. However, while TCP detects loss and aggressively cuts its congestion window, UDP does not throttle, causing TCP starvation and lower relative throughput.
- **Implementation Limitations:** Uses standard DropTail FIFO queueing; active queue management algorithms (such as RED or CoDel) are not enabled in this baseline.

#### Assignment 9: TCP Congestion Control Evaluation
- **General Objective:** Evaluate TCP variants (`NewReno`, `Cubic`, `BBR`) under network congestion, tracking throughput, delay, packet loss, congestion window ($Cwnd$), and inter-flow fairness.
- **Actual Code Implementation:**
  - *Baseline (`tcp-congestion-control.cc`):* 2 nodes, 10 Mbps, 10 ms delay link. Globally assigns `ns3::TcpL4Protocol::SocketType` to `TcpNewReno`, `TcpCubic`, or `TcpBbr` using command-line argument `--tcpVariant`.
  - *Multi-Flow Experiment (`tcp-congestion-control-experiment.cc`):* Models a VANET/ITS scenario where 5 vehicle nodes transmit bulk TCP data to a server through a roadside router (5 Mbps access, 8 Mbps bottleneck). Connects trace callbacks (`/NodeList/*/SocketList/0/CongestionWindow`) to log real-time $Cwnd$ values to `cwnd-<variant>.dat` and computes Jain's Fairness Index.
- **Variant Comparison:**
  - **TcpNewReno:** Loss-based AIMD (Additive Increase, Multiplicative Decrease). Halves $Cwnd$ upon packet loss; exhibits slow linear recovery.
  - **TcpCubic:** Linux default. Replaces linear growth with a cubic function of elapsed time since the last loss event; accelerates window expansion over high-BDP (Bandwidth-Delay Product) links while remaining stable near the saturation point.
  - **TcpBbr (Bottleneck Bandwidth and RTT):** Model-based congestion control. Measures estimated bottleneck bandwidth and minimum RTT to maximize delivery rate while preventing queue bloat (requires packet pacing enabled).
- **Implementation Limitations:** In the multi-flow experiment, vehicle nodes use point-to-point links parameterized as wireless uplinks rather than full 802.11p/WAVE physical models.

#### Assignment 11: Wireless Networks with Node Mobility
- **General Objective:** Evaluate wireless network performance under mobility across varying speeds and technological configurations.
- **Actual Code Implementation:**
  - *Baseline (`wireless-mobility.cc`):* 1 static Access Point (`wifiApNode` at origin `(0,0,0)`) and 4 mobile stations (`wifiStaNodes`) moving under `RandomWalk2dMobilityModel` within a 50×50 m boundary at a speed of 2.0 m/s. Uses 802.11n at 2.4 GHz (`WIFI_STANDARD_80211n`). FlowMonitor measures ~0.45 Mbps throughput per flow with 100% PDR and ~1.03 ms latency.
  - *Comparison Script (`wireless-mobility-experiment.cc`):* 2 nodes (1 fixed gateway, 1 mobile node moving away with `RandomWaypointMobilityModel` at `--speed=1, 5, 10` m/s). Evaluates signal strength (`SnifferRx`), throughput, packet loss, and delay across technologies.
- **Crucial Implementation Reality / Limitation (Viva Must-Know):**
  > [!IMPORTANT]
  > **NS-3 does not include a native Bluetooth protocol stack**, and simulating full IEEE 802.15.4/6LoWPAN Zigbee requires a completely separate, incompatible subsystem.  
  > Therefore, in this repository, **all three technologies are simulated using NS-3's real `WifiNetDevice` (ad-hoc mode) configured with representative physical parameters**:
  > - **Wi-Fi:** 802.11g, 20 dBm Tx power, 54 Mbps ERP-OFDM, 1024-byte packets.
  > - **Bluetooth:** 802.11b, 4 dBm Tx power (~Class 2), 1 Mbps DSSS, 200-byte packets.
  > - **Zigbee:** 802.11b, -10 dBm Tx power (very low power), 1 Mbps DSSS, 100-byte packets.
  > This is an engineering proxy to demonstrate relative comparative trends across transmit power and packet size, **not a protocol-exact Bluetooth or Zigbee stack**.

---

## Section C — Socket Programming Fundamentals

### 1. Purpose of a Socket and Client-Server Roles
- **Socket:** An abstraction representing an endpoint for two-way network communication across an IP network. In Linux/POSIX, a socket is manipulated as a standard integer file descriptor (`fd`).
- **Client:** The active entity. It initiates communication by establishing a connection (`connect()`) or sending an initial datagram to a known server address and port.
- **Server:** The passive entity. It binds to a well-known local port, marks itself ready to accept incoming connections (`listen()`), and waits for client requests (`accept()` or `recvfrom()`).

### 2. Comprehensive System Calls Reference Table

| Function | Signature / Header | Purpose & Operating Mode | Where Used in Lab |
|---|---|---|---|
| `socket()` | `int socket(int domain, int type, int protocol);` | Creates an unbound socket endpoint and returns a file descriptor. Domain: `AF_INET` (IPv4); Type: `SOCK_STREAM` (TCP) or `SOCK_DGRAM` (UDP). | First call in every client and server. |
| `bind()` | `int bind(int fd, const struct sockaddr *addr, socklen_t len);` | Associates a local IP address and port number with a socket descriptor. | Mandatory on all servers (`tserver.c`, `userver.c`, `dnsserver.c`). |
| `listen()` | `int listen(int fd, int backlog);` | Transitions a TCP socket into passive listening mode. `backlog` sets the queue size for pending connection requests. | TCP servers only, immediately preceding `accept()`. |
| `accept()` | `int accept(int fd, struct sockaddr *addr, socklen_t *len);` | Blocks until a client connection arrives. Extracts the first connection from queue and returns a **new dedicated connected socket descriptor** for client communication. | TCP servers (`tserver.c`, `cs_server.c`, `crc_server.c`). |
| `connect()` | `int connect(int fd, const struct sockaddr *addr, socklen_t len);` | Initiates the TCP 3-way handshake (`SYN` $\rightarrow$ `SYN-ACK` $\rightarrow$ `ACK`) to connect to a remote server. | TCP clients (`tclient.c`, `client.c`, `cs_client.c`, `crc_client.c`). |
| `send()` | `ssize_t send(int fd, const void *buf, size_t len, int flags);` | Transmits data over a connected stream socket. Returns bytes sent or -1 on error. | TCP communication (`client.c`, `cs_client.c`, `crc_client.c`). |
| `recv()` | `ssize_t recv(int fd, void *buf, size_t len, int flags);` | Receives incoming data from a connected stream socket. Returns bytes read, 0 on orderly disconnect (`FIN`), or -1 on error. | TCP communication (`client.c`, `cs_server.c`). |
| `sendto()` | `ssize_t sendto(int fd, const void *buf, size_t len, int flags, const struct sockaddr *to, socklen_t tolen);` | Sends an independent datagram to a specified destination address. | UDP clients and servers (`uclient.c`, `userver.c`, `dnsclient.c`). |
| `recvfrom()` | `ssize_t recvfrom(int fd, void *buf, size_t len, int flags, struct sockaddr *from, socklen_t *fromlen);` | Receives a datagram and captures the sender’s source IP and port in the `from` structure. | UDP servers and clients (`userver.c`, `uclient.c`, `dnsserver.c`). |
| `read()` | `ssize_t read(int fd, void *buf, size_t count);` | Standard POSIX stream read; reads bytes from a stream socket descriptor. | TCP client-server (`tserver.c`, `tclient.c`). |
| `write()` | `ssize_t write(int fd, const void *buf, size_t count);` | Standard POSIX stream write; writes bytes to a stream socket descriptor. | TCP client-server (`tserver.c`, `tclient.c`). |
| `close()` | `int close(int fd);` | Closes socket descriptor, releases file table entry, and triggers TCP `FIN` packet to tear down connection. | Terminating all client and server sockets. |
| `setsockopt()` | `int setsockopt(int fd, int lvl, int opt, const void *val, socklen_t len);` | Modifies socket behavior (e.g., `SO_REUSEADDR` to reuse local port, `SO_RCVTIMEO` for receive timeouts). | Configuring 3s receive timeout in `dnsclient.c`. |
| `select()` | `int select(int n, fd_set *rfds, fd_set *wfds, fd_set *efds, struct timeval *tv);` | Synchronous I/O multiplexing; monitors multiple socket descriptors for readability, writability, or errors without spawning threads. | Non-blocking server architectures. |
| `htons()` | `uint16_t htons(uint16_t hs);` | Host-to-Network Short: Converts 16-bit integer from host byte order (little-endian) to network byte order (big-endian). | Assigning port numbers (`addr.sin_port = htons(8080)`). |
| `htonl()` | `uint32_t htonl(uint32_t hl);` | Host-to-Network Long: Converts 32-bit integer from host byte order to network byte order. | Assigning IPv4 addresses (`addr.sin_addr.s_addr = htonl(INADDR_ANY)`). |
| `ntohs()` | `uint16_t ntohs(uint16_t ns);` | Network-to-Host Short: Converts 16-bit integer from network byte order to host byte order. | Extracting port numbers received from network. |
| `ntohl()` | `uint32_t ntohl(uint32_t nl);` | Network-to-Host Long: Converts 32-bit integer from network byte order to host byte order. | Extracting IPv4 addresses received from network. |

#### Additional Functions from Assignment 1
- `bzero(void *s, size_t n);`: Erases buffer by writing zeros (modern standard: `memset(&addr, 0, sizeof(addr))`).
- `fcntl(int fd, int cmd, ...);`: Controls file/socket descriptor properties (e.g., setting `O_NONBLOCK` for non-blocking I/O).
- `getpeername(int fd, struct sockaddr *addr, socklen_t *len);`: Returns the IP address and port of the remote peer connected to socket `fd`.
- `gethostname(char *name, size_t len);`: Returns the null-terminated hostname of the local machine.
- `gethostbyname(const char *name);`: Resolves a hostname string (e.g., `"localhost"`) to a `struct hostent *` containing IPv4 addresses (used in `http_server_download/client.c`).
- `gethostbyaddr(const void *addr, socklen_t len, int type);`: Performs reverse DNS lookup (maps IP address back to hostname).

### 3. Essential Execution Sequences

```
TCP SERVER:
socket() -> bind() -> listen() -> accept() -> read()/recv() -> write()/send() -> close()

TCP CLIENT:
socket() -> connect() -> write()/send() -> read()/recv() -> close()

UDP COMMUNICATION:
Server : socket() -> bind() ----------> recvfrom() -> sendto() -> close()
                                             ^           |
Client : socket() -> sendto() ---------------+           |
              |                                          v
              +----> recvfrom() <------------------------+
              +----> close()
```

> [!IMPORTANT]
> **Key Viva Distinction:** UDP is completely connectionless. A UDP server **never calls `listen()` or `accept()`**. It binds to a port and waits directly on `recvfrom()`.

---

## Section D — TCP, UDP, Timers and Retransmissions

### 1. TCP vs. UDP Head-to-Head Comparison

| Feature | TCP (`SOCK_STREAM`) | UDP (`SOCK_DGRAM`) |
|---|---|---|
| **Connection State** | Connection-oriented (requires 3-way handshake before data transfer) | Connectionless (no handshake; data sent immediately) |
| **Reliability** | Guaranteed delivery (cumulative ACKs, timeouts, retransmissions) | Best-effort (unreliable; packets may be lost or duplicated) |
| **Data Ordering** | Guaranteed in-order delivery via 32-bit sequence numbers | No ordering guarantee; datagrams can arrive out-of-order |
| **Error Recovery** | Automatic retransmission in kernel on timeout or 3 duplicate ACKs | No native retransmission; application must implement if needed |
| **Overhead** | Higher (20–60 byte header, connection state tables, buffer management) | Minimal (fixed 8-byte header, stateless) |
| **Data Stream Format** | Continuous unstructured byte-stream (no message boundaries) | Preserves discrete packet / datagram boundaries |
| **Flow & Congestion Control** | Supported (Sliding window flow control; AIMD / Cubic / BBR congestion control) | None (transmits as fast as application writes) |
| **Typical Functions** | `socket`, `bind`, `listen`, `accept`, `connect`, `read`, `write`, `close` | `socket`, `bind`, `sendto`, `recvfrom`, `close` |

### 2. Timers, Acknowledgements, and Retransmission Dynamics
- **Timer and Timeout:** When a sender transmits a TCP segment, it starts a Retransmission Timer. If the expected Acknowledgement (ACK) does not arrive before the timer expires, a **timeout** occurs.
- **Acknowledgement (ACK):** A control message returned by the receiver indicating the next sequence number it expects to receive (cumulative acknowledgement).
- **Retransmission:** Upon timer expiry, the sender infers that data was dropped in transit and retransmits the unacknowledged segment.
- **Lost Packet vs. Lost ACK:**
  - *Lost Packet:* The data segment was dropped before reaching the receiver. The receiver sends no ACK. Sender times out and retransmits.
  - *Lost ACK:* The receiver successfully received the data segment and generated an ACK, but the ACK was dropped on the return path. The sender times out and retransmits the same segment. The receiver detects the duplicate sequence number, discards the payload, and re-sends the ACK.
- **RTT (Round-Trip Time) and Premature Timeout:**
  - *RTT:* The elapsed duration for a packet to travel from sender to receiver plus the time for its ACK to return.
  - *Retransmission Timeout (RTO):* Must be set conservatively higher than RTT (typically computed dynamically using Jacobson's smoothed RTT algorithm).
  - *Premature Timeout Hazard:* If the timeout threshold is configured **shorter than the actual RTT**, the timer will expire while the legitimate ACK is still in flight. This triggers unnecessary duplicate retransmissions, wastes bandwidth, and causes false congestion window collapses.
- **UDP Application-Level Reliability:**
  - UDP provides no automatic retransmission in kernel space.
  - In our lab, [`dns_tcp_udp/dnsclient.c`](dns_tcp_udp/dnsclient.c) configures a 3-second receive timeout using `setsockopt(..., SO_RCVTIMEO, ...)`. If a server does not reply, `recvfrom()` unblocks with `-1` (`EWOULDBLOCK`), allowing the user application to log a timeout or retry.

---

## Section E — Error Detection: Checksum and CRC

### 1. Internet Checksum (One's Complement 8-Bit Sum)

```
[ Sender: Accumulate 8-bit words ] ---> [ If sum > 255: Add carry back ] ---> [ Checksum = ~Sum ]
                                                                                      |
[ Receiver: Sum all data words + Checksum byte ] ---> [ If result == 0xFF (255) -> NO ERROR ]
```

- **Data Partitioning:** Input data is divided into 8-bit words (or 16-bit words in standard IP/TCP headers).
- **One's Complement Addition:** Words are added together. Whenever an arithmetic carry occurs (sum exceeds `0xFF` / 255), the carry bit is wrapped around and added to the least significant bit (end-around carry):
  $$\text{sum} = (\text{sum} \ \& \ \text{0xFF}) + (\text{sum} \gg 8)$$
- **Sender Checksum Generation:** The checksum byte is the bitwise one's complement (inversion) of the final accumulated sum: `checksum = ~sum & 0xFF;`. The sender transmits data words followed by the checksum byte.
- **Receiver Verification:** The receiver adds all received data words **plus the checksum byte** using the same one's complement rules.
  - If no errors occurred: $\text{Sum} + \sim\text{Sum} = \text{0xFF}$ (`11111111` binary, 255 decimal).
  - If the final sum equals `0xFF`, the packet is declared **error-free**. Otherwise, an **error is detected**.
- **Tiny Worked Example (Hexadecimal):**
  - Data bytes: `0x12` and `0x34`
  - Sum = `0x12 + 0x34 = 0x46` (no carry)
  - Checksum = `~0x46 = 0xB9`
  - Receiver checks: `0x12 + 0x34 + 0xB9 = 0x46 + 0xB9 = 0xFF` (255). Error-free!

### 2. Cyclic Redundancy Check (CRC)

```
[ Data Bits: D ] ---> [ Append k Zeros: D || 0000 ] ---> [ Modulo-2 Division by G(x) ] ---> [ Remainder R ]
                                                                                                    |
[ Transmitted Frame: T = D || R ] <-----------------------------------------------------------------+
```

- **Binary Representation:** Data is treated as a bit string representing polynomial coefficients.
- **Generator Polynomial $G(x)$:** Predefined divisor agreed upon by sender and receiver. In our lab [`crc/`](crc/), $G(x) = x^4 + x + 1$, corresponding to binary `"10011"` (degree $k = 4$, length 5).
- **Sender Encoding Algorithm:**
  1. Append $k = 4$ zeros to the original binary data string (`data || "0000"`).
  2. Perform **modulo-2 binary division** (XOR subtraction at each step where the leading bit is 1; no borrows or carries).
  3. The resulting 4-bit remainder is the CRC checksum ($R$).
  4. Append $R$ to the original data to form the transmitted frame ($T = \text{data} \ || \ R$).
- **Receiver Verification Algorithm:**
  1. Divide the complete received frame by the identical generator polynomial `"10011"` using modulo-2 division.
  2. If the remainder is all zeros (`0000`), the frame is declared **error-free**.
  3. If any remainder bit is 1, an **error is detected**.
- **Bit-Error Demonstration in Lab:**
  In [`crc/crc_server.c`](crc/crc_server.c), the server prompts: `Introduce error? (1=yes, 0=no):`. Entering `1` and specifying a bit index flips that bit (`'0'` $\leftrightarrow$ `'1'`). Modulo-2 division then yields a non-zero remainder, verifying that CRC catches corrupted bits.

> [!WARNING]
> **Viva Pitfall:** CRC division uses **modulo-2 arithmetic (XOR)**, NOT standard mathematical integer division. Checksum uses **one's-complement arithmetic with end-around carry**, NOT polynomial division.

---

## Section F — Application Layer: DNS and HTTP File Download

### 1. DNS Resolution over UDP (`dns_tcp_udp/`)
- **Purpose of DNS:** Maps human-readable domain names (e.g., `google.com`) into 32-bit machine-routable IPv4 addresses (e.g., `142.250.72.14`).
- **Client and Server Roles:**
  - *Client:* Queries the server by sending a domain name inside a UDP datagram.
  - *Server:* Looks up the domain in an internal table and replies with the matching IP string.
- **Key Features in Repository Implementation:**
  - *Static Lookup Table:* Pre-configured in `dnsserver.c` (`google.com`, `youtube.com`, `github.com`, `example.com`, `ssn.edu.in`).
  - *Client-Side Caching (`dnsclient.c`):* Stores resolved queries in memory (`cache_name[20]`, `cache_ip[20]`). If a domain is re-queried, it prints `Cache hit!` and avoids sending any network packet.
  - *`NXDOMAIN` (Non-Existent Domain):* If the queried hostname is not in the database, the server returns `"NXDOMAIN"`, and the client prints `Domain not found`.
  - *Input Validation:* Checks that domain contains a dot (`.`) and neither begins nor ends with a dot (`Invalid domain format`).
  - *Timeout Simulation:* When querying `timeout.com`, `dnsserver.c` intentionally drops the packet (`continue`). The client has a 3-second receive timeout configured via `SO_RCVTIMEO`. When 3 seconds elapse with no response, `recvfrom()` returns `-1`, and the client outputs `Request timed out`.
- **Why UDP for DNS?**
  - Minimal overhead: 8-byte UDP header vs. 20-byte TCP header.
  - Fast single-round-trip resolution: No 3-way handshake setup or connection teardown required for a 50-byte query.
  - *(Viva Note: DNS falls back to TCP port 53 for responses exceeding 512 bytes / EDNS0 or zone transfers).*

### 2. Local HTTP Webpage and File Download (`http_server_download/`)
- **Architecture:** A C client program (`client.c`) connects over raw TCP sockets to a local web server to download web resources and profile performance.
- **Step-by-Step Execution Sequence:**
  1. *Host Assets:* Place sample assets (`index.html`, `sample.pdf`, `image.jpg`, `notes.txt`) in `server_files/`.
  2. *Start Python Server (Terminal 1):*
     ```bash
     cd http_server_download/server_files
     python3 -m http.server 8000
     ```
  3. *Compile and Run C Client (Terminal 2):*
     ```bash
     cd http_server_download
     gcc -Wall -Wextra client.c -o client
     ./client
     ```
  4. *Client Input:* User supplies a full URL: `http://localhost:8000/index.html`.
  5. *URL Parsing:* `sscanf()` extracts host (`localhost`), port (`8000`), and resource path (`/index.html`).
  6. *DNS / IP Lookup:* `gethostbyname("localhost")` resolves hostname to loopback address (`127.0.0.1`).
  7. *TCP Connection:* `connect()` initiates 3-way handshake to port 8000.
  8. *HTTP GET Request:* Formats and transmits RFC-compliant HTTP request:
     ```http
     GET /index.html HTTP/1.1\r\n
     Host: localhost\r\n
     Connection: close\r\n\r\n
     ```
  9. *Header / Body Separation:* Client locates double CRLF delimiter (`\r\n\r\n`) using `strstr()`. The HTTP body begins exactly 4 bytes after this delimiter.
  10. *Verification & Save:* Checks for `HTTP/1.1 200 OK`, opens local file (`fopen("index.html", "wb")`), and writes payload via `fwrite()`.
  11. *Performance Profiling:* Measures download time using `clock()` and reports data size and throughput:
      $$\text{Throughput (KB/s)} = \frac{\text{body\_size (Bytes)}}{1024.0 \times \text{elapsed\_seconds}}$$

> [!TIP]
> **Optional HTTPS Client Note:** The syllabus outlines an optional HTTPS client using OpenSSL APIs (`SSL_connect()`, port 443). In our baseline coursework, standard HTTP/1.1 over raw TCP sockets is the primary evaluated implementation.

---

## Section G — Performance Metrics and Formulas

### 1. Essential Metrics Reference Table

| Metric | Definition | Mathematical Formula | Standard Units |
|---|---|---|---|
| **Throughput** | Rate of successful data delivery over a channel during active transfer | $\text{Throughput} = \frac{\text{Total Received Bits}}{\text{Measurement Duration}}$ | bps, Kbps, Mbps |
| **Latency / One-Way Delay** | Time taken for a packet to travel from sender to destination | $\text{Latency} = t_{\text{receive}} - t_{\text{transmit}}$ | ms or $\mu$s |
| **Round-Trip Time (RTT)** | Total time elapsed from sending a request until receiving its response | $\text{RTT} = t_{\text{response\_received}} - t_{\text{request\_sent}}$ | ms |
| **Packet Delivery Ratio (PDR)** | Percentage of transmitted packets successfully delivered to receiver | $\text{PDR} = \left(\frac{\text{Received Packets}}{\text{Transmitted Packets}}\right) \times 100\%$ | % |
| **Packet Loss Ratio** | Percentage of transmitted packets dropped or lost in transit | $\text{Loss Ratio} = \left(\frac{\text{Lost Packets}}{\text{Transmitted Packets}}\right) \times 100\% = 100\% - \text{PDR}$ | % |
| **Average Delay (FlowStats)** | Mean end-to-end transmission delay across all received packets | $\text{Avg Delay} = \frac{\sum \text{Packet Delays}}{\text{Total Received Packets}}$ | ms |
| **Jain's Fairness Index** | Measures equality of bandwidth allocation among $n$ competing flows | $J(x_1, \dots, x_n) = \frac{\left(\sum_{i=1}^n x_i\right)^2}{n \cdot \sum_{i=1}^n x_i^2}$ | Dimensionless (Range: $\frac{1}{n}$ to $1.0$) |

### 2. Deep Dive: Jain's Fairness Index
- In Assignment 9 multi-flow experiment ([`tcp-congestion-control-experiment.cc`](A9-TCP-Congestion-Control/scratch-programs/tcp-congestion-control-experiment.cc)), Jain's Fairness Index is calculated across $n$ active TCP flows:
  - $x_i$: Measured throughput of flow $i$.
  - $n$: Total number of competing flows.
  - $\sum x_i$: Sum of throughputs.
  - $\sum x_i^2$: Sum of squared throughputs.
- **Interpretation:**
  - $J = 1.0$: Absolute fairness; every flow receives exactly equal bandwidth.
  - $J = 1/n$: Worst-case unfairness; one flow monopolizes all bandwidth while all other $n-1$ flows receive 0.

### 3. Tiny Numerical Viva Example
- **Scenario:** A sender transmits 100 packets. The receiver successfully accepts 80 packets; 20 are dropped.
- **Calculations:**
  - $\text{PDR} = \frac{80}{100} \times 100\% = 80\%$
  - $\text{Packet Loss Ratio} = \frac{20}{100} \times 100\% = 20\%$
  - Check: $\text{PDR} + \text{Loss Ratio} = 80\% + 20\% = 100\%$.

### 4. One-Way Delay vs. Round-Trip Time (RTT)
- **One-Way Delay:** Unidirectional measurement ($A \rightarrow B$). Requires clock synchronization between sender and receiver.
- **RTT:** Bidirectional round-trip measurement ($A \rightarrow B \rightarrow A$). Measured on a single clock at node $A$, eliminating clock synchronization errors:
  $$\text{RTT} \approx 2 \times \text{One-Way Propagation Delay} + \text{Transmission Delay} + \text{Queuing Delay} + \text{Server Processing Time}$$
- **Measurement Validity:** A metric is only valid when the measurement time window is strictly defined. For example, in Assignment 7, throughput is computed over the active 8.0-second traffic window, not the total 10.0-second simulation lifetime.

---

## Section H — Top 35 High-Yield Viva Questions and Short Answers

1. **What is NS-3?**  
   NS-3 is an open-source, discrete-event network simulator written in C++ that models computer network protocols, topologies, and packet traffic purely in software.

2. **What is the difference between a network simulator and real network communication?**  
   A simulator models protocol logic, links, and queues using a virtual software clock without real hardware; real communication executes across physical network interfaces, cables, and physical operating system kernels subject to real electrical conditions.

3. **What is a Node in NS-3?**  
   A Node (`NodeContainer`) is a virtual computing device in the simulation that acts as a host, router, access point, or switch by aggregating network devices and protocol stacks.

4. **What is the purpose of a Helper class in NS-3?**  
   Helpers provide high-level abstractions that automate the creation, configuration, and installation of complex low-level simulation components (channels, devices, stacks, and applications) across multiple nodes.

5. **What does `InternetStackHelper` do?**  
   It aggregates the TCP/IP network and transport layer protocols (IPv4, ARP, TCP, UDP) onto virtual nodes.

6. **What is the difference between wired and wireless simulations in NS-3?**  
   Wired simulations use dedicated point-to-point links with deterministic delay and fixed bandwidth; wireless simulations use shared radio broadcast channels with CSMA/CA contention, signal path loss, and optional node mobility.

7. **What is UDP Echo?**  
   A simple diagnostic application where a client sends a UDP packet to a server, and the server immediately echoes the exact same packet back to verify connectivity.

8. **What is TCP?**  
   TCP (Transmission Control Protocol) is a reliable, connection-oriented, byte-stream transport protocol that guarantees in-order packet delivery using acknowledgements, sequence numbers, and retransmissions.

9. **What is UDP?**  
   UDP (User Datagram Protocol) is a lightweight, connectionless, unreliable transport protocol that transmits discrete datagrams with minimal overhead and no delivery guarantees.

10. **Why does TCP use acknowledgements (ACKs)?**  
    To inform the sender which bytes have been successfully received, enabling reliable transmission, flow control, and retransmission of lost segments.

11. **What is a socket?**  
    A socket is an operating system abstraction and communication endpoint identified by an IP address and port number, accessed as a file descriptor.

12. **What is the difference between `bind()` and `connect()`?**  
    `bind()` associates a socket with a specific local IP address and port number (used by servers); `connect()` initiates a connection from the client to a remote IP and port (used by TCP clients).

13. **Why does a TCP server need both `listen()` and `accept()`?**  
    `listen()` transitions the socket into passive mode with a backlog queue for incoming connection requests; `accept()` extracts the first connection from the queue and creates a new connected socket for client communication.

14. **Why do UDP programs use `sendto()` and `recvfrom()` instead of `read()` and `write()`?**  
    Because UDP is connectionless; each datagram is independent and must explicitly specify or capture the destination or source IP address and port number.

15. **What is a timeout?**  
    A timeout occurs when a sender’s countdown timer expires before the expected acknowledgement or response is received from the remote peer.

16. **Why does packet retransmission happen?**  
    Retransmission occurs when a sender infers packet loss after a retransmission timer expires or upon receiving 3 duplicate ACKs (Fast Retransmit).

17. **What is an Internet Checksum?**  
    An error-detection mechanism where the sender calculates the one's complement sum of data words and inverts it; the receiver re-sums the data plus checksum to verify if the result equals all 1s (`0xFF`).

18. **What is CRC (Cyclic Redundancy Check)?**  
    An error-detecting code based on polynomial modulo-2 binary division where the remainder of dividing the data bit-string by a generator polynomial is appended to the transmitted frame.

19. **Why does CRC use modulo-2 division instead of regular integer division?**  
    Modulo-2 division is implemented with bitwise XOR gates without carry or borrow operations, making it extremely fast, computationally efficient, and ideal for hardware and bit-level error detection.

20. **What is DNS?**  
    Domain Name System (DNS) is an application-layer service that translates human-readable hostnames into numeric IP addresses.

21. **What is client-side DNS caching?**  
    A client-side optimization that stores recently resolved hostname-to-IP mappings in local memory, eliminating unnecessary network round-trips for repeated queries.

22. **What is an HTTP GET request?**  
    An application-layer request method sent by a client to retrieve a specific resource (webpage, file, image) from a web server.

23. **Why do we run a local Python HTTP server in Assignment 3?**  
    To provide a controllable, accessible HTTP/1.1 endpoint on `localhost:8000` hosting static files (`index.html`, `sample.pdf`) without requiring internet access.

24. **What is throughput?**  
    The effective rate of successfully delivered data bits over a communication channel per unit of time (bits per second).

25. **What is latency?**  
    The time required for a data packet to travel across a network from source to destination.

26. **What is Round-Trip Time (RTT)?**  
    The total time taken for a packet to travel from sender to receiver plus the time taken for its acknowledgement or response to return.

27. **What is Packet Delivery Ratio (PDR)?**  
    The ratio of total successfully received packets to total transmitted packets, expressed as a percentage.

28. **What happens to PDR when packet loss increases?**  
    PDR decreases proportionally, because $\text{PDR} = 100\% - \text{Packet Loss Ratio}$.

29. **Why does node mobility degrade wireless network performance?**  
    Mobile nodes move farther from access points, experiencing path loss, weaker signal strength, higher bit error rates, collisions, and packet drops.

30. **What is a bottleneck link?**  
    The link in a network path with the lowest bandwidth or highest congestion, which constrains the overall throughput of traffic passing through it.

31. **What happens when TCP and UDP compete across a bottleneck link (Assignment 8)?**  
    UDP transmits aggressively at a fixed rate without throttling; TCP detects congestion, cuts its window, and suffers bandwidth starvation.

32. **What is TCP Congestion Control?**  
    A mechanism used by TCP to prevent network collapse by dynamically adjusting the sender's congestion window ($Cwnd$) based on observed network congestion.

33. **How does TCP Cubic differ from TCP Reno?**  
    Reno uses linear additive increase ($Cwnd + 1$ per RTT); Cubic uses a cubic function of time since last loss, scaling rapidly over high-BDP networks while stabilizing near capacity.

34. **What is Jain's Fairness Index?**  
    A metric ranging from $1/n$ to $1.0$ that quantifies how equally network bandwidth is shared among $n$ competing flows.

35. **How are Bluetooth and Zigbee simulated in Assignment 11?**  
    NS-3 lacks native Bluetooth; both are simulated using ad-hoc `WifiNetDevice` configured with representative Tx power, data rates, and packet sizes (Bluetooth: 4 dBm, 1 Mbps, 200 B; Zigbee: -10 dBm, 1 Mbps, 100 B).

---

## Section I — Final 5-Minute Memory Sheet

### 1. NS-3 Workflow in One Line
$$\text{Create Nodes} \longrightarrow \text{Install Devices/Channels} \longrightarrow \text{Install Internet Stack} \longrightarrow \text{Assign IPs} \longrightarrow \text{Install Apps} \longrightarrow \text{Schedule Start/Stop} \longrightarrow \text{Run} \longrightarrow \text{Destroy}$$

### 2. Socket Lifecycles at a Glance
- **TCP Server:** `socket()` $\rightarrow$ `bind()` $\rightarrow$ `listen()` $\rightarrow$ `accept()` $\rightarrow$ `read()` / `write()` $\rightarrow$ `close()`
- **TCP Client:** `socket()` $\rightarrow$ `connect()` $\rightarrow$ `write()` $\rightarrow$ `read()` $\rightarrow$ `close()`
- **UDP Direct:** `socket()` $\rightarrow$ `bind()` (server only) $\rightarrow$ `recvfrom()` / `sendto()` $\rightarrow$ `close()`

### 3. Checksum vs. CRC Core Rule
- **Checksum:** 1's complement addition with end-around carry; valid if final sum $= \text{0xFF}$ (255).
- **CRC:** Modulo-2 polynomial division (XOR); valid if remainder $= 0000$.

### 4. Application Protocols in One Line
- **DNS:** UDP client sends hostname string $\rightarrow$ server responds with IP string $\rightarrow$ client caches result $\rightarrow$ 3s timeout via `SO_RCVTIMEO`.
- **HTTP Download:** Client parses URL $\rightarrow$ connects TCP port 8000 $\rightarrow$ sends `GET /file HTTP/1.1` $\rightarrow$ parses headers past `\r\n\r\n` $\rightarrow$ writes body to disk.

### 5. Critical Formulas Snapshot
- $\text{Throughput} = \frac{\text{Rx Bits}}{\text{Active Seconds}}$
- $\text{PDR} = \frac{\text{Rx Packets}}{\text{Tx Packets}} \times 100\%$
- $\text{Loss Ratio} = 100\% - \text{PDR}$
- $\text{One-Way Delay} = t_{\text{rx}} - t_{\text{tx}}$
- $\text{RTT} \approx 2 \times \text{Delay} + \text{Processing}$
- $\text{Jain's Fairness} = \frac{(\sum x_i)^2}{n \sum x_i^2}$

### 6. Throughput vs. Latency vs. RTT in One Line
- **Throughput** is *volume over time* (capacity); **Latency** is *one-way transit time*; **RTT** is *round-trip transit time*.

### 7. Assignments 7, 8, 9, 11 in One Sentence Each
- **Assignment 7:** Measures point-to-point UDP latency (~3.68 ms) and throughput (~0.84 Mbps) using `UdpClient/UdpServer` and `FlowMonitor`.
- **Assignment 8:** Demonstrates TCP starvation by greedy UDP traffic across a 1 Mbps / 10-packet DropTail bottleneck in a 6-node dumbbell topology.
- **Assignment 9:** Evaluates TCP `NewReno`, `Cubic`, and `BBR` over a congested link, showing Cubic's superior window scaling and measuring Jain's Fairness Index.
- **Assignment 11:** Models wireless mobility with 802.11n mobile nodes, demonstrating that increased node speed degrades signal strength, throughput, and PDR.
