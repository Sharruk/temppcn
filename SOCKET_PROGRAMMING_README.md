# Socket Programming Lab Guide: Execution, Analysis, and System Call Reference

This guide covers C socket programming and related network lab experiments for your Computer Networks lab examination. It details how to compile, execute, test, and troubleshoot every socket program, DNS resolver, HTTP client, and error-detection utility in this repository.

> [!NOTE]
> NS-3 simulations (Assignments 7, 8, 9, and 11) are documented separately in [`NS3_LAB_RUN_GUIDE.md`](NS3_LAB_RUN_GUIDE.md). Per instructor guidelines, ARP is excluded.

---

## 1. Directory Structure and Program Inventory

The repository organizes socket programming into six specialized directories.

| Directory | Core Purpose | Key Source Files | Port(s) Used | Protocol |
|---|---|---|---|---|
| [`tcp_udp/`](tcp_udp/) | Baseline TCP & UDP client-server communication | `tclient.c`, `tserver.c`, `uclient.c`, `userver.c`, `natuse.c` | `8080` | TCP & UDP |
| [`tcp_udp_csv/`](tcp_udp_csv/) | Student record lookup querying `students.csv` (Multithreaded TCP & UDP) | `tclient.c`, `tserver.c`, `uclient.c`, `userver.c`, `students.csv` | TCP: `8080`<br>UDP: `8081` | TCP (pthread) & UDP |
| [`dns_tcp_udp/`](dns_tcp_udp/) | Custom DNS resolver with caching & timeout simulation | `dnsclient.c`, `dnsserver.c` | `8080` | UDP |
| [`http_server_download/`](http_server_download/) | HTTP 1.1 client downloading files from local web server | `client.c`, `server_files/index.html` | `8000` (or custom) | TCP |
| [`checksum/`](checksum/) | Internet Checksum (One's complement 8-bit sum) | `cs_client.c`, `cs_server.c` | `8080` | TCP |
| [`crc/`](crc/) | Cyclic Redundancy Check ($G(x) = x^4 + x + 1$) with bit-error injection | `crc_client.c`, `crc_server.c` | `8081` | TCP |

> [!IMPORTANT]
> **Source Files vs. Precompiled Binaries:**  
> Files ending in `.c` (e.g., `tclient.c`) are C source code. Files without an extension (e.g., `tclient`) are compiled Linux ELF binaries. If you move across systems or edit code, **always recompile the binaries from source** using `gcc` or `cc`.

---

## 2. Detailed Program Walkthroughs

### A. TCP and UDP Client-Server (`tcp_udp/`)

This directory contains foundational echo-style client-server programs demonstrating fundamental socket lifecycles.

#### 1. TCP Workflow (`tserver.c` & `tclient.c`)
- **Server Lifecycle:**
  $$\text{socket()} \longrightarrow \text{bind()} \longrightarrow \text{listen()} \longrightarrow \text{accept()} \longrightarrow \text{read()} \longrightarrow \text{write()} \longrightarrow \text{close()}$$
  1. `socket(AF_INET, SOCK_STREAM, 0)`: Allocates an IPv4 stream socket descriptor.
  2. `bind()`: Binds the socket to `INADDR_ANY` on port `8080`.
  3. `listen(s, 5)`: Puts socket into passive listening mode with a backlog of 5 connections.
  4. `accept()`: Blocks until a client connects, returning a new dedicated connected socket descriptor `c`.
  5. `read(c, buf, 1023)`: Reads data sent by the client. Prints `clident: <data>` to stdout.
  6. `write(c, "hello", 5)`: Sends response string `"hello"` back to client.
  7. `close(c)` and `close(s)`: Terminates client connection and server listener.

- **Client Lifecycle:**
  $$\text{socket()} \longrightarrow \text{connect()} \longrightarrow \text{write()} \longrightarrow \text{read()} \longrightarrow \text{close()}$$
  1. `socket(AF_INET, SOCK_STREAM, 0)`: Allocates IPv4 stream socket.
  2. `inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr)`: Converts loopback address to binary format.
  3. `connect()`: Initiates the TCP 3-way handshake (`SYN` $\rightarrow$ `SYN-ACK` $\rightarrow$ `ACK`).
  4. `write(s, "hell", 4)`: Sends `"hell"` to server.
  5. `read(s, buf, 1023)`: Receives `"hello"` from server and prints `server:hello`.
  6. `close(s)`: Sends TCP `FIN` packet to tear down connection.

- **Syntax Reference Helper (`natuse.c`):**  
  `natuse.c` is a syntax cheat sheet included in the folder illustrating standard POSIX socket declarations and initialization snippets for quick reference.

#### 2. UDP Workflow (`userver.c` & `uclient.c`)
- **No Handshake / Connectionless:**
  $$\text{Server: } \text{socket()} \longrightarrow \text{bind()} \longrightarrow \text{recvfrom()} \longrightarrow \text{sendto()} \longrightarrow \text{close()}$$
  $$\text{Client: } \text{socket()} \longrightarrow \text{sendto()} \longrightarrow \text{recvfrom()} \longrightarrow \text{close()}$$
  - The client does not call `connect()`. It uses `sendto(s, "hell", 4, 0, &addr, sizeof(addr))` directly.
  - The server binds to port `8080` and calls `recvfrom()`, capturing the client's source address in `struct sockaddr_in client`.
  - The server replies directly to that client address using `sendto()`.

#### 3. Core Differences: TCP vs. UDP

| Feature | TCP (`SOCK_STREAM`) | UDP (`SOCK_DGRAM`) |
|---|---|---|
| **Connection State** | Connection-oriented (requires 3-way handshake) | Connectionless (no handshake) |
| **Reliability** | Guaranteed delivery (ACKs + kernel retransmission) | Unreliable (best effort, no native ACKs) |
| **Data Ordering** | Byte-stream with sequence numbers (in-order delivery) | Independent datagrams (may arrive out of order) |
| **Boundary Preservation** | Continuous stream (no message boundaries) | Preserves packet/message boundaries |
| **Overhead** | Higher (20-byte min header, state tracking, buffers) | Lower (8-byte header, minimal memory footprint) |

#### Exact Execution Commands:

**Terminal 1 (Server):**
```bash
cd /mnt/c/Directory/coding/College_Assignments/sem_5/Computer_Networks/exam/PCN-main/temppcn/tcp_udp

# Compile TCP Server
gcc -Wall -Wextra tserver.c -o tserver
./tserver
# Output: waiting...
```

**Terminal 2 (Client):**
```bash
cd /mnt/c/Directory/coding/College_Assignments/sem_5/Computer_Networks/exam/PCN-main/temppcn/tcp_udp

# Compile and Run TCP Client
gcc -Wall -Wextra tclient.c -o tclient
./tclient
# Output:
# got con 
# server:hello
```

*(For UDP, substitute `gcc -Wall -Wextra userver.c -o userver` and `gcc -Wall -Wextra uclient.c -o uclient`)*

---

### B. Student-Record Application (`tcp_udp_csv/`)

This directory implements a database query system querying student records stored in `students.csv`.

#### Content of `students.csv`:
```csv
193,Alice,CSE,5,9.12
195,Bob,CSE,5,8.44
197,Charlie,IT,5,8.57
199,David,ECE,5,9.03
```

#### 1. Multithreaded TCP Server (`tserver.c`)
- **Port:** `8080`
- **Concurrency:** Uses POSIX Threads (`pthread`).
  ```c
  while (1) {
      int *c = malloc(sizeof(int));
      *c = accept(s, NULL, NULL);
      pthread_t t;
      pthread_create(&t, NULL, handleClient, c);
      pthread_detach(t);
  }
  ```
  Each connection spawns a detached thread running `handleClient()`. This ensures the server never blocks while servicing multiple concurrent client queries.
- **Record Lookup:** Opens `students.csv`, reads line by line with `fgets()`, parses fields using `sscanf(line, "%19[^,],%29[^,],%19[^,],%9[^,],%9[^\n]", r, name, dept, sem, cgpa)`.
- **Match Found:** Returns formatted multi-line response:
  ```text
  Roll: 193
  Name: Alice
  Dept: CSE
  Semester: 5
  CGPA: 9.12
  ```
- **Match Not Found:** Returns `"Student Record Not Found"`.

> [!CAUTION]
> **Working Directory Dependency:**  
> `tserver.c` and `userver.c` open `"students.csv"` using a relative path (`fopen("students.csv", "r")`). You **must** execute the server from inside the `tcp_udp_csv/` directory. If started from another directory, `fopen()` will return `NULL` and report all records as not found.

#### 2. UDP Student Server (`userver.c`)
- **Port:** `8081` (Note: Uses port 8081 to avoid colliding with TCP 8080).
- **Lookup Logic:** Uses `strtok(line, ",")` to extract roll number and name.
- **Match Found:** Returns `roll num: 193 Alice`.
- **Match Not Found:** Returns literal string `"not found u fk"`.

#### Exact Execution Commands:

**Terminal 1 (Server):**
```bash
cd /mnt/c/Directory/coding/College_Assignments/sem_5/Computer_Networks/exam/PCN-main/temppcn/tcp_udp_csv

# Note the -pthread flag required for linking POSIX threads
gcc -Wall -Wextra tserver.c -o tserver -pthread
./tserver
# Output: TCP Server Started...
```

**Terminal 2 (Client):**
```bash
cd /mnt/c/Directory/coding/College_Assignments/sem_5/Computer_Networks/exam/PCN-main/temppcn/tcp_udp_csv

gcc -Wall -Wextra tclient.c -o tclient
./tclient
```
*Sample Interaction (Valid Roll Number):*
```text
enter roll: 193
Roll: 193
Name: Alice
Dept: CSE
Semester: 5
CGPA: 9.12
```
*Sample Interaction (Invalid Roll Number):*
```text
enter roll: 999
Student Record Not Found
```

---

### C. DNS Resolver over UDP with Caching and Timeouts (`dns_tcp_udp/`)

This directory models domain name resolution over UDP port `8080`.

#### Features Implemented in Source:
1. **Static DNS Mapping Table:** In `dnsserver.c`:
   - `google.com` $\rightarrow$ `142.250.72.14`
   - `youtube.com` $\rightarrow$ `142.250.72.206`
   - `github.com` $\rightarrow$ `140.82.112.4`
   - `example.com` $\rightarrow$ `93.184.216.34`
   - `ssn.edu.in` $\rightarrow$ `192.168.1.10`
2. **Client-Side Cache (`dnsclient.c`):**  
   Maintains internal cache arrays `cache_name[20][100]` and `cache_ip[20][100]`. If a queried domain is already in cache, it prints `Cache hit!` and skips the network query.
3. **Domain Validation:**  
   Verifies that the domain contains a `.` and does not start or end with `.`. Rejects invalid inputs with `Invalid domain format`.
4. **NXDOMAIN (Non-Existent Domain):**  
   If the domain is not in the server's table, the server replies with `"NXDOMAIN"`, and the client prints `Domain not found`.
5. **Timeout Simulation:**  
   When the client queries `timeout.com`, `dnsserver.c` intentionally drops the request (`if (strcmp(domain, "timeout.com") == 0) continue;`). The client socket has a 3-second receive timeout configured via `SO_RCVTIMEO`:
   ```c
   struct timeval t = {3, 0};
   setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &t, sizeof(t));
   ```
   When 3 seconds elapse with no packet, `recvfrom()` returns `-1`, and the client prints `Request timed out`.

#### Exact Execution Commands:

**Terminal 1 (DNS Server):**
```bash
cd /mnt/c/Directory/coding/College_Assignments/sem_5/Computer_Networks/exam/PCN-main/temppcn/dns_tcp_udp

gcc -Wall -Wextra dnsserver.c -o dnsserver
./dnsserver
# Output: DNS server waiting...
```

**Terminal 2 (DNS Client):**
```bash
cd /mnt/c/Directory/coding/College_Assignments/sem_5/Computer_Networks/exam/PCN-main/temppcn/dns_tcp_udp

gcc -Wall -Wextra dnsclient.c -o dnsclient
./dnsclient
```

*Sample Test Run Sequence:*
```text
Enter domain (or exit): google.com
IP address: 142.250.72.14

Enter domain (or exit): google.com
Cache hit!
IP address: 142.250.72.14

Enter domain (or exit): unknown.com
Domain not found

Enter domain (or exit): timeout.com
Request timed out

Enter domain (or exit): invalid
Invalid domain format

Enter domain (or exit): exit
```

---

### D. HTTP Webpage and File Download (`http_server_download/`)

This directory implements an HTTP 1.1 client in C that establishes a TCP connection to an HTTP server, sends an RFC-compliant HTTP `GET` request, separates HTTP response headers from the payload, saves the file to disk, and measures transfer performance.

#### Source Analysis (`client.c`):
- **URL Parser:**  
  `sscanf(url, "http://%99[^:]:%d/%199s", host, &port, path)`  
  Accepts URLs in the exact format: `http://localhost:8000/index.html` or `http://127.0.0.1:8082/index.html`.
- **Hostname Resolution:** Resolves hostname to IP using `gethostbyname()`.
- **HTTP Request Construction:**
  ```c
  snprintf(request, sizeof(request),
           "GET /%s HTTP/1.1\r\n"
           "Host: %s\r\n"
           "connection: close\r\n\r\n", path, host);
  ```
- **Header/Body Separation:** Uses `strstr(response, "\r\n\r\n")` to locate the double CRLF indicating end of HTTP headers. Payload starts 4 bytes after this delimiter.
- **Verification:** Ensures status line contains `HTTP/1.` and status code ` 200 `.
- **Disk Write:** Uses `fopen(name, "wb")` and `fwrite()` to write the downloaded body to the current directory.
- **Performance Metrics:** Uses `clock()` to measure download duration and calculates throughput in KB/s:
  $$\text{Throughput (KB/s)} = \frac{\text{body\_size}}{1024.0 \times \text{seconds}}$$

#### Exact Execution Commands:

**Terminal 1 (Start Local Python HTTP Server):**
```bash
cd /mnt/c/Directory/coding/College_Assignments/sem_5/Computer_Networks/exam/PCN-main/temppcn/http_server_download/server_files

# Run server on port 8000 (or port 8082 if 8000 is occupied by Windows)
python3 -m http.server 8000
# Output: Serving HTTP on 0.0.0.0 port 8000 ...
```

**Terminal 2 (Compile & Run C HTTP Client):**
```bash
cd /mnt/c/Directory/coding/College_Assignments/sem_5/Computer_Networks/exam/PCN-main/temppcn/http_server_download

gcc -Wall -Wextra client.c -o client
./client
```

*Interaction:*
```text
enter url: http://localhost:8000/index.html

Download complete!
Saved as: index.html
Data size: 116 bytes
Download time: 0.001 seconds
trhoughput: 113.281250 kb/s
```

*Verifying the Downloaded File:*
```bash
ls -la index.html
cat index.html
```

---

### E. Checksum Error Detection (`checksum/`)

This directory implements the classic Internet Checksum algorithm using 8-bit words and one's-complement arithmetic over TCP port `8080`.

#### Algorithm in Source:
1. **Client (`cs_client.c`):**
   - Prompts for number of bytes $n$.
   - Reads $n$ hexadecimal bytes (e.g., `12 34`).
   - Accumulates sum. When sum exceeds 255 (`0xFF`), performs end-around carry:
     $$\text{sum} = (\text{sum} \ \& \ 255) + 1$$
   - Checksum is the bitwise one's complement of the accumulated sum: `cs = ~sum;`
   - Transmits $n$, the raw data bytes, and the 1-byte checksum to the server over TCP.
2. **Server (`cs_server.c`):**
   - Receives $n$, data array, and checksum byte `cs`.
   - Initializes `int sum = cs;`.
   - Adds each received data byte to `sum` with end-around carry.
   - If no transmission errors occurred, the resulting sum must equal `255` (`0xFF`, all 1s in binary).
   - If `sum == 255`, prints `No error detected`. Otherwise, prints `Error detected`.

#### Exact Execution Commands:

**Terminal 1 (Server):**
```bash
cd /mnt/c/Directory/coding/College_Assignments/sem_5/Computer_Networks/exam/PCN-main/temppcn/checksum

gcc -Wall -Wextra cs_server.c -o cs_server
./cs_server
# Output: Waiting for client...
```

**Terminal 2 (Client):**
```bash
cd /mnt/c/Directory/coding/College_Assignments/sem_5/Computer_Networks/exam/PCN-main/temppcn/checksum

gcc -Wall -Wextra cs_client.c -o cs_client
./cs_client
```

*Sample Test Input & Expected Output:*
- Input: `2` bytes $\rightarrow$ Hex values: `12` and `34`.
- Client Terminal:
  ```text
  Number of bytes: 2
  Enter 2 hexadecimal values (00-FF):
  12 34
  Checksum = B9
  Data sent successfully
  ```
- Server Terminal:
  ```text
  Word 1 = 12, Sum = CB
  Word 2 = 34, Sum = FF
  Checksum = B9
  No error detected
  ```

---

### F. Cyclic Redundancy Check (CRC) with Bit-Error Injection (`crc/`)

This directory implements binary CRC encoding and decoding with the generator polynomial $G(x) = x^4 + x + 1$ (binary `"10011"`, degree 4) over TCP port `8081`.

#### Algorithm in Source:
1. **Client (`crc_client.c`):**
   - Reads ASCII text (e.g., `"A"`).
   - Converts each character into an 8-bit binary string (`"A"` $\rightarrow$ `"01000001"`).
   - Appends 4 zeros (`"0000"`, since degree of $G(x)$ is 4).
   - Performs modulo-2 polynomial division (XOR subtraction where leading bit is 1).
   - Appends the resulting 4-bit remainder (CRC) to the original binary string to produce the transmitted `frame`.
   - Sends the frame length and frame to the server over TCP.
2. **Server (`crc_server.c`):**
   - Receives frame from client.
   - **Deliberate Error Injection:** Asks the user:
     `Introduce error? (1=yes, 0=no):`
     If `1`, prompts for bit position (`0` to $n-1$) and flips that bit (`'0'` $\leftrightarrow$ `'1'`).
   - Runs modulo-2 division on the frame using the identical generator `"10011"`.
   - Checks the final 4 remainder bits:
     - If all 4 bits are `'0'`, prints `No error detected`.
     - If any bit is non-zero, prints `Error detected`.

#### Exact Execution Commands:

**Terminal 1 (Server):**
```bash
cd /mnt/c/Directory/coding/College_Assignments/sem_5/Computer_Networks/exam/PCN-main/temppcn/crc

gcc -Wall -Wextra crc_server.c -o crc_server
./crc_server
# Output: Waiting for client...
```

**Terminal 2 (Client):**
```bash
cd /mnt/c/Directory/coding/College_Assignments/sem_5/Computer_Networks/exam/PCN-main/temppcn/crc

gcc -Wall -Wextra crc_client.c -o crc_client
./crc_client
```

*Sample Test 1 (No Error Injected):*
- Client enters text: `A`
  - Binary: `01000001`
  - CRC: `0100`
  - Frame sent: `010000010100`
- Server enters `0` (No error):
  ```text
  Received frame: 010000010100
  Introduce error? (1=yes, 0=no): 0
  Checking frame: 010000010100
  No error detected
  ```

*Sample Test 2 (Error Injected at bit index 2):*
- Server enters `1`, then index `2`:
  ```text
  Received frame: 010000010100
  Introduce error? (1=yes, 0=no): 1
  Enter bit position (0 to 11): 2
  Checking frame: 011000010100
  Error detected
  ```

---

## 3. Timers and Retransmissions in Socket Programming

### Transport Layer (TCP) vs. Application Layer (UDP)

During your lab examination or viva, distinguish where timers and retransmissions live:

```
+-----------------------------------------------------------------------------------+
| TCP (Kernel Space - Automatic)                                                    |
| - Retransmission Timeout (RTO) calculated dynamically via Jacobson's algorithm.  |
| - Fast Retransmit triggered upon receiving 3 duplicate ACKs.                      |
| - Persist Timer prevents deadlocks from Zero Window probes.                       |
| - TIME_WAIT (2MSL) timer ensures lingering packets dissipate before socket reuse. |
| -> APPLICATION CODE DOES NOT IMPLEMENT RETRANSMISSIONS FOR TCP.                   |
+-----------------------------------------------------------------------------------+
| UDP (User / Application Space - Manual)                                           |
| - UDP stack does NOT retransmit lost packets or send acknowledgements.            |
| - Application must configure timeouts (e.g. SO_RCVTIMEO) and retry loops manually.|
+-----------------------------------------------------------------------------------+
```

### Status of Retransmission Programs in This Repository

> [!IMPORTANT]
> **Repository Implementation Note:**  
> This repository **does not contain a dedicated stop-and-wait ARQ or sliding-window retransmission C program**.  
> However:
> 1. Socket timeout handling **is implemented** in [`dns_tcp_udp/dnsclient.c`](dns_tcp_udp/dnsclient.c) using `setsockopt(..., SO_RCVTIMEO, ...)`.
> 2. Packet loss / server unresponsive simulation **is implemented** in [`dns_tcp_udp/dnsserver.c`](dns_tcp_udp/dnsserver.c) via `timeout.com`.
> 3. Transport retransmission and congestion control dynamics **are analyzed in NS-3** in [`A9-TCP-Congestion-Control/`](A9-TCP-Congestion-Control/).

### How Socket Timeouts Are Implemented in C

To prevent a socket call (`recv`, `recvfrom`) from blocking indefinitely, configure `SO_RCVTIMEO`:

```c
#include <sys/time.h>
#include <sys/socket.h>

struct timeval timeout;
timeout.tv_sec = 3;  // 3-second timeout
timeout.tv_usec = 0;

// Set receive timeout
setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

int n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&addr, &len);
if (n < 0) {
    // errno will be EAGAIN or EWOULDBLOCK
    perror("Socket timed out, ready for application retransmission");
}
```

---

## 4. Comprehensive Socket System Calls Reference Table

The following system calls and helper functions are defined in `<sys/socket.h>`, `<netinet/in.h>`, `<arpa/inet.h>`, `<unistd.h>`, and `<netdb.h>` (Assignment 1 syllabus):

| Function | Syntax Signature | Purpose | Where Used in Lab / Flow |
|---|---|---|---|
| `socket()` | `int socket(int domain, int type, int protocol);` | Creates an unbound socket endpoint and returns a file descriptor. | Start of every client and server (`AF_INET`, `SOCK_STREAM` or `SOCK_DGRAM`). |
| `bind()` | `int bind(int fd, const struct sockaddr *addr, socklen_t len);` | Assigns a local IP address and port number to a socket. | Server setup (`tserver.c`, `userver.c`, `dnsserver.c`). |
| `listen()` | `int listen(int fd, int backlog);` | Marks a TCP socket as passive, ready to accept incoming connections. | TCP servers only, before `accept()`. |
| `connect()` | `int connect(int fd, const struct sockaddr *addr, socklen_t len);` | Initiates a TCP connection to a remote listening server. | TCP clients (`tclient.c`, `client.c`, `cs_client.c`, `crc_client.c`). |
| `accept()` | `int accept(int fd, struct sockaddr *addr, socklen_t *len);` | Extracts first connection request from queue; returns new socket fd. | TCP servers (`tserver.c`, `cs_server.c`, `crc_server.c`). |
| `close()` | `int close(int fd);` | Closes socket descriptor and terminates/releases network resources. | End of every socket program to prevent descriptor leaks. |
| `bzero()` | `void bzero(void *s, size_t n);` | Erases data in buffer by writing zeros (modern alternative: `memset`). | Initializing `sockaddr_in` structures. |
| `htons()` | `uint16_t htons(uint16_t hostshort);` | Host-To-Network-Short: Converts 16-bit integer from host to network byte order. | Assigning port numbers (`addr.sin_port = htons(8080)`). |
| `htonl()` | `uint32_t htonl(uint32_t hostlong);` | Host-To-Network-Long: Converts 32-bit integer from host to network byte order. | Assigning IPv4 addresses (`addr.sin_addr.s_addr = htonl(INADDR_ANY)`). |
| `ntohs()` | `uint16_t ntohs(uint16_t netshort);` | Network-To-Host-Short: Converts 16-bit integer from network to host byte order. | Extracting port numbers received from network. |
| `ntohl()` | `uint32_t ntohl(uint32_t netlong);` | Network-To-Host-Long: Converts 32-bit integer from network to host byte order. | Extracting IPv4 addresses received from network. |
| `read()` | `ssize_t read(int fd, void *buf, size_t count);` | Standard POSIX read from a stream descriptor. | TCP data reception (`tserver.c`, `tclient.c`). |
| `write()` | `ssize_t write(int fd, const void *buf, size_t count);` | Standard POSIX write to a stream descriptor. | TCP data transmission (`tserver.c`, `tclient.c`). |
| `send()` | `ssize_t send(int fd, const void *buf, size_t len, int flags);` | Transmits data on a connected stream or datagram socket. | TCP transmission (`client.c`, `cs_client.c`, `crc_client.c`). |
| `recv()` | `ssize_t recv(int fd, void *buf, size_t len, int flags);` | Receives data from a connected socket. | TCP reception (`client.c`, `cs_server.c` with `MSG_WAITALL`). |
| `sendto()` | `ssize_t sendto(int fd, const void *buf, size_t len, int flags, const struct sockaddr *dest, socklen_t addrlen);` | Sends a datagram to a specific destination address. | UDP transmission (`uclient.c`, `userver.c`, `dnsclient.c`). |
| `recvfrom()` | `ssize_t recvfrom(int fd, void *buf, size_t len, int flags, struct sockaddr *src, socklen_t *addrlen);` | Receives a datagram and captures sender's source address. | UDP reception (`userver.c`, `uclient.c`, `dnsserver.c`). |
| `select()` | `int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);` | Synchronous I/O multiplexing; monitors multiple socket descriptors for events. | Handling multiple clients or timeout detection without threads. |
| `setsockopt()` | `int setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen);` | Manipulates socket options (`SO_REUSEADDR`, `SO_RCVTIMEO`). | Configuring 3s timeout in `dnsclient.c`. |
| `fcntl()` | `int fcntl(int fd, int cmd, ... /* arg */ );` | Performs file control operations (e.g., setting non-blocking mode `O_NONBLOCK`). | Advanced asynchronous socket configuration. |
| `getpeername()` | `int getpeername(int fd, struct sockaddr *addr, socklen_t *len);` | Retrieves the IP address and port of the peer connected to socket `fd`. | Identifying connected remote client IP. |
| `gethostname()` | `int gethostname(char *name, size_t len);` | Returns standard host name of current local machine. | Network diagnostics and system identification. |
| `gethostbyname()` | `struct hostent *gethostbyname(const char *name);` | Resolves domain/hostname (e.g., `"localhost"`) to IPv4 network address. | Used in `http_server_download/client.c` for URL resolution. |
| `gethostbyaddr()` | `struct hostent *gethostbyaddr(const void *addr, socklen_t len, int type);` | Reverse DNS: Resolves IP address to hostname. | Reverse DNS diagnostics. |

---

### Understanding Byte Ordering (Endianness)

Network communication protocols dictate that integers transmitted over the wire must follow **Big-Endian** order (most significant byte transmitted first), known as **Network Byte Order**.

Most personal computers (x86_64 architecture) represent integers in memory in **Little-Endian** order (least significant byte first), known as **Host Byte Order**.

```
Host Order (x86_64 Little-Endian): Port 8080 = 0x1F90 -> [0x90][0x1F] in memory
                           |
                     htons(8080)
                           v
Network Order (Big-Endian):       Port 8080 = 0x1F90 -> [0x1F][0x90] on wire
```

If you forget `htons(8080)` and write `addr.sin_port = 8080`, your machine puts `0x901F` (port 36895) on the wire, causing `Connection refused` because the server is listening on a completely different port!

---

## 5. Performance Metrics Reference Table

| Metric | Definition | Standard Formula | Implemented In Repository? |
|---|---|---|---|
| **Throughput** | Rate of successful data delivery over communication channel | $\text{Throughput} = \frac{\text{Delivered Bits}}{\text{Total Elapsed Time}}$ | **Yes**, in [`http_server_download/client.c`](http_server_download/client.c) (`body_size / 1024.0 / seconds` in KB/s) and NS-3 FlowMonitor. |
| **Latency** | Delay taken for a packet to travel from source to destination | $\text{Latency} = t_{\text{receive}} - t_{\text{transmit}}$ | **Yes**, measured in NS-3 Assignment 7 and 11. In C programs, approximated via single-trip timing. |
| **Round-Trip Time (RTT)** | Total time from sending request until receiving corresponding response | $\text{RTT} = t_{\text{response\_received}} - t_{\text{request\_sent}}$ | **Theoretical / Ping**: Measured during request-response loops or ICMP ping. |
| **Packet Delivery Ratio (PDR)** | Percentage of successfully delivered packets over total sent | $\text{PDR} = \left(\frac{\text{Packets Received}}{\text{Packets Sent}}\right) \times 100\%$ | **Yes**, in NS-3 FlowMonitor simulations (Assignments 7, 8, 11). |
| **Packet Loss Percentage** | Percentage of sent packets that failed to reach destination | $\text{Loss \%} = \left(\frac{\text{Packets Sent} - \text{Packets Received}}{\text{Packets Sent}}\right) \times 100\%$ | **Yes**, in NS-3 Assignment 8 (bottleneck drops) and Assignment 11. |
| **Average Delay** | Mean end-to-end transmission delay across all successfully received packets | $\text{Avg Delay} = \frac{\sum_{i=1}^{N} \text{Delay}_i}{N}$ | **Yes**, in NS-3 FlowMonitor and wireless technology comparisons. |

> [!NOTE]
> **Latency vs. RTT:** Latency is one-way transit time ($A \rightarrow B$). RTT is two-way elapsed time ($A \rightarrow B \rightarrow A$), including network traversal in both directions plus remote server processing delay.

---

## 6. How to Run Socket Programs in the Lab (Step-by-Step)

Follow this 11-step procedure for every socket test during your exam:

1. **Open Linux Terminal 1:** This terminal will host the server.
2. **Navigate to the target directory:**
   ```bash
   cd /mnt/c/Directory/coding/College_Assignments/sem_5/Computer_Networks/exam/PCN-main/temppcn/<target_dir>
   ```
3. **Inspect the files:** Use `ls -l` to verify source files and inputs exist.
4. **Compile the server source file:**
   ```bash
   gcc -Wall -Wextra <server_source>.c -o <server_bin>
   # Add -pthread if the server uses POSIX threads (e.g., tcp_udp_csv/tserver.c)
   ```
5. **Start the server in Terminal 1:**
   ```bash
   ./<server_bin>
   ```
6. **Open Linux Terminal 2:** This terminal will execute the client.
7. **Navigate to the exact same directory in Terminal 2:**
   ```bash
   cd /mnt/c/Directory/coding/College_Assignments/sem_5/Computer_Networks/exam/PCN-main/temppcn/<target_dir>
   ```
8. **Compile the client source file:**
   ```bash
   gcc -Wall -Wextra <client_source>.c -o <client_bin>
   ```
9. **Run the client:**
   ```bash
   ./<client_bin>
   ```
10. **Enter input and observe outputs:** Verify the outputs printed in both Terminal 1 and Terminal 2.
11. **Clean Shutdown:** Stop the server in Terminal 1 by pressing `Ctrl + C`.

---

## 7. Common Errors and Troubleshooting Table

| Error / Symptom | Root Cause | Immediate Diagnostic & Solution |
|---|---|---|
| `Address already in use` (`EADDRINUSE`) | Port is held by a previous server instance or lingering `TIME_WAIT` connection. | 1. Find and kill the process holding the port:<br>`fuser -k 8080/tcp` or `killall tserver`<br>2. In C code, enable address reuse before `bind()`:<br>`int opt=1; setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));` |
| `Connection refused` (`ECONNREFUSED`) | 1. Server is not running.<br>2. Client is targeting wrong IP or wrong port. | 1. Ensure server is started in Terminal 1 *before* running client.<br>2. Verify port in client code matches server (`8080` vs `8081`). |
| `Connection timed out` (`ETIMEDOUT`) | Target IP address is unreachable or blocked by firewall. | For local testing, ensure client targets `"127.0.0.1"` or `"localhost"`. |
| Client started before server | In TCP, client `connect()` fails immediately with `Connection refused`. In UDP, client sends packet into void. | Always launch the server first, confirm `waiting...`, then launch client. |
| Wrong port or IP address | Packet routed to wrong service or rejected. | Check `addr.sin_port = htons(PORT)` in both client and server source files. |
| `No such file or directory` | 1. Binary has not been compiled.<br>2. Input file (`students.csv`) missing from current working directory. | 1. Recompile with `gcc <file>.c -o <bin>`.<br>2. Check `pwd` and verify `students.csv` is in the folder. |
| CSV lookup returns "Not Found" for valid roll | Server was launched from a different folder than `tcp_udp_csv/`. | `cd /path/to/tcp_udp_csv` and run `./tserver` directly from that directory so relative `fopen()` succeeds. |
| `undefined reference to 'pthread_create'` | Compiler did not link the POSIX threads library. | Add `-pthread` flag during compilation:<br>`gcc tserver.c -o tserver -pthread` |
| `recv()` returns 0 | The remote peer performed an orderly shutdown and closed the socket (`FIN`). | The connection has ended; close the local socket descriptor and exit cleanly. |
| `recv()` returns -1 | Socket error or timeout occurred. | Check `errno` using `perror("recv")`. If `EAGAIN` or `EWOULDBLOCK`, the socket timed out. |
| UDP client receives no response | UDP datagram dropped, or server crashed/not bound to expected port. | Check server is running. In UDP, no error is returned to client if server is down unless ICMP port unreachable is handled. |
| HTTP download returns `404 Not Found` | Python web server was started in the wrong folder, or filename is mistyped. | Start Python server inside `server_files/` (`cd server_files && python3 -m http.server 8000`) and request `http://localhost:8000/index.html`. |
| Checksum/CRC reports corrupted frame | Bit flip occurred during transmission, or incorrect input formatting. | Check input formatting. In `crc_server`, verify whether error injection option (`1`) was selected. |

---

## 8. Question Papers and Cheat Sheet Links

The question PDFs corresponding to each laboratory assignment are available in the repository's [`questions/`](questions/) directory:

- [Assignment 01 — Network Diagnostics and Socket System Calls](questions/assignment-01-network-diagnostics-and-socket-calls.pdf)
- [Assignment 02 — TCP and UDP Client-Server Programming](questions/assignment-02-tcp-udp-client-server.pdf)
- [Assignment 03 — HTTP Client Using TCP Sockets](questions/assignment-03-http-client-tcp-sockets.pdf)
- [Assignment 04 — HTTP Packet Analysis](questions/assignment-04-http-packet-analysis.pdf)  
  *(Note: Assignment 4 focuses on analyzing captured HTTP traffic headers. In a full diagnostic lab this uses packet capture tools such as Wireshark or `tcpdump`, but no external packet capture packages are required to run the socket programs in this repo.)*
- [Assignment 05 — DNS Implementation Using UDP Sockets](questions/assignment-05-dns-udp-socket-programming.pdf)
- [Assignment 06 — Error Detection: Checksum and CRC](questions/assignment-06-checksum-and-crc.pdf)

### Additional Reference Documents

- [Socket Programming Cheat Sheet PDF](socket_cheat.pdf): Root-level reference card covering socket data structures and C system call syntax.
- [NS-3 Simulation Lab Run Guide](NS3_LAB_RUN_GUIDE.md): Dedicated execution and troubleshooting guide for discrete-event NS-3 network simulations.
