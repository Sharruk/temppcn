# Performance Comparison of TCP vs UDP in a 6-Node Network (NS-3)

## 1. Network Topology & Specifications
* **Number of Nodes**: 6 nodes (`Node 0` to `Node 5`)
* **Topology**:
  * Main Transmission Path: `Node 0 -> Node 2 -> Node 3 -> Node 5`
  * Branched Connections: `Node 1 -> Node 2` and `Node 4 -> Node 2`
* **Source & Destination**:
  * Source: `Node 0`
  * Destination: `Node 5`
* **Link Characteristics**:
  * Link Bandwidth: `5 Mbps`
  * Link Propagation Delay: `5 ms`
  * Queue Discipline: `DropTail`
  * Maximum Queue Size: `50 packets`
* **Application Parameters**:
  * Application Data Rate: `2 Mbps`
  * Packet Size: `1024 bytes`
  * Simulation Duration: `30 seconds` (Traffic transmission between 2.0s and 30.0s)

---

## 2. Simulation Results (Measured via NS-3 FlowMonitor)

| Performance Metric | TCP | UDP |
| :--- | :--- | :--- |
| **Transmitted Packets (Tx)** | 13,658 | 6,835 |
| **Received Packets (Rx)** | 13,649 | 6,831 |
| **Lost Packets** | 0 | 0 |
| **Throughput (Kbps / Mbps)** | **2199.15 Kbps (2.199 Mbps)** | **2053.20 Kbps (2.053 Mbps)** |
| **Packet Delivery Ratio (PDR %)** | **99.934 %** | **99.941 %** |
| **Average End-to-End Delay (ms)** | **18.27 ms** | **20.06 ms** |

---

## 3. Analysis & Reasons for Differences

1. **Packet Transmission Volume**:
   * **TCP** sent 13,658 packets while **UDP** sent 6,835 packets. This is because TCP generates bidirectional feedback packets (acknowledgments / ACKs) and dynamic window updates, whereas UDP follows a simple unidirectional fire-and-forget approach.
2. **Throughput**:
   * Both protocols achieved the offered load of ~2 Mbps over the 5 Mbps bottleneck link. TCP achieved slightly higher effective throughput due to continuous sliding-window streaming.
3. **End-to-End Delay**:
   * The theoretical propagation delay across 3 links (`n0-n2`, `n2-n3`, `n3-n5`) is `3 * 5ms = 15ms`.
   * With transmission time and queue buffering, the delay was **18.27 ms** for TCP and **20.06 ms** for UDP.
4. **Packet Delivery Ratio & Loss**:
   * Since the offered traffic (2 Mbps) is below the link capacity (5 Mbps) and queue capacity is large (50 packets), both protocols experienced **0 dropped packets** and a near-perfect PDR of **~99.94%**.

---

## 4. How to Run in NS-3 (WSL)

### Step 1: Copy script to NS-3 scratch folder (already done)
```bash
cp /mnt/c/Users/natha/Downloads/cn/assign/ns3_model/tcp_udp_comparison.cc /mnt/c/Users/natha/Downloads/cn/assign/ns-allinone-3.43/ns-3.43/scratch/
```

### Step 2: Run TCP Simulation
```bash
cd /mnt/c/Users/natha/Downloads/cn/assign/ns-allinone-3.43/ns-3.43
./ns3 run "scratch/tcp_udp_comparison --transportProt=TCP"
```

### Step 3: Run UDP Simulation
```bash
./ns3 run "scratch/tcp_udp_comparison --transportProt=UDP"
```

### Step 4: Re-generate the plots (Windows terminal)
```powershell
python C:\Users\natha\Downloads\cn\assign\ns3_model\plot_results.py
```
