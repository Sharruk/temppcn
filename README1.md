# Protocols and Computer Networks (PCN) Laboratory

**Course:** UCS3511 — Networks Laboratory  
**Institution:** Sri Sivasubramaniya Nadar College of Engineering (SSN)  
**Department:** Computer Science & Engineering  

This repository contains implementations, simulations, experimental results, and analytical reports for 11 laboratory assignments covering:
1. **Linux Network Diagnostics & C Socket System Calls**
2. **Concurrent Multi-Client TCP & Datagram UDP Client-Server Programming**
3. **Application-Layer HTTP/1.1 Client & Local HTTP Asset Server**
4. **Packet Inspection & Wireshark Trace Analysis**
5. **UDP-Based DNS Server Simulation with Client Caching & Error Handling**
6. **Data Link Layer Error Detection (1's Complement Checksum & CRC-4 Polynomial Division)**
7. **NS-3 Discrete-Event Simulation: Multi-hop Point-to-Point Links & Port-Based UDP Demultiplexing**
8. **NS-3 Transport Layer Congestion & Queueing: TCP Reno vs. UDP over a Bottleneck Link**
9. **NS-3 TCP Congestion Control Evaluation (Reno vs. Cubic vs. BBR) in a VANET / ITS Scenario**
10. **NS-3 Routing Protocol Comparative Study: RIP vs. OSPF vs. AODV under Congestion & Link Failure**
11. **NS-3 Heterogeneous Wireless Technologies: Wi-Fi, Bluetooth, and Zigbee under Varying Node Mobility**

---

## Table of Contents

- [Quick Reference Index](#quick-reference-index)
- [General Environment & Prerequisites](#general-environment--prerequisites)
  - [C Socket Programming Environment](#1-c-socket-programming-environment)
  - [NS-3 Simulation Environment (Docker Container)](#2-ns-3-simulation-environment-docker-container)
  - [Packet Inspection (Wireshark)](#3-packet-inspection-wireshark)
- [Assignment Details](#assignment-details)
  - [Assignment 1: Network Diagnostics & Socket API System Calls](#assignment-1-network-diagnostics--socket-api-system-calls)
  - [Assignment 2: Multi-Client TCP & UDP Student Academic Record Query](#assignment-2-multi-client-tcp--udp-student-academic-record-query)
  - [Assignment 3: HTTP/1.1 Client over Raw TCP Sockets](#assignment-3-http11-client-over-raw-tcp-sockets)
  - [Assignment 4: Wireshark Packet Sniffer Lab - HTTP GET & Embedded Objects](#assignment-4-wireshark-packet-sniffer-lab---http-get--embedded-objects)
  - [Assignment 5: UDP DNS Server Simulation with Client Caching & Error Handling](#assignment-5-udp-dns-server-simulation-with-client-caching--error-handling)
  - [Assignment 6: Reliable Data Transmission via Checksum and CRC](#assignment-6-reliable-data-transmission-via-checksum-and-crc)
  - [Assignment 7: NS-3 Network Basics & Port-Based UDP Traffic Delivery](#assignment-7-ns-3-network-basics--port-based-udp-traffic-delivery)
  - [Assignment 8: TCP & UDP Performance over Bottleneck Link in NS-3](#assignment-8-tcp--udp-performance-over-bottleneck-link-in-ns-3)
  - [Assignment 9: TCP Congestion Control (Reno, Cubic, BBR) in VANET/ITS](#assignment-9-tcp-congestion-control-reno-cubic-bbr-in-vanetits)
  - [Assignment 10: Routing Protocols Evaluation (RIP vs. OSPF vs. AODV)](#assignment-10-routing-protocols-evaluation-rip-vs-ospf-vs-aodv)
  - [Assignment 11: Heterogeneous Wireless Networks with Mobility (Wi-Fi, Bluetooth, Zigbee)](#assignment-11-heterogeneous-wireless-networks-with-mobility-wi-fi-bluetooth-zigbee)
- [Summary of Captured Outputs & Artifacts](#summary-of-captured-outputs--artifacts)

---

## Quick Reference Index

| Assgn # | Topic | Key Technologies / Tools | Core Output / Deliverables | Location |
|:---:|:---|:---|:---|:---|
| **01** | Network Diagnostic Tools & Socket System Calls | `netstat`, `ifconfig`, `iwconfig`, `ping`, `route`, `nmap`, `tcpdump`, `dig`, `traceroute`, `mtr`, `curl`, `wget`, `tracepath`, `nslookup`, `host`, `whois`, POSIX Sockets | 16 CLI terminal captures, system call syntax & state transition analysis | [`ass1/`](file:///c:/Users/Sundareswaran/pcn/PCN/ass1) |
| **02** | Multi-Client Academic Record Retrieval | C (`pthread`, `AF_INET`, `SOCK_STREAM`, `SOCK_DGRAM`), CSV database | TCP full profile retrieval vs. UDP verification, concurrency logs | [`ass2/`](file:///c:/Users/Sundareswaran/pcn/PCN/ass2) |
| **03** | HTTP/1.1 Socket Client & Asset Retrieval | C sockets, HTTP/1.1 protocol parsing, Python `http.server` | Downloaded HTML, TXT, PDF, JPG; throughput & latency profiling | [`ass3/`](file:///c:/Users/Sundareswaran/pcn/PCN/ass3) |
| **04** | Wireshark Packet Inspection of HTTP Exchanges | Wireshark sniffer, HTTP protocol, TCP streams, caching | Wireshark packet traces (`file1`, `file4`, redirects, parallel downloads) | [`ass4/`](file:///c:/Users/Sundareswaran/pcn/PCN/ass4) |
| **05** | UDP Domain Name Resolution Simulation | C UDP sockets, hash cache table, non-blocking timeout | Cache hit/miss, `NXDOMAIN`, invalid format, timeout demo | [`ass5/`](file:///c:/Users/Sundareswaran/pcn/PCN/ass5) |
| **06** | Reliable Data Transmission: Checksum & CRC | C TCP sockets, 1's complement addition, polynomial division $G(x)=x^4+x+1$ | Step-by-step intermediate calculation, error-free & bit-injected verification | [`ass6/`](file:///c:/Users/Sundareswaran/pcn/PCN/ass6) |
| **07** | NS-3 Simulation: Multi-hop UDP & Port Demux | NS-3 (v3.42) C++, `PointToPointHelper`, `UdpClient`/`UdpServer` | Exp 1: 4-node 2-flow latency; Exp 2: 2-node port 5000/6000 latency sweep | [`ass7/`](file:///c:/Users/Sundareswaran/pcn/PCN/ass7) |
| **08** | NS-3 Dumbbell Topology: TCP vs. UDP Dynamics | NS-3 C++, `DropTailQueue`, `FlowMonitor`, `BulkSendHelper` | Baseline + 4 Experiments: BW (2/5/10 Mbps), UDP rate, queue size, delay | [`ass8/`](file:///c:/Users/Sundareswaran/pcn/PCN/ass8) |
| **09** | NS-3 TCP Congestion Control (Reno/Cubic/BBR) | NS-3 C++, `TcpNewReno`, `TcpCubic`, `TcpBbr`, Matplotlib | Per-flow throughput, loss, delay, Jain's Fairness Index, $Cwnd$ vs. time plots | [`ass9/`](file:///c:/Users/Sundareswaran/pcn/PCN/ass9) |
| **10** | NS-3 Dynamic Routing: RIP vs. OSPF vs. AODV | NS-3 C++, `RipHelper`, `GlobalRoutingHelper`, `AodvHelper`, CSMA | Normal, Congested, and Link-Failure runs; PDR, delay, update count plots | [`ass10/`](file:///c:/Users/Sundareswaran/pcn/PCN/ass10) |
| **11** | NS-3 Heterogeneous Wireless Mobility | NS-3 C++, `WifiNetDevice`, `ConstantVelocityMobilityModel` | Wi-Fi vs. Bluetooth vs. Zigbee at 1, 5, 10 m/s; signal, throughput, loss, delay | [`ass11/`](file:///c:/Users/Sundareswaran/pcn/PCN/ass11) |

---

## General Environment & Prerequisites

### 1. C Socket Programming Environment
Assignments 2, 3, 5, and 6 are written in standard C99/POSIX C.
- **Compiler:** `gcc` (with `-Wall -Wextra -pthread`)
- **Build tool:** `make`
- **Supported Platforms:** Linux, macOS, WSL2, or Docker.

### 2. NS-3 Simulation Environment (Docker Container)
Assignments 7, 8, 9, 10, and 11 use **NS-3 (Network Simulator 3.42)** running on Ubuntu 24.04 inside a persistent Docker container named `pcn_ns3`.

#### Container Management Commands:
```bash
# 1. Start the container if it is stopped
docker start pcn_ns3

# 2. Open an interactive shell inside the container
docker exec -it pcn_ns3 bash

# 3. Inside the container, navigate to NS-3 installation
cd /root/ns-3-dev

# 4. Build any updated scratch programs
./ns3 build
```

#### Copying NS-3 Code into the Container:
```bash
# Example for Assignment 8:
docker cp ass8/ns-3-work/scratch/ass8.cc pcn_ns3:/root/ns-3-dev/scratch/ass8.cc
```

### 3. Packet Inspection (Wireshark)
Assignment 4 relies on Wireshark. Ensure dumpcap privileges are granted on Linux:
```bash
sudo usermod -aG wireshark $USER
newgrp wireshark
```

---

## Assignment Details

---

### Assignment 1: Network Diagnostics & Socket API System Calls
**Specification File:** [`ass1/ass1.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass1/ass1.pdf)  
**Report Document:** [`ass1/report_sundar.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass1/report_sundar.pdf)  
**Screenshots Folder:** [`ass1/screenshots/`](file:///c:/Users/Sundareswaran/pcn/PCN/ass1/screenshots)  

#### 1. What It Is
An empirical and analytical study split into two parts:
- **Part A:** Practical execution, parameter exploration, and observation of 16 critical network diagnostics and monitoring command-line tools.
- **Part B:** In-depth architectural analysis of 24 POSIX socket programming system calls, utility functions, and network address conversion routines.

#### 2. Tools Covered & How to Run Each (Part A)
Each command was executed in a terminal and captured in [`ass1/screenshots/`](file:///c:/Users/Sundareswaran/pcn/PCN/ass1/screenshots):

1. **`netstat`** (`01_netstat.png`): Inspects kernel routing tables, network connections, and socket states.
   ```bash
   netstat -rn
   netstat -tuln
   ```
2. **`ifconfig`** (`02_ifconfig.png`): Queries and configures active network interface cards (MAC address, IP, MTU, broadcast).
   ```bash
   ifconfig en0
   ```
3. **`iwconfig`** (`03_iwconfig.png`): Inspects wireless parameters (SSID, frequency, bit rate, transmit power, link quality).
   ```bash
   iwconfig wlan0
   ```
4. **`ping`** (`04_ping.png`): Tests ICMP ECHO_REQUEST reachability, round-trip time (RTT), and packet loss.
   ```bash
   ping -c 4 8.8.8.8
   ```
5. **`route`** (`05_route.png`): Displays and manipulates the IP routing table.
   ```bash
   route -n
   ```
6. **`nmap`** (`06_nmap.png`): Network exploration, port scanner, and security auditor.
   ```bash
   nmap -sT -p 22,80,443 scanme.nmap.org
   ```
7. **`tcpdump`** (`07_tcpdump.png`): Command-line packet sniffer capturing live raw frames.
   ```bash
   sudo tcpdump -i any -c 5 -nn icmp
   ```
8. **`dig`** (`08_dig.png`): Domain Information Groper for querying DNS name servers.
   ```bash
   dig google.com +noall +answer
   ```
9. **`traceroute`** (`09_traceroute.png`): Discovers hop-by-hop layer-3 routers via incrementing IP TTL values.
   ```bash
   traceroute -m 15 google.com
   ```
10. **`mtr`** (`10_mtr.png`): Combines the functionality of `traceroute` and `ping` into an interactive dynamic diagnostic tool.
    ```bash
    mtr -r -c 5 google.com
    ```
11. **`curl`** (`11_curl.png`): Command-line HTTP/HTTPS client retrieving raw headers and bodies.
    ```bash
    curl -I https://www.google.com
    ```
12. **`wget`** (`12_wget.png`): Non-interactive network downloader.
    ```bash
    wget -S --spider https://www.google.com
    ```
13. **`tracepath`** (`13_tracepath.png`): Traces network path while discovering Path MTU (PMTU) without root privileges.
    ```bash
    tracepath google.com
    ```
14. **`nslookup`** (`14_nslookup.png`): Interactive query tool for DNS records.
    ```bash
    nslookup google.com
    ```
15. **`host`** (`15_host.png`): Simple DNS lookup resolving domain names to IPv4/IPv6 addresses and mail exchangers.
    ```bash
    host google.com
    ```
16. **`whois`** (`16_whois.png`): Queries WHOIS databases for domain registrar, creation date, expiry, and contact info.
    ```bash
    whois google.com
    ```

#### 3. Socket System Calls Analyzed (Part B)
Detailed purpose, signature, arguments, return values, and TCP/UDP context for 24 functions:
- **Core Life Cycle:** `socket()`, `bind()`, `listen()`, `connect()`, `accept()`, `close()`.
- **Byte Order & Memory Manipulation:** `bzero()`, `htons()`, `htonl()`, `ntohs()`, `ntohl()`.
- **I/O & Data Transfer:** `read()`, `write()`, `send()`, `recv()`, `sendto()`, `recvfrom()`.
- **Socket Options & Multiplexing:** `select()`, `setsockopt()`, `fcntl()`.
- **Address Resolution & Discovery:** `getpeername()`, `gethostname()`, `gethostbyname()`, `gethostbyaddr()`.

---

### Assignment 2: Multi-Client TCP & UDP Student Academic Record Query
**Specification File:** [`ass2/ass2.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass2/ass2.pdf)  
**Source Code:** [`ass2/tcp_server.c`](file:///c:/Users/Sundareswaran/pcn/PCN/ass2/tcp_server.c), [`ass2/tcp_client.c`](file:///c:/Users/Sundareswaran/pcn/PCN/ass2/tcp_client.c), [`ass2/udp_server.c`](file:///c:/Users/Sundareswaran/pcn/PCN/ass2/udp_server.c), [`ass2/udp_client.c`](file:///c:/Users/Sundareswaran/pcn/PCN/ass2/udp_client.c)  
**Database File:** [`ass2/students.csv`](file:///c:/Users/Sundareswaran/pcn/PCN/ass2/students.csv)  
**Algorithms & Comparison:** [`ass2/algorithms.md`](file:///c:/Users/Sundareswaran/pcn/PCN/ass2/algorithms.md), [`ass2/comparison.md`](file:///c:/Users/Sundareswaran/pcn/PCN/ass2/comparison.md)  
**Captured Output:** [`ass2/output.txt`](file:///c:/Users/Sundareswaran/pcn/PCN/ass2/output.txt)  

#### 1. What It Is
A university academic information lookup service implemented in two paradigms:
1. **TCP Implementation:** Multi-threaded (`pthread`) server listening on port 8080. When a student sends a Roll Number, the server searches `students.csv` and returns the **complete academic profile** (`Roll Number`, `Name`, `Department`, `Semester`, `CGPA`).
2. **UDP Implementation:** Datagram-based stateless server listening on port 8081. Returns **verification only** (`Roll Number`, `Student Name`) to minimize bandwidth overhead.
3. **Protocol Comparison:** Comprehensive comparison between TCP and UDP regarding Connection, Reliability, Ordering, Error Recovery, and Communication Overhead.

#### 2. Architecture & CSV Database Format
`students.csv` structure:
```csv
RollNumber,Name,Department,Semester,CGPA
101,Aarav Patel,Computer Science,5,9.1
102,Diya Sharma,Information Technology,5,8.7
103,Rohan Iyer,Computer Science,3,8.4
104,Ananya Sen,Electronics,7,9.3
```

#### 3. How to Compile & Run
```bash
cd ass2
make clean && make
```

##### Running TCP Test Cases:
- **Terminal 1 (Server):**
  ```bash
  ./tcp_server
  ```
- **Terminal 2 (Client — Found Record):**
  ```bash
  ./tcp_client
  # Input: 101
  ```
- **Terminal 3 (Client — Not Found Record):**
  ```bash
  ./tcp_client
  # Input: 999
  ```

##### Running UDP Test Cases:
- **Terminal 1 (Server):**
  ```bash
  ./udp_server
  ```
- **Terminal 2 (Client — Found Record):**
  ```bash
  ./udp_client
  # Input: 102
  ```
- **Terminal 3 (Client — Not Found Record):**
  ```bash
  ./udp_client
  # Input: 999
  ```

#### 4. Expected Outputs
##### TCP Client Output:
```
=== TCP client: found (Roll 101) ===
Enter Roll Number: 101
--- Student Details ---
Roll Number : 101
Name        : Aarav Patel
Department  : Computer Science
Semester    : 5
CGPA        : 9.1
-----------------------

=== TCP client: not found (Roll 999) ===
Enter Roll Number: 999
--- Student Details ---
Student Record Not Found
-----------------------
```

##### UDP Client Output:
```
=== UDP client: found (Roll 102) ===
Enter Roll Number: 102
--- Student Details ---
Roll Number : 102
Student Name: Diya Sharma
-----------------------

=== UDP client: not found (Roll 999) ===
Enter Roll Number: 999
--- Student Details ---
Student Record Not Found
-----------------------
```

---

### Assignment 3: HTTP/1.1 Client over Raw TCP Sockets
**Specification File:** [`ass3/ass3.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass3/ass3.pdf)  
**Source Code:** [`ass3/http_client.c`](file:///c:/Users/Sundareswaran/pcn/PCN/ass3/http_client.c)  
**Asset Generator:** [`ass3/CN_Web_Server/generate_assets.py`](file:///c:/Users/Sundareswaran/pcn/PCN/ass3/CN_Web_Server/generate_assets.py)  
**Algorithm Details:** [`ass3/algorithms.md`](file:///c:/Users/Sundareswaran/pcn/PCN/ass3/algorithms.md)  
**Captured Output:** [`ass3/output.txt`](file:///c:/Users/Sundareswaran/pcn/PCN/ass3/output.txt)  

#### 1. What It Is
An application-layer HTTP/1.1 client written in C from scratch using POSIX TCP stream sockets. It connects to a Python-hosted web server (`localhost:8000`), parses arbitrary input URLs (`http://<host>:<port>/<path>`), formats and transmits an `HTTP/1.1 GET` request, receives the HTTP response, splits the HTTP header block (`\r\n\r\n`) from the payload body, writes binary/text data to disk as `downloaded_<basename>`, and computes rigorous communication performance metrics.

#### 2. Resources Downloaded
Inside [`ass3/CN_Web_Server/`](file:///c:/Users/Sundareswaran/pcn/PCN/ass3/CN_Web_Server):
- `index.html`: Web page HTML structure (168 B)
- `notes.txt`: Plain text document (159 B)
- `sample.pdf`: Multi-page binary PDF document (33,790 B)
- `image.jpg`: Binary JPEG image (332 B)

#### 3. How to Compile & Run
##### Step 1: Generate assets and start the Python HTTP Server
```bash
cd ass3
make assets
cd CN_Web_Server
python3 -m http.server 8000
```

##### Step 2: In a separate terminal, compile and run the HTTP Client
```bash
cd ass3
make
```

Run for each resource type:
```bash
# 1. Download HTML web page
./http_client
# Input: http://localhost:8000/index.html

# 2. Download text notes
./http_client
# Input: http://localhost:8000/notes.txt

# 3. Download binary PDF document
./http_client
# Input: http://localhost:8000/sample.pdf

# 4. Download JPEG image
./http_client
# Input: http://localhost:8000/image.jpg
```

#### 4. Expected Outputs & Metrics Profile
From [`ass3/output.txt`](file:///c:/Users/Sundareswaran/pcn/PCN/ass3/output.txt):
```
=== Request: http://localhost:8000/sample.pdf ===
Client extracts:
Host : localhost
Port : 8000
Path : /sample.pdf

Then sends:
GET /sample.pdf HTTP/1.1
Host: localhost
Connection: close

--- Performance Metrics ---
Response Time : 0.492 ms
Download Time : 0.668 ms (0.000668 seconds)
Data Size     : 33790 bytes (33.00 KB)
Throughput    : 49398.27 KB/s
Saved resource to: downloaded_sample.pdf
---------------------------
```

Metric Formulas:
- $\text{Response Time} = T_{\text{first\_byte\_received}} - T_{\text{request\_sent}}$
- $\text{Download Time} = T_{\text{last\_byte\_received}} - T_{\text{first\_byte\_received}}$
- $\text{Throughput} = \frac{\text{Data Size (KB)}}{\text{Download Time (s)}}$

---

### Assignment 4: Wireshark Packet Sniffer Lab - HTTP GET & Embedded Objects
**Specification File:** [`ass4/Assignment4_CN_Mtech.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass4/Assignment4_CN_Mtech.pdf)  
**Report Document:** [`ass4/report_sundar.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass4/report_sundar.pdf)  
**Captured Traces:** [`ass4/shot1_file1_get.png`](file:///c:/Users/Sundareswaran/pcn/PCN/ass4/shot1_file1_get.png) through [`ass4/shot5_embedded_objects_zoom.png`](file:///c:/Users/Sundareswaran/pcn/PCN/ass4/shot5_embedded_objects_zoom.png)  

#### 1. What It Is
An empirical packet-sniffing lab analyzing the behavior of the HTTP protocol on live network connections using Wireshark:
- **Part 1:** Basic HTTP GET/Response interaction (`HTTP-wireshark-file1.html`).
- **Part 2:** Conditional GET/Response and Web Browser Caching (`HTTP-wireshark-file2.html` using `If-Modified-Since` and `304 Not Modified`).
- **Part 3:** HTTP Redirection (`301 Moved Permanently` header analysis).
- **Part 4:** Retrieving HTML documents containing multiple embedded objects across distinct web servers (`HTTP-wireshark-file4.html`).

#### 2. Step-by-Step Execution Guide
1. Launch Wireshark and select your active network interface (e.g. `eth0`, `en0`, `wlan0`).
2. Set display filter:
   ```wireshark
   http
   ```
3. Open a browser and load the test URLs:
   - Base interaction: `http://gaia.cs.umass.edu/wireshark-labs/HTTP-wireshark-file1.html`
   - Conditional caching: Load `http://gaia.cs.umass.edu/wireshark-labs/HTTP-wireshark-file2.html`, then hit refresh.
   - Long file: `http://gaia.cs.umass.edu/wireshark-labs/HTTP-wireshark-file3.html`
   - Embedded objects: `http://gaia.cs.umass.edu/wireshark-labs/HTTP-wireshark-file4.html`
4. Stop packet capture and examine the request/response headers in the packet details pane.

#### 3. Key Observations & Answers
- **HTTP Version:** Both browser and server negotiate `HTTP/1.1`.
- **Accepted Languages:** Browser sends `Accept-Language: en-US,en;q=0.9`.
- **IP Addressing:** Client IP `10.6.13.15`, Gaia Server IP `128.119.245.12`.
- **Status Codes:** First request yields `200 OK`; cached request yields `304 Not Modified`; redirected URL yields `301 Moved Permanently`.
- **Embedded Objects Interaction:** 
  - Loading `file4.html` triggers **3 HTTP GET requests**:
    1. `GET /wireshark-labs/HTTP-wireshark-file4.html` $\rightarrow$ `128.119.245.12`
    2. `GET /pearson.png` $\rightarrow$ `128.119.245.12`
    3. `GET /8E_cover_small.jpg` $\rightarrow$ `2.56.99.24` (different server!)
  - The browser opens parallel TCP connections: the second image request is issued at $t = 30.608\text{ s}$ before the first image's response is received ($t = 31.194\text{ s}$), demonstrating **parallel object retrieval**.

---

### Assignment 5: UDP DNS Server Simulation with Client Caching & Error Handling
**Specification File:** [`ass5/ass5.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass5/ass5.pdf)  
**Source Code:** [`ass5/server.c`](file:///c:/Users/Sundareswaran/pcn/PCN/ass5/server.c), [`ass5/client.c`](file:///c:/Users/Sundareswaran/pcn/PCN/ass5/client.c)  
**Captured Output:** [`ass5/output.txt`](file:///c:/Users/Sundareswaran/pcn/PCN/ass5/output.txt)  
**Report Document:** [`ass5/report_sundar.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass5/report_sundar.pdf)  

#### 1. What It Is
A complete DNS name resolution simulation using connectionless UDP sockets (`SOCK_DGRAM`) on port 8080.
- **Server:** Maintains a DNS table (domain $\rightarrow$ IP), validates domain structure, simulates non-existent domains (`NXDOMAIN`), and reproduces unresponsive networks (timeouts).
- **Client:** Implements a local client-side cache table (capacity of 10 records) to avoid redundant network queries, handles query timeouts using `select()` with a 3-second deadline, and displays resolution status.
- **Protocol Analysis:** Evaluates why UDP is favored over TCP for DNS queries in terms of 0-RTT connection overhead, message sizing, and transport efficiency.

#### 2. How to Compile & Run
```bash
cd ass5
gcc -Wall -Wextra -o server server.c
gcc -Wall -Wextra -o client client.c
```

- **Terminal 1 (Server):**
  ```bash
  ./server
  ```
- **Terminal 2 (Client):**
  ```bash
  ./client
  ```

#### 3. Test Cases to Execute at Client Prompt
1. **Cache Miss (First Query):** Enter `google.com` $\rightarrow$ Queries server, prints IP `142.250.183.14`, adds to client cache.
2. **Cache Hit (Repeated Query):** Enter `google.com` $\rightarrow$ Immediately returns from local cache without contacting server.
3. **Domain Not Found (NXDOMAIN):** Enter `notfound.com` $\rightarrow$ Server responds `NXDOMAIN`, client reports `Domain Not Found.`
4. **Invalid Domain Format:** Enter `invalidhost` (no period) $\rightarrow$ Server detects malformed query, responds `ERROR: Invalid Format`.
5. **Timeout Simulation:** Enter `timeout.com` $\rightarrow$ Server deliberately sleeps for 5 seconds; client's 3-second `select()` timer expires and outputs `Request Timed Out!`.
6. **Exit:** Enter `exit` to close client.

#### 4. Expected Client Output
```
=========== DNS Client ===========

Enter Domain (or exit): google.com
Cache Miss! Querying Server...
IP Address : 142.250.183.14

----- Client Cache -----
google.com --> 142.250.183.14
------------------------

Enter Domain (or exit): google.com
Cache Hit!
IP Address : 142.250.183.14

Enter Domain (or exit): unknownsite.org
Cache Miss! Querying Server...
Domain Not Found.

Enter Domain (or exit): badurl
Cache Miss! Querying Server...
Invalid Domain Format.

Enter Domain (or exit): timeout.com
Cache Miss! Querying Server...
Request Timed Out!
```

---

### Assignment 6: Reliable Data Transmission via Checksum and CRC
**Specification File:** [`ass6/Ass6.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass6/Ass6.pdf)  
**Source Code:** [`ass6/server.c`](file:///c:/Users/Sundareswaran/pcn/PCN/ass6/server.c), [`ass6/client.c`](file:///c:/Users/Sundareswaran/pcn/PCN/ass6/client.c)  
**Build File:** [`ass6/Makefile`](file:///c:/Users/Sundareswaran/pcn/PCN/ass6/Makefile)  
**Captured Outputs:** [`ass6/output_checksum.txt`](file:///c:/Users/Sundareswaran/pcn/PCN/ass6/output_checksum.txt), [`ass6/output_crc.txt`](file:///c:/Users/Sundareswaran/pcn/PCN/ass6/output_crc.txt)  
**Report Document:** [`ass6/report_sundar.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass6/report_sundar.pdf)  

#### 1. What It Is
A reliable data transmission application in C over TCP stream sockets implementing two data link layer error-detection mechanisms:
1. **1's Complement Checksum:** 
   - Divides arbitrary input string into 8-bit words.
   - Computes sum, folds overflow carries back into the lowest 8 bits (end-around carry), and takes 1's complement (`~sum & 0xFF`).
   - Server recomputes the sum over received data plus checksum byte; an error-free frame evaluates to `0x00`.
2. **Cyclic Redundancy Check (CRC):**
   - Implements polynomial division modulo-2 using standard generator polynomial:
     $$G(x) = x^4 + x + 1 \quad \implies \quad 10011_2 \text{ (0x13)}$$
   - Displays binary representations and intermediate linear feedback shift register states.
   - Server performs identical polynomial division; zero remainder signifies error-free transmission.
3. **Intentional Error Injection:** Allows the user to flip any selected bit (positions 0–7) in transmission to prove detection capability.

#### 2. How to Compile & Run
```bash
cd ass6
make clean && make
```

##### Terminal 1: Start Server
```bash
./server
```

##### Terminal 2: Run Client - Mode 1 (Checksum)
```bash
./client
# Prompt: Choose (1/2): 1
# Prompt: Enter data: HELLO
# Prompt: Demonstrate error detection? (y/n): n   # Run 1: Error-free
# Repeat with 'y' and flip bit 0                 # Run 2: Error injected
```

##### Terminal 2: Run Client - Mode 2 (CRC)
```bash
./client
# Prompt: Choose (1/2): 2
# Prompt: Enter data: TEST
# Prompt: Demonstrate error detection? (y/n): n   # Run 1: Error-free
# Repeat with 'y' and flip bit 0                 # Run 2: Error injected
```

#### 3. Expected Outputs
##### Checksum Computation & Verification:
```
=== CHECKSUM MODE ===
Enter data: HELLO
[Intermediate Sum Calculation]
  Step 1: 0x48 + Sum = 0x0048
  Step 2: 0x45 + Sum = 0x008D
  Step 3: 0x4C + Sum = 0x00D9
  Step 4: 0x4C + Sum = 0x0125
  Step 5: 0x4F + Sum = 0x0174
  Total Sum: 0x0174
  Carry (End-around): 0x01
  After adding carry: 0x0075
  1's Complement (Checksum): 0x8A

[Server Output]
[Server] Received: HELLO (6 bytes)
[CHECKSUM] Received: 0x8A, Calculated: 0x8A
[Result] ERROR-FREE
```

##### Error Detection Verification:
When bit 0 is flipped:
```
[Server] Received: IELLO (6 bytes)
[CHECKSUM] Received: 0x8A, Calculated: 0x89
[Result] ERROR DETECTED!
```

---

### Assignment 7: NS-3 Network Basics & Port-Based UDP Traffic Delivery
**Specification Files:** [`ass7/NS3_Exp1.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass7/NS3_Exp1.pdf), [`ass7/udpns3.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass7/udpns3.pdf)  
**Simulation Source:** [`ass7/ns-3-work/scratch/exp1.cc`](file:///c:/Users/Sundareswaran/pcn/PCN/ass7/ns-3-work/scratch/exp1.cc), [`ass7/ns-3-work/scratch/exp2.cc`](file:///c:/Users/Sundareswaran/pcn/PCN/ass7/ns-3-work/scratch/exp2.cc)  
**Captured Outputs:** [`ass7/output_exp1.txt`](file:///c:/Users/Sundareswaran/pcn/PCN/ass7/output_exp1.txt), [`ass7/output_exp2.txt`](file:///c:/Users/Sundareswaran/pcn/PCN/ass7/output_exp2.txt)  
**Report Document:** [`ass7/report_sundar.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass7/report_sundar.pdf)  

#### 1. What It Is
Two baseline simulations in **NS-3 (v3.42)** modeling computer networks without real hardware:
- **Experiment 1 (`exp1.cc`):** 4-node network connected via three 5 Mbps, 2 ms point-to-point links. Two concurrent UDP flows:
  - Flow 1: $n_0 \rightarrow n_2$ at 1 Mbps.
  - Flow 2: $n_3 \rightarrow n_0$ at 500 Kbps.
  - Measures total bytes, throughput, and packet latency using direct application-level `Tx`/`Rx` trace sinks (explicitly without `FlowMonitor`).
- **Experiment 2 (`exp2.cc`):** 2 nodes connected by a point-to-point link. Demonstrates transport-layer port demultiplexing using two independent `UdpClient`/`UdpServer` pairs on ports 5000 and 6000 while sweeping link latency across 2 ms, 5 ms, 10 ms, and 20 ms. Generates PCAP traces for Wireshark inspection.

#### 2. Topologies

##### Experiment 1:
```
n0 ───── n1 ───── n2
          │
          n3
Link specs: 5 Mbps, 2 ms delay
Flow 1: n0 -> n2 (1 Mbps UDP)
Flow 2: n3 -> n0 (500 Kbps UDP)
```

##### Experiment 2:
```
Node1 ───────────── Node2
        P2P Link
App 1: Port 5000 (1 Mbps UDP)
App 2: Port 6000 (1 Mbps UDP)
Latency swept: 2ms, 5ms, 10ms, 20ms
```

#### 3. How to Run Inside Docker
```bash
docker start pcn_ns3
docker exec -it pcn_ns3 bash
cd /root/ns-3-dev
```

##### Run Experiment 1:
```bash
./ns3 run scratch/exp1
```

##### Run Experiment 2 (Latency Sweeps):
```bash
# 1. Single Flow (Port 5000 only)
for d in 2ms 5ms 10ms 20ms; do
  ./ns3 run "scratch/exp2 --delay=$d --twoFlows=false"
done

# 2. Two Flows (Port 5000 and Port 6000)
for d in 2ms 5ms 10ms 20ms; do
  ./ns3 run "scratch/exp2 --delay=$d --twoFlows=true"
done
```

#### 4. Expected Outputs
##### Experiment 1 Output:
```
===== Flow 1 (n0 -> n2) =====
Total bytes received : 998912 bytes
Throughput           : 0.998912 Mbps
Avg packet latency   : 5.7344 ms

===== Flow 2 (n3 -> n0) =====
Total bytes received : 436736 bytes
Throughput           : 0.499127 Mbps
Avg packet latency   : 8.6016 ms
```

##### Experiment 2 Throughput Sweep Table:
| Propagation Delay | Single Flow Throughput | Dual Flow (Port 5000) | Dual Flow (Port 6000) |
|:---:|:---:|:---:|:---:|
| **2 ms** | 1.00024 Mbps | 1.00024 Mbps | 1.00024 Mbps |
| **5 ms** | 0.99942 Mbps | 0.99942 Mbps | 0.99942 Mbps |
| **10 ms** | 0.99861 Mbps | 0.99861 Mbps | 0.99861 Mbps |
| **20 ms** | 0.99615 Mbps | 0.99615 Mbps | 0.99615 Mbps |

*Insight:* UDP throughput remains almost completely insensitive to propagation delay because UDP does not require round-trip acknowledgments before transmitting subsequent packets.

---

### Assignment 8: TCP & UDP Performance over Bottleneck Link in NS-3
**Specification File:** [`ass8/Assgn8_PCN.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass8/Assgn8_PCN.pdf)  
**Simulation Source:** [`ass8/ns-3-work/scratch/ass8.cc`](file:///c:/Users/Sundareswaran/pcn/PCN/ass8/ns-3-work/scratch/ass8.cc)  
**Captured Output:** [`ass8/output.txt`](file:///c:/Users/Sundareswaran/pcn/PCN/ass8/output.txt)  
**Report Document:** [`ass8/report_sundar.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass8/report_sundar.pdf)  

#### 1. What It Is
A classic **dumbbell network topology** simulation in NS-3 studying how a congestion-controlled, loss-adaptive transport protocol (**TCP Reno**) behaves when forced to compete for a narrow bottleneck link with an aggressive, rate-based, non-adaptive protocol (**UDP**).
- **Default Parameters:** Access links: 10 Mbps, 2 ms delay; Bottleneck link: 2 Mbps, 10 ms delay; Queue: DropTail 50 packets; TCP packet: 1024 B; UDP packet: 512 B at 1 Mbps.
- **Metrics Collected:** Packet loss %, End-to-end delay, Throughput, and TCP retransmission rate using NS-3's `FlowMonitor`.

#### 2. Network Topology
```
Node 0 (TCP Sender) ──┐                                   ┌── Node 3 (TCP Receiver)
                       ├── Router 1 ═══════ Router 2 ─────┤
Node 1 (UDP Sender) ──┘    (Bottleneck: 2Mbps, 10ms)      └── Node 2 (UDP Receiver)
[Access: 10Mbps, 2ms]                                      [Access: 10Mbps, 2ms]
```

#### 3. How to Run Inside Docker
```bash
docker start pcn_ns3
docker exec -it pcn_ns3 bash
cd /root/ns-3-dev
./ns3 build
```

##### Command for Baseline:
```bash
./ns3 run scratch/ass8
```

##### Commands for Required & Optional Parameter Sweeps:
```bash
# Experiment 1: Bottleneck Bandwidth (2, 5, 10 Mbps)
./ns3 run "scratch/ass8 --bottleneckBw=2Mbps"
./ns3 run "scratch/ass8 --bottleneckBw=5Mbps"
./ns3 run "scratch/ass8 --bottleneckBw=10Mbps"

# Experiment 2: UDP Traffic Rate (500Kbps, 1Mbps, 2Mbps, 5Mbps)
./ns3 run "scratch/ass8 --udpRate=500Kbps"
./ns3 run "scratch/ass8 --udpRate=1Mbps"
./ns3 run "scratch/ass8 --udpRate=2Mbps"
./ns3 run "scratch/ass8 --udpRate=5Mbps"

# Experiment 3: Queue Size (20, 50, 100 packets)
./ns3 run "scratch/ass8 --queueSize=20"
./ns3 run "scratch/ass8 --queueSize=50"
./ns3 run "scratch/ass8 --queueSize=100"

# Experiment 4: Bottleneck Propagation Delay (5, 10, 20, 50 ms)
./ns3 run "scratch/ass8 --bottleneckDelay=5ms"
./ns3 run "scratch/ass8 --bottleneckDelay=10ms"
./ns3 run "scratch/ass8 --bottleneckDelay=20ms"
./ns3 run "scratch/ass8 --bottleneckDelay=50ms"
```

#### 4. Expected Results & Key Findings
##### Experiment 1: Varying Bottleneck Bandwidth
| Bottleneck BW | TCP Throughput | TCP Loss % | TCP Delay | UDP Throughput | UDP Loss % | UDP Delay |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| **2 Mbps** | 0.990 Mbps | 1.95% | 173.56 ms | 1.001 Mbps | 4.23% | 170.77 ms |
| **5 Mbps** | 3.927 Mbps | 0.21% | 83.93 ms | 1.050 Mbps | 0.00% | 79.43 ms |
| **10 Mbps** | 8.890 Mbps | 0.05% | 54.20 ms | 1.052 Mbps | 0.00% | 49.59 ms |

*Crucial Takeaway:*
- At 2 Mbps, the 1 Mbps UDP stream consumes half the bottleneck capacity. When queue drops occur, TCP cuts its congestion window in half, while UDP continues transmitting blindly.
- When bottleneck bandwidth expands to 10 Mbps, TCP scales up to take 8.89 Mbps with virtually zero losses ($0.05\%$) and lower queueing delay.

---

### Assignment 9: TCP Congestion Control (Reno, Cubic, BBR) in VANET/ITS
**Specification File:** [`ass9/Assign9_PCN.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass9/Assign9_PCN.pdf)  
**Simulation Source:** [`ass9/ns-3-work/scratch/ass9.cc`](file:///c:/Users/Sundareswaran/pcn/PCN/ass9/ns-3-work/scratch/ass9.cc)  
**Plotting Script:** [`ass9/plot.py`](file:///c:/Users/Sundareswaran/pcn/PCN/ass9/plot.py)  
**Observation Summary:** [`ass9/observation.md`](file:///c:/Users/Sundareswaran/pcn/PCN/ass9/observation.md)  
**Captured Output:** [`ass9/output.txt`](file:///c:/Users/Sundareswaran/pcn/PCN/ass9/output.txt)  
**Report Document:** [`ass9/report_sundar.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass9/report_sundar.pdf)  
**Generated Figures:** [`ass9/screenshots/cwnd_comparison.png`](file:///c:/Users/Sundareswaran/pcn/PCN/ass9/screenshots/cwnd_comparison.png), [`ass9/screenshots/metrics_comparison.png`](file:///c:/Users/Sundareswaran/pcn/PCN/ass9/screenshots/metrics_comparison.png)  

#### 1. What It Is
Simulates an Intelligent Transportation System (ITS) / Vehicular Ad-Hoc Network (VANET). 5 vehicles concurrently transmit telemetry and camera video data to a Traffic Control Server through a Roadside Unit (RSU) and an 8 Mbps, 20 ms bottleneck link.
Compares three fundamental TCP congestion control paradigms:
1. **TCP Reno:** Loss-based with Additive Increase / Multiplicative Decrease (AIMD).
2. **TCP Cubic:** Loss-based with a cubic window growth function independent of RTT.
3. **TCP BBR (Bottleneck Bandwidth and RTT):** Model-based congestion control that probes for maximum delivery rate and minimum RTT without relying on packet loss.

#### 2. Architecture & Topology
```
Vehicle 0 ──┐
Vehicle 1 ──┤
Vehicle 2 ──┼── Roadside Unit (RSU) ═══════ Internet ═══════ Traffic Control Server
Vehicle 3 ──┤                       (Bottleneck: 8Mbps, 20ms)
Vehicle 4 ──┘
```

#### 3. How to Run Inside Docker
```bash
docker start pcn_ns3
docker exec -it pcn_ns3 bash
cd /root/ns-3-dev
./ns3 build

# Run all 3 congestion control algorithms
./ns3 run "scratch/ass9 --tcpVariant=Reno"
./ns3 run "scratch/ass9 --tcpVariant=Cubic"
./ns3 run "scratch/ass9 --tcpVariant=Bbr"

# Render the comparison graphs
python3 plot.py
```

#### 4. Expected Results & Metrics
From [`ass9/output.txt`](file:///c:/Users/Sundareswaran/pcn/PCN/ass9/output.txt):
| TCP Variant | Aggregate Throughput | Avg End-to-End Delay | Packet Loss % | Jain's Fairness Index |
|:---:|:---:|:---:|:---:|:---:|
| **Reno** | 1.570 Mbps / flow | 124.60 ms | 0.38% | **0.9994** |
| **Cubic** | 1.581 Mbps / flow | 139.80 ms | 0.91% | **0.9986** |
| **BBR** | 1.547 Mbps / flow | 125.80 ms | 1.49% | **0.9995** |

*ITS Justification:*
For Intelligent Transportation Systems, low delay and determinism are far more critical than raw bulk throughput; delayed collision warnings or traffic telemetry lose their utility if stuck in high-delay buffers.

---

### Assignment 10: Routing Protocols Evaluation (RIP vs. OSPF vs. AODV)
**Specification File:** [`ass10/Assign10_PCN.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass10/Assign10_PCN.pdf)  
**Simulation Source:** [`ass10/ns-3-work/scratch/ass10.cc`](file:///c:/Users/Sundareswaran/pcn/PCN/ass10/ns-3-work/scratch/ass10.cc)  
**Automation Script:** [`ass10/run_sweep.sh`](file:///c:/Users/Sundareswaran/pcn/PCN/ass10/run_sweep.sh)  
**Plotting Script:** [`ass10/plot.py`](file:///c:/Users/Sundareswaran/pcn/PCN/ass10/plot.py)  
**Captured Output:** [`ass10/output.txt`](file:///c:/Users/Sundareswaran/pcn/PCN/ass10/output.txt)  
**Report Document:** [`ass10/report_sundar.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass10/report_sundar.pdf)  
**Generated Figures:** [`ass10/screenshots/pdr_vs_condition.png`](file:///c:/Users/Sundareswaran/pcn/PCN/ass10/screenshots/pdr_vs_condition.png), [`ass10/screenshots/delay_vs_condition.png`](file:///c:/Users/Sundareswaran/pcn/PCN/ass10/screenshots/delay_vs_condition.png), [`ass10/screenshots/updates_vs_condition.png`](file:///c:/Users/Sundareswaran/pcn/PCN/ass10/screenshots/updates_vs_condition.png)  

#### 1. What It Is
A multi-router network evaluation comparing three routing philosophies:
1. **Distance-Vector (RIP):** Bellman-Ford algorithm with periodic advertisement messages.
2. **Link-State (OSPF stand-in):** Dijkstra's Shortest Path First implemented via NS-3 `Ipv4GlobalRoutingHelper` recomputation.
3. **On-Demand Reactive (AODV):** Ad-hoc On-Demand Distance Vector using route requests (RREQ) and route replies (RREP) over broadcast-capable CSMA links.

Evaluated under 3 environmental conditions:
- **Normal:** Baseline CBR/UDP traffic from $n_0 \rightarrow n_1$.
- **Congestion:** Added cross-traffic from $n_2 \rightarrow n_3$.
- **Link Failure:** Intermediate link $r_1 - r_2$ is torn down mid-simulation at $t = 5.0\text{ s}$, testing automatic rerouting via $r_1 - r_4 - r_3$.

#### 2. Diamond Network Topology
```
      n0                                       n1
        \                                     /
         r1 ──────── r2 ─────────────── r3
        /              \               /  \
      n2                 ───── r4 ─────    n3
```

#### 3. How to Run Inside Docker
```bash
docker start pcn_ns3
docker exec -it pcn_ns3 bash
cd /root/ns-3-dev
./ns3 build

# Option A: Run the complete automated 9-combination sweep
chmod +x run_sweep.sh
./run_sweep.sh

# Option B: Run individual experiments
./ns3 run "scratch/ass10 --protocol=RIP --condition=normal"
./ns3 run "scratch/ass10 --protocol=RIP --condition=failure"
./ns3 run "scratch/ass10 --protocol=OSPF --condition=failure"
./ns3 run "scratch/ass10 --protocol=AODV --condition=congestion"

# Render the 3 comparison graphs
python3 plot.py
```

#### 4. Expected Sweep Results
| Protocol | Condition | Packet Delivery Ratio (PDR) | End-to-End Delay | Routing Updates Count |
|:---|:---|:---:|:---:|:---:|
| **RIP** | Normal | 94.90% | 25.34 ms | 36 |
| **RIP** | Congestion | 89.62% | 375.90 ms | 36 |
| **RIP** | **Link Failure** | **39.42%** | **25.34 ms** | **46** |
| **OSPF** | Normal | 99.86% | 25.34 ms | 1 |
| **OSPF** | Congestion | 94.26% | 381.49 ms | 1 |
| **OSPF** | **Link Failure** | **99.32%** | **25.34 ms** | **2** |
| **AODV** | Normal | 98.22% | 26.52 ms | 2 |
| **AODV** | Congestion | 58.67% | 741.36 ms | 6 |
| **AODV** | **Link Failure** | **98.22%** | **26.52 ms** | **4** |

*Key Findings:*
- When a link fails, RIP suffers from slow convergence (counting-to-infinity / periodic timer delay), dropping PDR to $39.42\%$.
- OSPF stand-in recalculates the Dijkstra tree immediately, maintaining $99.32\%$ delivery.
- AODV re-establishes an alternate path on demand via RREQ/RREP, restoring delivery to $98.22\%$, but struggles heavily under high congestion ($58.67\%$ PDR) due to route broadcast collision.

---

### Assignment 11: Heterogeneous Wireless Networks with Mobility (Wi-Fi, Bluetooth, Zigbee)
**Specification File:** [`ass11/Assign11_PCN.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass11/Assign11_PCN.pdf)  
**Simulation Source:** [`ass11/ns-3-work/scratch/ass11.cc`](file:///c:/Users/Sundareswaran/pcn/PCN/ass11/ns-3-work/scratch/ass11.cc)  
**Plotting Script:** [`ass11/plot.py`](file:///c:/Users/Sundareswaran/pcn/PCN/ass11/plot.py)  
**Observation Summary:** [`ass11/observation.md`](file:///c:/Users/Sundareswaran/pcn/PCN/ass11/observation.md)  
**Captured Output:** [`ass11/output.txt`](file:///c:/Users/Sundareswaran/pcn/PCN/ass11/output.txt)  
**Report Document:** [`ass11/report_sundar.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass11/report_sundar.pdf)  
**Generated Figures:** [`ass11/screenshots/mobility_comparison.png`](file:///c:/Users/Sundareswaran/pcn/PCN/ass11/screenshots/mobility_comparison.png)  

#### 1. What It Is
Simulates a Smart Campus Heterogeneous Wireless Network in NS-3 consisting of three wireless technologies:
- **Wi-Fi (802.11g):** High power (20 dBm), 54 Mbps PHY, 1024 B MTU.
- **Bluetooth (Class 2):** Medium power (4 dBm), 1 Mbps PHY, 200 B MTU.
- **Zigbee (802.15.4):** Low power (-10 dBm), 1 Mbps PHY, 100 B MTU.

Nodes move outward from the base gateway using a deterministic constant-velocity mobility model at **1 m/s**, **5 m/s**, and **10 m/s**, capturing the degradation of signal strength (RSSI), throughput collapse, and packet loss as distance increases.

#### 2. How to Run Inside Docker
```bash
docker start pcn_ns3
docker exec -it pcn_ns3 bash
cd /root/ns-3-dev
./ns3 build

# Sweep all 9 technology and mobility speed combinations
for t in WiFi Bluetooth Zigbee; do
  for s in 1 5 10; do
    ./ns3 run "scratch/ass11 --tech=$t --speed=$s"
  done
done

# Render the 4-panel graph
python3 plot.py
```

#### 3. Expected Results & Mobility Degradation Table
From [`ass11/output.txt`](file:///c:/Users/Sundareswaran/pcn/PCN/ass11/output.txt):
| Technology | Speed | Received Signal Power | Throughput | Packet Loss % | End-to-End Delay |
|:---|:---:|:---:|:---:|:---:|:---:|
| **Wi-Fi** | 1 m/s | -61.96 dBm | 205.47 Kbps | **0.00%** | 0.247 ms |
| **Wi-Fi** | 5 m/s | -68.40 dBm | 58.91 Kbps | 71.33% | 0.561 ms |
| **Wi-Fi** | 10 m/s | -69.98 dBm | 25.54 Kbps | 87.57% | 0.436 ms |
| **Bluetooth** | 1 m/s | -73.26 dBm | 137.18 Kbps | 39.82% | 2.357 ms |
| **Bluetooth** | 5 m/s | -76.21 dBm | 21.13 Kbps | 90.73% | 2.376 ms |
| **Bluetooth** | 10 m/s | -78.78 dBm | 6.60 Kbps | 97.10% | 2.423 ms |
| **Zigbee** | 1 m/s | -77.25 dBm | 35.13 Kbps | 86.27% | 1.567 ms |
| **Zigbee** | 5 m/s | Link severed | **0.00 Kbps** | **100.00%** | — |
| **Zigbee** | 10 m/s | Link severed | **0.00 Kbps** | **100.00%** | — |

*Key Findings:*
- **Signal Attenuation:** Signal strength drops logarithmically with distance according to the log-distance path loss model.
- **Coverage Ranking:** $\text{Wi-Fi} > \text{Bluetooth} > \text{Zigbee}$. Due to Zigbee's minimal transmit power (-10 dBm), moving at $5\text{ m/s}$ places the node out of reception range almost immediately, causing $100\%$ loss.
- **Delay Invariance:** Delivered packets experience roughly constant propagation and transmission delay; mobility affects *whether* a packet is received, not transit time.

---

## Summary of Captured Outputs & Artifacts

| Assignment | Primary Code / Script | Configuration / Spec File | Result Data File | Generated Visualizations |
|:---|:---|:---|:---|:---|
| **Ass 1** | Terminal CLI / Sockets | [`ass1/ass1.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass1/ass1.pdf) | Shell output logs | 16 command screenshots in `screenshots/` |
| **Ass 2** | `tcp_server.c`, `udp_server.c` | [`ass2/students.csv`](file:///c:/Users/Sundareswaran/pcn/PCN/ass2/students.csv) | [`ass2/output.txt`](file:///c:/Users/Sundareswaran/pcn/PCN/ass2/output.txt) | Server and client terminal execution records |
| **Ass 3** | `http_client.c` | `CN_Web_Server/` assets | [`ass3/output.txt`](file:///c:/Users/Sundareswaran/pcn/PCN/ass3/output.txt) | Downloaded assets (`downloaded_*`) |
| **Ass 4** | Wireshark sniffer | [`ass4/Assignment4_CN_Mtech.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass4/Assignment4_CN_Mtech.pdf) | Wireshark capture | 5 packet capture screenshots (`shot*.png`) |
| **Ass 5** | `server.c`, `client.c` | [`ass5/ass5.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass5/ass5.pdf) | [`ass5/output.txt`](file:///c:/Users/Sundareswaran/pcn/PCN/ass5/output.txt) | Cache records & timeout execution logs |
| **Ass 6** | `server.c`, `client.c` | [`ass6/Makefile`](file:///c:/Users/Sundareswaran/pcn/PCN/ass6/Makefile) | `output_checksum.txt`, `output_crc.txt` | Intermediate sum & division logs |
| **Ass 7** | `exp1.cc`, `exp2.cc` | [`ass7/NS3_Exp1.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass7/NS3_Exp1.pdf) | `output_exp1.txt`, `output_exp2.txt` | `udp-port-based-*.pcap` Wireshark traces |
| **Ass 8** | `ass8.cc` | [`ass8/Assgn8_PCN.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass8/Assgn8_PCN.pdf) | [`ass8/output.txt`](file:///c:/Users/Sundareswaran/pcn/PCN/ass8/output.txt) | FlowMonitor packet logs |
| **Ass 9** | `ass9.cc`, `plot.py` | [`ass9/Assign9_PCN.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass9/Assign9_PCN.pdf) | [`ass9/output.txt`](file:///c:/Users/Sundareswaran/pcn/PCN/ass9/output.txt) | `cwnd_comparison.png`, `metrics_comparison.png` |
| **Ass 10** | `ass10.cc`, `run_sweep.sh` | [`ass10/Assign10_PCN.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass10/Assign10_PCN.pdf) | [`ass10/output.txt`](file:///c:/Users/Sundareswaran/pcn/PCN/ass10/output.txt) | `pdr_vs_condition.png`, `delay_vs_condition.png`, `updates_vs_condition.png` |
| **Ass 11** | `ass11.cc`, `plot.py` | [`ass11/Assign11_PCN.pdf`](file:///c:/Users/Sundareswaran/pcn/PCN/ass11/Assign11_PCN.pdf) | [`ass11/output.txt`](file:///c:/Users/Sundareswaran/pcn/PCN/ass11/output.txt) | `mobility_comparison.png` (4-panel subplots) |

---
*Maintained for UCS3511 Protocols and Computer Networks Laboratory.*
