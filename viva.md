# P2P vs CDN for Video Streaming – 12 Marks

## 1. Introduction

Both **Peer-to-Peer (P2P) networks** and **Content Distribution Networks (CDNs)** are used to deliver video content to a large number of users.

When the number of viewers suddenly **doubles**, the network must handle increased traffic without causing buffering or delays.

The main difference is:

- **P2P:** Viewers can also act as **uploaders** and share video data with other viewers.
- **CDN:** Video is distributed through **multiple geographically distributed servers** located close to users.

---

# 2. P2P Network

In a P2P network, users download video data from other peers as well as the original server.

### Normal Traffic

```text
                    Original Server
                         |
                         v
                    Peer 1
                   /      \
                  v        v
              Peer 2     Peer 3
                /  \       |
               v    v      v
            Peer 4 Peer 5 Peer 6
```

As more viewers join, they can also contribute upload bandwidth.

### When Viewers Double

Suppose there are initially **100 viewers** and suddenly the number becomes **200**.

```text
             Original Server
                    |
          ---------------------
          |        |          |
        Peer     Peer       Peer
       /   \      | \        / \
      v     v     v  v      v   v
   New   New    New New   New  New
  Peers  Peers  Peers      Peers
```

The new peers can obtain pieces of the video from existing peers.

### Advantages During a Surge

- Upload capacity increases as more peers join.
- Traffic is distributed among many users.
- The original server does not need to send the complete video to every viewer.
- Can scale well for very large numbers of viewers.

### Disadvantages

- Performance depends on **peer availability and upload bandwidth**.
- Peers may leave suddenly.
- Upload speeds may be different for different users.
- Maintaining peer connections can introduce complexity.
- Quality can become unpredictable during sudden changes in peer availability.

---

# 3. CDN

A CDN consists of multiple geographically distributed servers called **edge servers**.

Instead of every viewer contacting the origin server, users are directed to a nearby CDN server.

### Normal Traffic

```text
                    Origin Server
                         |
              ---------------------
              |         |          |
              v         v          v
           CDN 1      CDN 2      CDN 3
             |          |          |
           Users      Users      Users
```

### When Viewers Double

When the number of viewers suddenly doubles:

```text
                    Origin Server
                         |
              ---------------------
              |         |          |
              v         v          v
           CDN 1      CDN 2      CDN 3
          / | \       / | \       / | \
         v  v  v     v  v  v     v  v  v
       Users        Users        Users
       + New        + New        + New
       Users        Users        Users
```

The additional viewers are distributed across different CDN edge servers.

If the video is already cached at the edge servers, the CDN can serve the additional viewers **without repeatedly contacting the origin server**.

---

# 4. Comparison During Sudden Increase

| Feature | P2P | CDN |
|---|---|---|
| Main source | Other peers | CDN edge servers |
| Traffic distribution | Among peers | Among CDN servers |
| Effect of more users | More upload capacity can become available | Load distributed across edge servers |
| Server load | Can be reduced significantly | Reduced using caching and distribution |
| Reliability | Depends on peer availability | Generally more predictable |
| Performance | Variable | More consistent |
| Scalability | High | Very high |
| Control over quality | Lower | Higher |
| Infrastructure | Less centralized | Requires distributed infrastructure |

---

# 5. Which Handles the Surge More Efficiently?

### **CDN is generally more efficient and reliable for a sudden doubling of viewers.**

The main reasons are:

### 1. Distributed servers

A CDN has multiple edge servers, so the additional viewers can be distributed across different locations.

### 2. Caching

If the video is already cached at the edge server, the CDN does not need to retrieve it repeatedly from the origin server.

### 3. Reduced latency

Users are connected to a geographically closer CDN server, reducing network delay.

### 4. Better traffic management

CDNs use mechanisms such as **load balancing** to distribute users among available servers.

### 5. Predictable performance

Unlike P2P, CDN performance does not depend heavily on whether individual users remain online or have sufficient upload bandwidth.

---

# 6. Overall Traffic Behavior

### P2P

```text
More Viewers
     ↓
More Peers
     ↓
More Possible Upload Sources
     ↓
Traffic Distributed Among Peers
     ↓
Reduced Origin Server Load
```

### CDN

```text
More Viewers
     ↓
Load Distributed Across Edge Servers
     ↓
Cached Video Served Locally
     ↓
Less Traffic to Origin Server
     ↓
Faster and More Reliable Streaming
```

---

# 7. Conclusion

Both P2P and CDN networks can handle an increase in video-streaming users by **distributing network traffic**.

In **P2P**, the additional viewers can also become sources of video data, allowing the system to scale with the number of peers. However, its performance depends on peer availability and upload capacity.

In a **CDN**, traffic is distributed among geographically distributed edge servers, and cached content can be delivered without repeatedly contacting the origin server. Therefore, **CDNs are generally better suited for sudden surges in viewers because they provide more predictable performance, lower latency, effective load balancing, and reduced congestion at the origin server.**




# Reliable Data Transfer Protocol for Bit Errors, Corrupted ACKs and Packet Loss – 12 Marks

## 1. Introduction

A reliable data transfer (RDT) protocol ensures that data sent from a **sender** reaches the **receiver** correctly and in the proper order, even when the underlying channel is unreliable.

The protocol can be developed progressively:

1. **Bit errors → checksum and retransmission**
2. **Corrupted ACKs → sequence numbers**
3. **Packet loss → timeout and retransmission**

---

## 2. Handling Bit Errors

First, assume that packets may be **corrupted**, but are not lost.

The sender adds a **checksum** to each packet. The receiver checks the checksum.

- If correct → receiver sends **ACK**.
- If corrupted → receiver sends **NAK**.
- On receiving NAK, sender retransmits the packet.

### Sequence Diagram

```text
Sender                         Receiver
  |                               |
  |-------- Data + Checksum ----->|
  |                               |
  |     Packet corrupted          |
  |                               |
  |<------------- NAK ------------|
  |                               |
  |-------- Retransmit ---------->|
  |                               |
  |<------------- ACK ------------|
  |                               |
```

Thus, corrupted data is detected and retransmitted.

---

# 3. Handling Corrupted Acknowledgments

Now assume that **ACK/NAK messages themselves can also be corrupted**.

If the sender receives a corrupted ACK/NAK, it cannot know whether the receiver correctly received the packet.

To solve this, **sequence numbers** are added to packets.

For example, packets use sequence numbers **0 and 1** alternately.

The receiver uses the sequence number to identify duplicate packets.

### Sequence Diagram

```text
Sender                         Receiver
  |                               |
  |------ Packet (Seq = 0) ------>|
  |                               |
  |<--------- ACK (0) ------------|
  |                               |
  |     ACK becomes corrupted     |
  |                               |
  |<------ Corrupted ACK ---------|
  |                               |
  |------ Packet (Seq = 0) ------>|
  |                               |
  |  Duplicate packet detected    |
  |                               |
  |<--------- ACK (0) ------------|
  |                               |
  |------ Packet (Seq = 1) ------>|
  |                               |
  |<--------- ACK (1) ------------|
```

The receiver does **not deliver the duplicate packet again**. It simply sends the ACK again.

Thus, sequence numbers prevent **duplicate delivery**.

---

# 4. Handling Packet Loss

Now assume that packets or acknowledgments can be **lost**.

If the sender waits forever for an ACK, communication may stop.

Therefore, the sender uses a **timer**.

- Sender transmits a packet and starts a timer.
- If ACK arrives → send the next packet.
- If ACK is lost or packet is lost → timer expires.
- Sender retransmits the packet.

### Sequence Diagram

```text
Sender                         Receiver
  |                               |
  |------ Packet (Seq = 0) ------>|
  |                               |
  |<--------- ACK (0) ------------|
  |                               |
  |------ Packet (Seq = 1) ------>|
  |          X                    |
  |       Packet lost             |
  |                               |
  |       Timer expires           |
  |                               |
  |------ Retransmit Seq = 1 ---->|
  |                               |
  |<--------- ACK (1) ------------|
```

If the ACK is lost:

```text
Sender                         Receiver
  |                               |
  |------ Packet (Seq = 0) ------>|
  |                               |
  |<--------- ACK (0) ---X        |
  |          ACK lost             |
  |                               |
  |       Timer expires           |
  |                               |
  |------ Retransmit Seq = 0 ---->|
  |                               |
  |   Duplicate detected          |
  |                               |
  |<--------- ACK (0) ------------|
```

The receiver discards the duplicate and sends the ACK again.

---

# 5. Final Reliable Data Transfer Protocol

The complete protocol therefore uses:

- **Checksum** → detects corrupted packets.
- **ACK/NAK** → informs the sender about successful or unsuccessful reception.
- **Sequence numbers** → detect duplicate packets caused by retransmission.
- **Timer** → detects packet or ACK loss.
- **Retransmission** → recovers lost or corrupted packets.

### Overall Operation

```text
        Sender                         Receiver
          |                               |
          |--- Data + Checksum + Seq ---->|
          |                               |
          |       Check checksum          |
          |                               |
          |<------------ ACK -------------|
          |                               |
          |       ACK corrupted/lost?      |
          |                               |
          |--- Retransmit if required ---->|
          |                               |
          |       Duplicate?              |
          |       Discard + ACK           |
          |                               |
          |<------------ ACK -------------|
```

---

## 6. Conclusion

The reliable data transfer protocol is progressively enhanced to handle different channel problems. **Checksums and ACK/NAK handle bit errors, sequence numbers handle corrupted acknowledgments and duplicate packets, and timers with retransmission handle packet loss.** Together, these mechanisms provide reliable, ordered, and duplicate-free data delivery over an unreliable channel.




Yes. **This answer is suitable for the 12-mark question you asked.** In fact, it is **better aligned with the wording of the question** than the previous answer because the question specifically asks you to *“apply the principles of reliable data transfer”* and explain how the protocol **progressively handles** the different channel problems.

Your answer correctly covers:

- **RDT 1.0** → reliable channel
- **RDT 2.0** → bit errors using checksum + ACK/NAK
- **RDT 2.1** → corrupted ACK/NAK using sequence numbers
- **RDT 2.2** → ACK-only approach
- **RDT 3.0** → packet loss using timer + retransmission
- **Sequence diagrams** for the important stages
- **Summary table**
- **Conclusion**

### One important correction

In the **RDT 2.2 diagram**, this part:

```text
Sender                         Receiver
  |                               |
  |------ Data (seq 1) ---------->|
  |              X                |
  |          Corrupted            |
  |                               |
  |<--------- ACK 0 --------------|
```

is slightly misleading. In RDT 2.2, if the **data packet is corrupted**, the receiver sends a **duplicate ACK for the last correctly received packet**. So the idea is correct, but write it as:

```text
Sender                         Receiver
  |                               |
  |------ Data (seq 1) ---------->|
  |          Corrupted            |
  |                               |
  |<--------- ACK 0 --------------|
  |                               |
  |------ Data (seq 1) ---------->|
  |                               |
  |<--------- ACK 1 --------------|
```

Also, for a **12-mark answer**, you don't necessarily need to explain RDT 1.0 in much detail. The examiner will mainly look for the **progressive handling of errors → corrupted ACKs → loss**.

### Final verdict

**Yes — you can write this in the exam for 12 marks.** I would rate it around **11–12/12** if the diagrams are drawn clearly and the terminology is correct.

The key line to remember is:

> **RDT 2.0 → checksum + ACK/NAK → RDT 2.1/2.2 → sequence numbers → RDT 3.0 → timer + retransmission.**

That progression directly answers the question.




# Computer Networks CAT2 — Complete Practical Viva Preparation

Your practical viva is today, so prepare to explain concepts aloud, describe how protocols work, draw a diagram when asked, and answer follow-up questions. Knowing a definition alone may not be enough; the examiner may immediately ask, “How does it work?”, “Why is it needed?”, or “What is the difference between these two?”

We’ll cover your CAT2 syllabus in Units IV, V and III, with extra attention to CSMA because it is one of the topics you were specifically asked about.

Your confirmed Unit V scope: wireless networks, CDMA, Wi-Fi/802.11, cellular networks, handover, Mobile IP and SDN architecture/components. Cloud Computing is excluded.

# 1. CSMA — learn this first

## What is CSMA?

CSMA stands for Carrier Sense Multiple Access. It is a medium-access control (MAC) protocol that allows multiple devices to share the same communication channel by listening to the channel before transmitting.

The basic principle is:

> Listen before you transmit.

Imagine five computers connected to a shared communication medium. If every computer transmits whenever it has data, two computers might transmit simultaneously, causing a collision.

CSMA tries to reduce this problem by checking whether the channel is already being used.

## How does CSMA work?

Sequence diagram — suitable for drawing in your viva

```
Device A                    Shared Channel
    |                             |
    |---- Sense channel --------->|
    |                             |
    |<-------- Idle --------------|
    |                             |
    |---- Transmit DATA --------->|
    |                             |
    |        Transmission         |
    |           ends              |
    |                             |
```

If the channel is busy, the device defers transmission and waits until the channel becomes available.

The general procedure is:

1. A device has a frame to transmit.
2. It senses the channel.
3. If the channel is idle, it transmits.
4. If the channel is busy, it waits or backs off, depending on the protocol.
5. Transmission completes, or a collision occurs and the relevant protocol handles recovery.

## If CSMA listens first, why can collisions still happen?

This is a very common viva question.

Answer: Because of propagation delay.

Suppose Device A starts transmitting. Its signal takes some time to reach Device B. During this interval, B may still sense the channel as idle and begin transmitting too. Both transmissions then interfere.

```
Device A                         Device B
   |                                |
   |---- DATA starts -------------->|
   |                                |
   |                    Channel still
   |                    appears idle
   |                                |
   |<----------- DATA starts -------|
   |                                |
   |          COLLISION             |
```

CSMA reduces collisions, but ordinary CSMA cannot eliminate them completely.

## What are the advantages of CSMA?

- It reduces collisions compared with transmitting without sensing the channel.
- It is distributed: devices do not need one central controller to decide every transmission.
- A device can use the channel when it is available.
- It is relatively simple.

## What are the limitations of CSMA?

- Collisions can still occur because of propagation delay.
- Time is wasted when collided frames must be retransmitted.
- Performance can decrease when many devices compete for the channel.
- Wireless environments introduce additional problems, including hidden terminals and fading.

Your Unit IV slides explain carrier sensing and the possibility of collisions, while your Unit V slides explain why wireless LANs require collision avoidance.&#x20;

Data_Link_Layer(1).pdf



## CSMA vs CSMA/CD vs CSMA/CA

This distinction is particularly important for your viva.

## CSMA

Carrier Sense Multiple Access

Listen before transmitting. Collisions can still happen because of propagation delay.

## CSMA/CD

Collision Detection

Detects a collision during transmission, aborts the frame, and uses backoff before retrying. Associated with traditional shared Ethernet.

## CSMA/CA

Collision Avoidance

Tries to avoid collisions before transmitting. Used by IEEE 802.11 wireless LANs.

Your professor’s material explicitly associates CSMA/CD with Ethernet and CSMA/CA with 802.11 Wi-Fi.&#x20;

Data_Link_Layer.pdf



### A. CSMA/CD — how does it work?

CSMA/CD means Carrier Sense Multiple Access with Collision Detection.

```
Ethernet Device                Shared Medium
       |                             |
       |---- Sense channel --------->|
       |                             |
       |---- Transmit frame -------->|
       |                             |
       |   Collision detected        |
       |---- Abort transmission ---->|
       |---- Send jam signal ------->|
       |                             |
       |---- Random backoff -------->|
       |---- Retry transmission ---->|
```

The steps are:

1. The network interface card (NIC) receives a datagram from the network layer and creates an Ethernet frame.
2. It senses the channel.
3. If the channel is idle, it transmits; otherwise, it waits.
4. If a collision is detected, it aborts transmission and sends a jam signal.
5. It uses binary exponential backoff: the device chooses a random waiting interval, with the range increasing after repeated collisions.
6. It senses the channel again and retries.

Your slides describe this Ethernet procedure, including aborting a collided frame and using binary exponential backoff.&#x20;

Data_Link_Layer(1).pdf



Why not use CSMA/CD in Wi-Fi?

A wireless device has difficulty detecting another signal while it is transmitting because its own transmission may overpower the received signal. Hidden terminals and fading make collision detection more difficult. Wi-Fi therefore uses CSMA/CA.

### B. CSMA/CA — how does it work?

CSMA/CA means Carrier Sense Multiple Access with Collision Avoidance. It is used in IEEE 802.11 wireless LANs.

```
Wireless Sender                  Access Point
      |                               |
      |---- Sense channel             |
      |                               |
      |  If busy: wait/backoff        |
      |  If idle: wait DIFS           |
      |                               |
      |---- Random backoff            |
      |                               |
      |-------- DATA frame ---------->|
      |                               |
      |<------------- ACK ------------|
      |                               |
```

The steps in your Unit V material are:

1. The sender senses the channel.
2. If the channel remains idle for DIFS (Distributed Inter-Frame Space), it can proceed to transmit.
3. If the channel is busy, it starts a random backoff timer.
4. The timer counts down while the channel is idle; the sender transmits when the timer expires.
5. If the frame is received correctly, the receiver sends an ACK after SIFS (Short Inter-Frame Space).
6. If no ACK arrives, the sender increases its random backoff interval and retries.&#x20;

   Unit-5 Wireless and Mobile Networks(1).pdf



### What are DIFS and SIFS?

- DIFS: Distributed Inter-Frame Space. The sender waits for this period of channel idleness before starting a normal contention-based transmission.
- SIFS: Short Inter-Frame Space. A shorter waiting period used before immediate responses such as ACKs.

Why is SIFS shorter than DIFS? It allows the receiver’s ACK to be sent before another device starts a new contention-based transmission.

### What is the hidden-terminal problem?

Suppose A and C can both communicate with B, but A and C cannot hear each other.

```
Device A  ------>  Device B  <------  Device C
   \                                    /
    \_____ A and C cannot hear _______/
```

A and C might both sense their local channels as idle and transmit to B at the same time. The transmissions then collide at B.

How can RTS/CTS help?

```
Sender A                Access Point               Other Nodes
   |                         |                          |
   |--------- RTS ---------->|                          |
   |<-------- CTS -----------|------------------------->|
   |                         |                          |
   |--------- DATA --------->|             Defer transmission
   |<--------- ACK ----------|                          |
```

RTS means Request to Send, and CTS means Clear to Send. The sender asks to reserve the channel using RTS; the access point responds with CTS, which other stations can hear. Other stations defer, allowing the sender to transmit its data. RTS/CTS reduces collisions involving long data frames, although RTS messages themselves can still collide.&#x20;

Unit-5 Wireless and Mobile Networks(1).pdf



# 2. Unit IV — Link Layer and LANs

This unit is about how devices communicate over a single link or local network, how they share a communication medium, and how they detect errors.

## A. What is the Data Link Layer?

The Data Link Layer is Layer 2 of the OSI model. Its main responsibility is transferring a network-layer datagram between physically adjacent nodes over a link.

- The Layer 2 data unit is called a frame.
- The Layer 3 data unit is called a packet or datagram.
- The Data Link Layer uses MAC addresses for local delivery.

Your course material lists framing, link access, error detection, error correction, flow control and link-level reliability among its services.&#x20;

Data_Link_Layer.pdf



Viva question: What is the difference between IP and MAC addresses?

An IP address is a logical network-layer address used for routing between networks. A MAC address is a link-layer address used to deliver frames over a local link.

Viva question: Why do we need both the Network Layer and Data Link Layer?

The Network Layer moves packets toward the destination across networks. The Data Link Layer handles frame delivery between adjacent nodes on each link.

## B. Error Detection and Correction

Errors can occur when noise or interference changes bits during transmission. Error-detection mechanisms add redundant information that helps the receiver check whether data has been corrupted.

### 1. Parity checking

A parity bit is an additional bit used to check the number of 1s in a data word.

- Even parity: total number of 1s, including the parity bit, must be even.
- Odd parity: total number of 1s, including the parity bit, must be odd.

Example:

Data = `1011001`

It contains four 1s. For even parity, the parity bit is `0`.

Transmitted data: `10110010`

If a single bit changes, the parity check fails.

Limitations: Simple parity detects any single-bit error but cannot identify and correct the erroneous bit. It may miss errors if an even number of bits change.

### 2. Two-dimensional parity

Parity bits are added for both rows and columns of a data block.

If one bit changes, the receiver identifies:

- the row whose parity fails;
- the column whose parity fails.

Their intersection identifies the erroneous bit, allowing a single-bit error to be corrected. Some multiple-bit errors can still go undetected.&#x20;

crc_new\.pdf



### 3. Checksum

A checksum is an error-detection value computed from data.

For the Internet checksum, the sender:

1. Divides the data into 16-bit words.
2. Adds the words using one's-complement arithmetic.
3. Wraps any carry around to the least significant end.
4. Takes the one's complement of the sum.
5. Transmits the checksum with the data.

The receiver performs a corresponding check. This mechanism is covered in your materials as an Internet checksum used at the transport layer.&#x20;

Chapter_3_VSF_UDP.pdf



### 4. CRC — Cyclic Redundancy Check

CRC is a more powerful error-detection method that uses modulo-2 binary division.

How it works:

```
Sender                              Receiver
   |                                    |
   | Append zeros                       |
   | Divide using XOR                   |
   | Calculate remainder                |
   | Append remainder                   |
   |                                    |
   |---------- Transmit frame --------->|
   |                                    |
   |                         Divide received frame
   |                         by same generator
   |                                    |
   |                         Remainder = 0?
   |                         /           \
   |                       Yes            No
   |                        |              |
   |                  No error        Error detected
   |                  detected
```

The sender appends \\(r\\) zeros, where \\(r\\) is the degree of the generator polynomial, divides by the generator using XOR, and appends the remainder to the original data.

At the receiver:

- remainder `0` means no error was detected;
- non-zero remainder means an error was detected.

Viva questions

Q. What is the difference between error detection and error correction?

Error detection identifies that data may be corrupted. Error correction identifies and corrects certain errors without necessarily requiring retransmission.

Q. Why is CRC preferred to simple parity for many link-layer applications?

CRC detects a wider range of error patterns, including many burst errors. Your slides identify CRC as being used in Ethernet and Wi-Fi.&#x20;

crc_new\.pdf



Q. What does XOR do in CRC?

XOR performs the modulo-2 subtraction operation used during binary division.

## C. Multiple Access Protocols

Multiple access protocols determine how several devices share a common broadcast channel. When two or more devices transmit simultaneously, their signals can interfere.

Your course divides MAC protocols into three principal categories.&#x20;

Data_Link_Layer.pdf



| Category             | Principle                              | Examples               |
| -------------------- | -------------------------------------- | ---------------------- |
| Channel partitioning | Divide channel resources               | TDMA, FDMA, CDMA       |
| Random access        | Devices contend for the channel        | ALOHA, CSMA            |
| Taking turns         | Devices transmit in an organized order | Polling, token passing |

### TDMA — Time Division Multiple Access

The channel is divided into time slots. Each station is assigned a slot in a repeating cycle.

```
Time →
| A | B | C | A | B | C |
```

Advantage: Predictable, organized access.

Limitation: An unused slot can waste capacity.

### FDMA — Frequency Division Multiple Access

The available frequency spectrum is split into frequency bands.

```
Frequency
   ↑
   | Band A — Device A
   | Band B — Device B
   | Band C — Device C
   +--------------------→ Time
```

Advantage: Different devices can use different frequencies.

Limitation: An assigned band can remain unused when its device has no data.

### CDMA — Code Division Multiple Access

Each user receives a unique chipping code. Multiple users can transmit over the same frequency at the same time, and a receiver uses the intended user's code to recover its data.

Remember: TDMA divides by time, FDMA by frequency, and CDMA by code.

### ALOHA and Slotted ALOHA

Pure ALOHA: A node transmits when data becomes available. Overlapping transmissions can collide.

Slotted ALOHA: Time is divided into slots, and nodes transmit only at slot boundaries.

Your notes give maximum theoretical efficiencies of approximately 18% for Pure ALOHA and 37% for Slotted ALOHA under the stated assumptions.

### Polling and token passing

Polling: A central master invites stations to transmit in turn.

Token passing: A special token circulates between nodes; a node transmits when it holds the token.

Both organize transmission opportunities, but they add overhead and can incur waiting delays.

# 3. ARP — Address Resolution Protocol

ARP answers the question:

> Given a local destination's IP address, how do I discover its MAC address?

Each node maintains an ARP table containing IP-to-MAC mappings.

### Communication within the same LAN

```
Host A                         Host B
   |                              |
   |---- ARP Request ------------>|
   | "Who has B's IP address?"    |
   |                              |
   |<------- ARP Reply ------------|
   |        B's MAC address        |
   |                              |
   |-------- DATA FRAME ---------->|
```

Host A broadcasts an ARP Request. Host B replies with its MAC address, which A can cache in its ARP table.

### Communication across different LANs

```
Host A                 Router                 Host B
   |                      |                      |
   |-- ARP for gateway -->|                      |
   |<-- Gateway MAC ------|                      |
   |                      |                      |
   |-- Frame to router -->|                      |
   |                      |-- ARP for B's MAC -->|
   |                      |<-- B's MAC ----------|
   |                      |-- New frame -------->|
```

A does not ARP for B directly because B is in another subnet. A ARPs for the default gateway. The router forwards the IP packet toward LAN2 and, if necessary, performs ARP there to discover B's MAC.

Important viva question: Do IP addresses change at every router?

Normally, the source and destination IP addresses remain the same end-to-end, except for changes caused by mechanisms such as NAT. The link-layer frame is replaced at each routed hop, so the MAC addresses change.

This same-LAN versus different-LAN behavior is directly demonstrated in your ARP material.&#x20;

How ARP Works on another LAN (1).docx



# 4. Ethernet and switches

## What is Ethernet?

Ethernet is a widely used LAN technology. It defines frame formats and link-layer communication methods.

Your course notes describe the Ethernet frame as containing:

```
| Preamble | Destination MAC | Source MAC | Type | Data | CRC |
```

- Preamble: synchronizes sender and receiver.
- Destination MAC: identifies the intended link-layer recipient.
- Source MAC: identifies the sender.
- Type: indicates the encapsulated protocol.
- Data: contains the payload.
- CRC: detects errors in the received frame.&#x20;

  Data_Link_Layer(1).pdf



## What is a network switch?

An Ethernet switch is a Layer 2 device that forwards frames based on MAC addresses.

It learns which MAC addresses are reachable through which ports by inspecting the source address of incoming frames.

When a frame arrives:

1. The switch learns the source MAC and incoming port.
2. It looks up the destination MAC in its forwarding table.
3. If the destination port is known, it forwards the frame there.
4. If the destination is unknown, it floods the frame through other ports, except the incoming port.
5. If the destination is on the same incoming segment, it can filter/drop the frame rather than forward it unnecessarily.&#x20;

   Data_Link_Layer.pdf



Viva question: What is the difference between a switch and a router?

A switch generally forwards frames using MAC addresses within a LAN. A router forwards IP packets between networks using routing information.

Viva question: What is a collision domain?

A collision domain is a network segment in which simultaneous transmissions can collide. A switched Ethernet network with dedicated full-duplex links generally avoids the shared-medium collision problem on each link.

## What is a VLAN?

A Virtual LAN (VLAN) logically divides a switched network into separate Layer 2 broadcast domains.

Why do we use VLANs?

- To separate departments logically.
- To reduce unnecessary broadcast traffic within each VLAN.
- To improve network organization and isolation.

Devices in different VLANs generally need Layer 3 routing to communicate.

## What is MPLS?

Multiprotocol Label Switching forwards packets using labels to select paths through an MPLS network.

```
IP Packet
    |
    v
Ingress LER — Adds label
    |
    v
Core LSR — Swaps label
    |
    v
Core LSR — Swaps label
    |
    v
Egress LER — Removes label
    |
    v
Destination
```

- LER: Label Edge Router; adds or removes labels at the network boundary.
- LSR: Label Switch Router; forwards packets using labels.

MPLS supports traffic engineering, QoS and VPN services. The key phrase to remember is label-based forwarding.&#x20;

MPLS.docx



## What is data center networking?

Data center networking connects large numbers of servers and storage systems through switches and routers.

Its major goals include:

- high throughput and low latency;
- scalable interconnection of many servers;
- redundant paths and availability;
- efficient movement of data between servers and external networks.

# 5. Unit V — Wireless and Mobile Networks

Your Unit V slides cover wireless link characteristics, CDMA, IEEE 802.11, cellular Internet access and mobility.&#x20;

Unit-5 Wireless and Mobile Networks.pdf



## A. What is a wireless network?

A wireless network transfers data using radio communication rather than requiring a physical cable for each device.

### Main components

- Wireless host: a laptop, phone or sensor that runs applications.
- Base station: relays communication between wireless hosts and the network infrastructure.
- Wireless link: connects a wireless host to a base station or another node.
- Network infrastructure: connects the wireless part to the broader network.

### Infrastructure mode vs ad hoc mode

Infrastructure mode: wireless hosts communicate through a base station or access point.

Ad hoc mode: there is no base station; nodes communicate directly within wireless coverage and may relay data for one another.

### Why is wireless communication more difficult than wired communication?

Three important reasons in your notes are:

1. Path loss: signal strength decreases as radio signals propagate.
2. Interference: other devices or transmissions affect the signal.
3. Multipath propagation: radio signals reflect off objects and arrive at the receiver over different paths.&#x20;

   Unit-5 Wireless and Mobile Networks(1).pdf



### What is SNR?

SNR means Signal-to-Noise Ratio. It measures the signal strength relative to noise.

A higher SNR generally makes it easier for the receiver to distinguish the transmitted signal.

### What is BER?

BER means Bit Error Rate, the proportion of transmitted bits received incorrectly.

Your slides discuss the SNR–BER trade-off: when signal quality worsens, the system may lower its transmission rate to reduce errors.&#x20;

Unit-5 Wireless and Mobile Networks(1).pdf



## B. CDMA

CDMA stands for Code Division Multiple Access.

Each user is assigned a unique chipping sequence, or code. All users may share the same frequency, while their different codes allow the receiver to separate their transmissions.

```
Sender A ── Code A ──┐
                     ├── Shared Channel ──> Receiver
Sender B ── Code B ──┘                          |
                                                v
                                  Select intended user's code
                                                |
                                                v
                                        Recover data
```

The sender encodes data using its chipping sequence. The receiver uses that same code to decode the desired data from the combined channel signal. Ideally orthogonal codes allow users to coexist with minimal interference.&#x20;

Unit-5 Wireless and Mobile Networks(1).pdf



Possible viva questions

Q. Why are codes needed in CDMA?\
They allow the receiver to distinguish one user's data from the signals of other users sharing the channel.

Q. What is a chipping sequence?\
It is the code sequence used to encode each user's data in CDMA.

Q. How is CDMA different from TDMA?\
CDMA separates users by code; TDMA separates users by time slots.

## C. Wi-Fi / IEEE 802.11

IEEE 802.11 is the family of standards used for Wi-Fi wireless LANs.

In infrastructure mode, a wireless host communicates through an Access Point (AP). A Basic Service Set (BSS) contains the wireless hosts and AP.&#x20;

Unit-5 Wireless and Mobile Networks(1).pdf



### What is association?

Association is the process by which a wireless host selects an AP and establishes a relationship with it so that it can communicate through that AP.

### Passive scanning

The host listens for beacon frames sent by APs, selects an AP and sends an Association Request. The AP sends an Association Response.

### Active scanning

The host broadcasts a Probe Request. APs respond with Probe Responses, and the host then requests association with a selected AP.&#x20;

Unit-5 Wireless and Mobile Networks(1).pdf



### What is the 802.11 frame structure?

```
| Frame Control | Duration | Address 1 | Address 2 |
| Address 3 | Sequence Control | Address 4* |
| Payload | CRC |

*Address 4 is used only in ad hoc mode.
```

- Address 1: receiver.
- Address 2: transmitter.
- Address 3: router interface associated with the AP in the described infrastructure example.
- Address 4: used in ad hoc mode.
- Payload: the data.
- CRC: error detection.

The exact interpretation of address fields depends on the frame type and transmission direction. Your slides give this address mapping for the illustrated 802.11 scenario.&#x20;

Unit-5 Wireless and Mobile Networks.pdf



### What is rate adaptation?

As the mobile moves away from an AP, SNR can fall and BER can rise. The system may switch to a lower transmission rate to achieve a lower error rate.&#x20;

Unit-5 Wireless and Mobile Networks.pdf



### What is power management?

A wireless host can signal that it will sleep until the next beacon frame. The AP tracks whether frames are waiting, and the host wakes to check for pending traffic. If nothing is waiting, it can sleep again. This saves power.&#x20;

Unit-5 Wireless and Mobile Networks.pdf



## D. Cellular Networks and Handover

A cellular network divides a geographic area into cells, each served by a base station.

Important components include:

- Base station (BS): communicates with mobile devices over the radio link.
- MSC (Mobile Switching Center): handles call setup, connection to other networks and mobility functions.
- BSC (Base Station Controller): manages several base stations in the described 2G architecture.
- HLR (Home Location Register): stores permanent subscriber information and location-related information.
- VLR (Visitor Location Register): stores temporary information about subscribers currently visiting an area.

Your Unit V slides cover these elements and show how the cellular architecture evolved across 2G, 3G and 4G LTE.&#x20;

Unit-5 Wireless and Mobile Networks.pdf



### What is handover/handoff?

Handover transfers an ongoing connection from the current base station to a new base station when the mobile moves between cells.

Common reasons include:

- the new base station provides a stronger signal;
- maintaining connectivity as the user moves;
- load balancing between cells.

### Handover sequence

```
Mobile             Old Base Station         New Base Station         Controller/MSC
  |                        |                        |                        |
  |---- Active call ------>|                        |                        |
  |                        |                        |                        |
  |---- Moves toward new cell -------------------->|                        |
  |                        |                        |                        |
  |                        |---- Handover request ------------------------->|
  |                        |                        |                        |
  |                        |<---- Handover decision / coordination --------|
  |                        |                        |                        |
  |                        |                        |<---- Assign resources--|
  |                        |                        |                        |
  |<-------------------- Connection via new base station ------------------|
  |                                                                         |
```

The exact signaling differs by cellular architecture, but the objective is to route the ongoing call through the new base station without interruption. The course material highlights stronger signal and load balancing as reasons for handoff.&#x20;

Unit-5 Wireless and Mobile Networks(1).pdf



Viva question: What is the difference between handover and roaming?

Handover is the transfer of an ongoing connection between base stations or cells. Roaming is the use of a mobile service while registered in another service area or network, away from the home system.

## E. Mobile IP

Mobile IP allows a mobile host to move between networks while retaining its permanent home IP address.

### Key terms

- Home network: the mobile's permanent network.
- Permanent/home address: the stable address used to reach the mobile.
- Home agent (HA): keeps track of where the mobile can currently be reached.
- Visited network: the network in which the mobile is currently located.
- Foreign agent (FA): an entity in the visited network that helps deliver packets to the mobile.
- Care-of address (CoA): the current reachable address associated with the visited network.

Your course material identifies indirect routing, agent discovery and registration with the home agent as the major Mobile IP functions.&#x20;

Unit-5 Wireless and Mobile Networks.pdf



### Mobile IP sequence diagram

```
Correspondent              Home Agent              Foreign Agent            Mobile
      |                          |                        |                     |
      |                          |                        |<-- Enters network --|
      |                          |<------ Registration ---|                     |
      |                          |                        |                     |
      |---- Packet to home IP -->|                        |                     |
      |                          |                        |                     |
      |                          |---- Encapsulated ------>|                     |
      |                          |     packet             |                     |
      |                          |                        |---- Packet -------->|
      |                          |                        |                     |
```

### How does Mobile IP work?

1. The mobile enters a visited network and discovers an available mobility agent.
2. It registers its current care-of address with the home agent.
3. The correspondent continues to send packets to the mobile's permanent home address.
4. The home agent intercepts the packets and forwards them toward the care-of address, often using encapsulation.
5. The foreign agent or equivalent endpoint delivers the packets to the mobile.
6. If the mobile moves to another network, it registers a new care-of address, and the home agent updates its forwarding information.

Thus, the permanent address remains the same, while the care-of address changes as the mobile moves.&#x20;

Unit-5 Wireless and Mobile Networks(1).pdf



Viva question: Why is tunneling used in Mobile IP?

Tunneling encapsulates the original packet inside another packet so it can be forwarded to the mobile's current care-of address while preserving the original packet's destination information.

# 6. SDN — Software-Defined Networking

Your professor explicitly included SDN architecture and components with a diagram.

## What is SDN?

Software-Defined Networking (SDN) separates the control plane, which makes forwarding decisions, from the data plane, which forwards packets. This makes the network more centrally manageable and programmable.&#x20;

SDN (2).docx



### SDN architecture

```
             ┌────────────────────────┐
             │   APPLICATION PLANE    │
             │ Firewall, monitoring,  │
             │ load balancing         │
             └───────────┬────────────┘
                         |
             ┌───────────▼────────────┐
             │  Northbound Interface   │
             │          NBI            │
             └───────────┬────────────┘
                         |
             ┌───────────▼────────────┐
             │    SDN CONTROLLER      │
             │      Control Plane     │
             └───────────┬────────────┘
                         |
             ┌───────────▼────────────┐
             │  Southbound Interface  │
             │          SBI            │
             └───────────┬────────────┘
                         |
             ┌───────────▼────────────┐
             │     SDN SWITCHES       │
             │       Data Plane       │
             └────────────────────────┘

              Management & Orchestration
                        ↕
                  SDN Controller
```

## Main components

1\. SDN Controller — Control Plane

The brain of the network. It maintains a global view, determines forwarding rules, installs flow rules in switches and manages traffic dynamically.

2\. SDN Switches — Data Plane

Forward packets using flow-table rules installed by the controller. They can report statistics and events back to the controller.

3\. Northbound Interface (NBI)

Allows applications to communicate with the controller through APIs. Examples in your material include REST APIs and gRPC.

4\. Southbound Interface (SBI)

Allows the controller to communicate with network devices and install rules/configurations. Examples include OpenFlow, NETCONF and OVSDB.

5\. Application Plane

Contains network applications such as firewall/security, load balancing, traffic engineering and network virtualization.

6\. Management and Orchestration

Supports visualization, automation, policy enforcement and reporting.&#x20;

SDN (2).docx



### Viva questions

Q. Why is SDN useful?

It separates network decision-making from packet forwarding, enabling centralized management, programmability and dynamic policy changes.

Q. What is OpenFlow?

It is a protocol used in the course material as an example of a southbound interface for controller-to-switch communication.

Q. What is the difference between NBI and SBI?

NBI connects applications to the controller. SBI connects the controller to network devices.

Q. Does SDN mean all network traffic passes through the controller?

No. The controller configures forwarding rules; switches generally forward data packets themselves using those rules.

# 7. Unit III — Network Layer

The Network Layer provides host-to-host packet delivery across networks. The syllabus includes IPv4, addressing, NAT, IPv6, switching, routing algorithms, OSPF and BGP.&#x20;

Network Layer - DataPlane.pdf

Network Layer - Control Plane.pdf



## A. Routing vs Forwarding

| Routing                                                      | Forwarding                                                                  |
| ------------------------------------------------------------ | --------------------------------------------------------------------------- |
| Determines the path packets should take through the network. | Moves a packet from the input port to the selected output port of a router. |
| Network-wide/control-plane function.                         | Local/data-plane function.                                                  |

Easy answer: Routing chooses the path; forwarding moves the packet along the path.

## B. Data Plane vs Control Plane

- Data plane: local forwarding operation inside a router.
- Control plane: decides how packets should travel through the network, using routing algorithms or an SDN controller.

A router generally has input ports, a switching fabric, output ports and a routing processor.&#x20;

Network Layer - DataPlane.pdf



## C. IPv4, IPv6 and NAT

IPv4: uses 32-bit addresses.

IPv6: uses 128-bit addresses, providing a much larger address space.

NAT (Network Address Translation): enables devices with private addresses to communicate externally through a translated public address. The NAT device stores translation mappings so that returning traffic can be delivered to the correct internal device.&#x20;

Network Layer - DataPlane.pdf



Viva questions

Q. Why was IPv6 introduced?

Primarily to provide a vastly larger address space than IPv4, along with a redesigned protocol header.

Q. What does NAT do?

It translates addresses—and commonly transport port numbers—between a private network and an external network.

Q. What is a subnet?

A subnet is a group of interfaces sharing the same subnet portion of their IP addresses and able to reach one another without an intervening router.&#x20;

Network Layer - DataPlane.pdf



## D. Link-State Routing — Dijkstra

In Link-State routing, routers learn the network topology and link costs, then run Dijkstra's algorithm to calculate least-cost routes.

### Dijkstra's algorithm

1. Initialize the source distance to 0.
2. Initialize directly connected neighbor distances to their link costs and other distances to infinity.
3. Select the unvisited node with the smallest tentative distance.
4. Mark it as visited.
5. Update the distances to its unvisited neighbors.
6. Repeat until all nodes have been processed.

Update formula:

\\[ D(v)=\min\\{D(v),D(w)+c(w,v)\\} \\]

Viva question: What is the difference between Link State and Distance Vector?

Link State uses global topology information and computes shortest paths locally. Distance Vector uses information exchanged with neighbors and updates routes iteratively.&#x20;

Network Layer - Control Plane.pdf



## E. Distance Vector Routing

Each router initially knows:

- its own distance to itself;
- its cost to directly connected neighbors;
- not the complete route costs known by every other router.

It periodically exchanges distance vectors with neighbors.

The Bellman–Ford update rule is:

\\[ D_x(y)=\min_v\\{c(x,v)+D_v(y)\\} \\]

This means: add the cost to a neighbor to that neighbor's reported cost to the destination, then choose the smallest total. The neighbor achieving the minimum becomes the next hop.&#x20;

Network Layer - Control Plane.pdf



Viva question: What is count-to-infinity?

It is a Distance Vector problem in which routers may slowly increase their estimated distance to an unreachable destination because they incorrectly believe a neighbor has a valid route.

## F. OSPF

OSPF stands for Open Shortest Path First.

It is an intra-AS routing protocol that uses Link-State routing.

Its operation:

1. Routers distribute link-state advertisements.
2. Routers build a topology map.
3. Each router runs Dijkstra's algorithm.
4. Routing/forwarding information is generated from the shortest paths.

OSPF also supports authentication, multiple equal-cost routes and hierarchical areas.&#x20;

Network Layer - Control Plane.pdf



## G. BGP

BGP stands for Border Gateway Protocol.

It is used for routing between autonomous systems (ASes) and is a path-vector protocol.

The two main forms are:

- eBGP: exchanges routing information between different autonomous systems.
- iBGP: distributes BGP routing information within the same autonomous system.

BGP exchanges reachable network prefixes and path attributes, including the AS path, so networks can make routing decisions based on policy as well as reachability.

Important comparison:

| OSPF                                          | BGP                                                          |
| --------------------------------------------- | ------------------------------------------------------------ |
| Intra-AS routing                              | Inter-AS routing, with iBGP distributing routes inside an AS |
| Link-State protocol                           | Path-Vector protocol                                         |
| Uses Dijkstra                                 | Uses path attributes and policy-based route selection        |
| Focuses on routes within an autonomous system | Helps networks exchange routes between autonomous systems    |

# 8. Rapid-Fire Viva Questions

Practise answering each in one or two sentences. These are high-value questions drawn from your syllabus.

## Unit IV — Link Layer

1\. What is a frame?\
A Layer 2 data unit that encapsulates a network-layer datagram.

2\. What is a MAC address?\
A link-layer address used for local frame delivery; typically 48 bits for Ethernet LANs.

3\. What is ARP?\
Address Resolution Protocol maps an IP address to a MAC address on a local network.

4\. What happens if the destination is on another LAN?\
The host ARPs for the default gateway's MAC. The router then routes the packet and uses ARP on the destination LAN if needed.

5\. What is CRC?\
An error-detection method that uses modulo-2 division with a generator polynomial.

6\. What is the difference between CSMA/CD and CSMA/CA?\
CSMA/CD detects and aborts collisions; CSMA/CA attempts to avoid collisions. Your material associates them with Ethernet and Wi-Fi respectively.

7\. What is a switch forwarding table?\
A table mapping MAC addresses to switch ports.

8\. What is flooding?\
Forwarding an incoming frame through all ports except the incoming port when the destination is unknown, subject to the switch's forwarding rules.

9\. What is a VLAN?\
A logical Layer 2 network that creates a separate broadcast domain.

10\. What is MPLS?\
A packet-forwarding technology that uses labels to select paths through an MPLS network.

## Unit V — Wireless and Mobile

11\. What is an access point?\
A base station that connects wireless hosts to a network infrastructure.

12\. What is the hidden-terminal problem?\
Two transmitters cannot hear each other but can interfere at the same receiver.

13\. What is CDMA?\
A multiple-access technique that distinguishes users by assigning different chipping codes.

14\. Why does Wi-Fi use CSMA/CA?\
Collision detection is difficult in wireless networks, so Wi-Fi uses sensing, backoff and acknowledgements to reduce collisions and detect unsuccessful delivery.

15\. What is RTS/CTS?\
A channel-reservation exchange: Request to Send followed by Clear to Send.

16\. What is handover?\
Transferring an ongoing connection from one base station/cell to another as the mobile moves.

17\. What is Mobile IP?\
A mechanism that lets a mobile host retain its permanent home IP address while communicating through another network.

18\. What is a care-of address?\
The current reachable address associated with the mobile's visited network.

19\. What is the role of the Home Agent?\
It keeps track of the mobile's current location and forwards packets toward its care-of address.

20\. What is SDN?\
A networking approach that separates control-plane decisions from data-plane forwarding.

21\. What is a northbound interface?\
An API between applications and the SDN controller.

22\. What is a southbound interface?\
An interface/protocol through which the SDN controller communicates with network devices.

## Unit III — Network Layer

23\. What is the Network Layer's main job?\
To deliver packets from source hosts to destination hosts across networks.

24\. What is the difference between routing and forwarding?\
Routing determines the path; forwarding transfers a packet from the router's input to its selected output port.

25\. What is NAT?\
Translation between private-side and external network address/port mappings.

26\. What is the difference between IPv4 and IPv6?\
IPv4 has 32-bit addresses; IPv6 has 128-bit addresses.

27\. What is Dijkstra's algorithm used for?\
It computes least-cost paths from a source node to other nodes when network topology and link costs are known.

28\. What is Distance Vector routing?\
A distributed routing algorithm in which routers exchange distance estimates with their neighbors and update their routes using the Bellman–Ford equation.

29\. What is OSPF?\
An intra-AS Link-State routing protocol that uses Dijkstra's algorithm.

30\. What is BGP?\
A path-vector routing protocol used to exchange routing information between autonomous systems, with route selection influenced by path attributes and policies.

# 9. The last-minute revision order

If your viva is starting soon, revise in this order:

1. CSMA, CSMA/CD, CSMA/CA, RTS/CTS — explain their operation without looking.
2. ARP and Ethernet switches — know how a frame travels and how MAC addresses are learned.
3. CRC, checksum, parity and CDMA — understand what each does.
4. Wi-Fi frame, handover and Mobile IP — practise the sequence diagrams.
5. SDN architecture and components — be able to draw the diagram from memory.
6. IPv4, IPv6, NAT, Dijkstra, Distance Vector, OSPF and BGP — focus on the differences and algorithms.

One final rule for the viva: start every answer with a clear definition, then explain the working, and finish with its purpose or an example. For a question about CSMA, for instance, begin with “CSMA is a MAC protocol that senses the channel before transmitting,” then explain the steps and the reason collisions can still occur.

That structure makes even an unfamiliar follow-up question easier to answer clearly.