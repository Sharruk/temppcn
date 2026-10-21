# NS-3 Lab Run Guide: Practical Execution, Compilation, and Troubleshooting

This guide provides step-by-step instructions for copying, writing, compiling, executing, and troubleshooting NS-3 C++ simulations for your Computer Networks lab examination.

---

## 1. Environment and Path Configuration

### Current Development Setup

| Component | Current Local Environment | Lab Exam Machine (To Substitute) |
|---|---|---|
| **Operating System** | Windows with Ubuntu running through WSL | Native Ubuntu / Linux or Lab VM |
| **NS-3 Version** | `3.47` | Lab installed NS-3 version |
| **NS-3 Installation Root** | `~/ns3-lab/ns-3.47` | e.g., `/home/student/ns-allinone-3.xx/ns-3.xx` |
| **Project Repository** | `/mnt/c/Directory/coding/College_Assignments/sem_5/Computer_Networks/exam/PCN-main/temppcn` | Path where you clone/extract your repository |
| **Execution Launcher** | `./ns3 run ...` | `./ns3 run ...` or `./waf --run ...` (in NS-3.35 or older) |

> [!IMPORTANT]
> The paths above (such as `/mnt/c/...` and `~/ns3-lab/ns-3.47`) are specific to this Windows/WSL computer. In the lab examination, substitute the **actual path** to the assignment folder and the **actual path** to the NS-3 installation provided on the lab machine.

---

## 2. Core Architecture Concept: Repository vs. NS-3 Root

Understanding this distinction is the single most important rule for avoiding errors during the exam:

```
+-------------------------------------------------------------------------------+
| 1. PROJECT REPOSITORY                                                         |
|    Path: /mnt/c/.../temppcn/                                                  |
|    Contains: Assignment folders (A7, A8, A9, A11), source variants, scripts.  |
|    CANNOT compile or execute NS-3 simulations directly here!                  |
+-------------------------------------------------------------------------------+
                                      |
                               (Step: cp file)
                                      v
+-------------------------------------------------------------------------------+
| 2. NS-3 INSTALLATION ROOT                                                     |
|    Path: ~/ns3-lab/ns-3.47/                                                   |
|    Contains:                                                                  |
|      - ./ns3               (Python/C++ build & runner executable)             |
|      - scratch/            (The directory where your user programs live)      |
|      - build/ & src/       (Core simulator libraries and models)              |
|    YOU MUST BE IN THIS DIRECTORY TO RUN ./ns3 commands!                       |
+-------------------------------------------------------------------------------+
```

### The Standard 6-Step Workflow

1. **Locate the NS-3 installation root** on the lab computer (where the `./ns3` launcher is present).
2. **Locate the required `.cc` source file** in your project repository.
3. **Copy that source file** into the NS-3 root's `scratch/` directory.
4. **Change terminal directory (`cd`)** to the NS-3 installation root.
5. **Run the program** via `./ns3 run scratch/<filename-without-extension>`.
6. **Read and verify the simulation metrics** (Throughput, Delay, Packet Delivery Ratio, Packet Loss).

---

## 3. Beginner-Friendly Setup and Directory Verification

### Step 3.1: Navigate and Verify on Your Current Computer

Open your WSL terminal and run:

```bash
# 1. View your project repository
cd "/mnt/c/Directory/coding/College_Assignments/sem_5/Computer_Networks/exam/PCN-main/temppcn"
pwd
ls -l

# 2. Switch to your NS-3 installation root
cd ~/ns3-lab/ns-3.47
pwd
ls -l ns3
```

Check the contents of the `scratch/` folder:

```bash
ls -la scratch
```

If the `scratch/` directory does not exist for any reason, create it:

```bash
mkdir -p scratch
```

### Step 3.2: How to Find NS-3 on Any Lab Computer

When you sit at a new lab computer, NS-3 might be installed in a different folder (e.g., `~/Desktop/ns-allinone-3.47/ns-3.47`, `/opt/ns3`, or `/home/user/ns3-lab`).

Do **not** reinstall NS-3 if it is already present. Search for the `./ns3` executable:

```bash
# Search in your user home directory
find ~ -maxdepth 4 -name "ns3" -type f -perm /111 2>/dev/null

# If not in home, search common system paths
find /opt /usr/local /home -name "ns3" -type f -perm /111 2>/dev/null
```

Once you see a path like `/home/student/ns-allinone-3.47/ns-3.47/ns3`, change directory to that folder and verify:

```bash
cd /home/student/ns-allinone-3.47/ns-3.47
ls
# Verify that './ns3', 'src', and 'scratch' exist
```

---

## 4. Source Files and Copy Commands

To make commands copy-paste ready, set your shell path variables first:

```bash
# Set repository and NS-3 directories as variables
REPO_DIR="/mnt/c/Directory/coding/College_Assignments/sem_5/Computer_Networks/exam/PCN-main/temppcn"
NS3_ROOT="$HOME/ns3-lab/ns-3.47"
```

### Assignment Mapping Table

The repository provides two variants for each assignment:
- **`source-variants/`**: Self-contained, clean baseline implementations (ideal for standard viva questions and standard runs).
- **`scratch-programs/`**: Multi-parameter experiment sweeps (ideal for investigating metrics across speeds, bandwidths, or queues).

The table below lists the verified baseline programs to copy and execute:

| Assignment | Topic | Original Repository Path | Destination in `NS3_ROOT/scratch/` | Copy Command | Run Command (from `NS3_ROOT`) |
|---|---|---|---|---|---|
| **Assignment 7** | UDP Point-to-Point Latency & PDR | `A7-UDP-Latency/source-variants/point-to-point-udp-latency.cc` | `ass7_p2p_latency.cc` | `cp "$REPO_DIR/A7-UDP-Latency/source-variants/point-to-point-udp-latency.cc" "$NS3_ROOT/scratch/ass7_p2p_latency.cc"` | `./ns3 run scratch/ass7_p2p_latency` |
| **Assignment 8** | TCP vs. UDP Bottleneck Competition | `A8-TCP-UDP-Bottleneck/source-variants/tcp-udp-bottleneck.cc` | `ass8_tcp_udp_bottleneck.cc` | `cp "$REPO_DIR/A8-TCP-UDP-Bottleneck/source-variants/tcp-udp-bottleneck.cc" "$NS3_ROOT/scratch/ass8_tcp_udp_bottleneck.cc"` | `./ns3 run scratch/ass8_tcp_udp_bottleneck` |
| **Assignment 9** | TCP Congestion Control (Cubic / Reno / BBR) | `A9-TCP-Congestion-Control/source-variants/tcp-congestion-control.cc` | `ass9_tcp_congestion.cc` | `cp "$REPO_DIR/A9-TCP-Congestion-Control/source-variants/tcp-congestion-control.cc" "$NS3_ROOT/scratch/ass9_tcp_congestion.cc"` | `./ns3 run "scratch/ass9_tcp_congestion --tcpVariant=TcpCubic"` |
| **Assignment 11** *(Base)* | 802.11 AP & Mobile Stations | `A11-Wireless-Mobility/source-variants/wireless-mobility.cc` | `ass11_wireless_mobility.cc` | `cp "$REPO_DIR/A11-Wireless-Mobility/source-variants/wireless-mobility.cc" "$NS3_ROOT/scratch/ass11_wireless_mobility.cc"` | `./ns3 run scratch/ass11_wireless_mobility` |
| **Assignment 11** *(Comparison)* | Multi-Tech (WiFi/Bluetooth/Zigbee) Sweep | `A11-Wireless-Mobility/scratch-programs/wireless-mobility-experiment.cc` | `ass11_wireless_comparison.cc` | `cp "$REPO_DIR/A11-Wireless-Mobility/scratch-programs/wireless-mobility-experiment.cc" "$NS3_ROOT/scratch/ass11_wireless_comparison.cc"` | `./ns3 run "scratch/ass11_wireless_comparison --tech=WiFi --speed=1"` |

> [!NOTE]
> **Assignment 10 (Dynamic Routing Protocols)** is excluded from this guide as requested.
> 
> **Important API Note for Assignment 11 (NS-3.47):**  
> In `A11-Wireless-Mobility/source-variants/wireless-mobility.cc`, the code specifies `wifi.SetStandard(WIFI_STANDARD_80211n_2_4GHZ);`. In NS-3.47, that constant has been streamlined to `WIFI_STANDARD_80211n`. If you copy directly from the repository on NS-3.47, ensure that line reads `wifi.SetStandard(WIFI_STANDARD_80211n);` (already updated in your working `ass11_wireless_mobility.cc`).

---

## 5. Writing a New Program From Scratch During the Exam

If the lab examiner instructs you to write or modify a program from scratch during the exam:

### Step 1: Open Terminal and Go to NS-3 Root
```bash
cd ~/ns3-lab/ns-3.47
```

### Step 2: Create the Source File Directly Inside `scratch/`
Use `nano` (or another installed terminal editor like `gedit` or `vim`):

```bash
nano scratch/my_exam_test.cc
```

### Step 3: Type or Paste Your Minimal NS-3 Program
For example, a minimal simulation skeleton:

```cpp
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("ExamLab");

int main(int argc, char *argv[]) {
    CommandLine cmd(__FILE__);
    cmd.Parse(argc, argv);

    Time::SetResolution(Time::NS);
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    NodeContainer nodes;
    nodes.Create(2);

    PointToPointHelper p2p;
    p2p.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    p2p.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer devices = p2p.Install(nodes);

    InternetStackHelper stack;
    stack.Install(nodes);

    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces = address.Assign(devices);

    UdpEchoServerHelper echoServer(9);
    ApplicationContainer serverApps = echoServer.Install(nodes.Get(1));
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient(interfaces.GetAddress(1), 9);
    echoClient.SetAttribute("MaxPackets", UintegerValue(5));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApps = echoClient.Install(nodes.Get(0));
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));

    Simulator::Run();
    Simulator::Destroy();
    return 0;
}
```

### Step 4: Save and Exit `nano`
- Press `Ctrl + O` (WriteOut).
- Press `Enter` (Confirm file name: `scratch/my_exam_test.cc`).
- Press `Ctrl + X` (Exit `nano`).

### Step 5: Run the Simulation
```bash
./ns3 run scratch/my_exam_test
```

> [!WARNING]
> **Always omit the `.cc` extension in the run command.**
> - Correct: `./ns3 run scratch/my_exam_test`
> - Incorrect: `./ns3 run scratch/my_exam_test.cc` (will cause a "target not found" error).

---

## 6. Detailed Step-by-Step Execution Workflow

### Assignment 7: UDP Traffic & Latency Analysis

1. **Copy source to scratch:**
   ```bash
   cd ~/ns3-lab/ns-3.47
   cp /mnt/c/Directory/coding/College_Assignments/sem_5/Computer_Networks/exam/PCN-main/temppcn/A7-UDP-Latency/source-variants/point-to-point-udp-latency.cc scratch/ass7_p2p_latency.cc
   ```
2. **Execute simulation:**
   ```bash
   ./ns3 run scratch/ass7_p2p_latency
   ```
3. **Expected Output:**
   ```text
   --- Flow 1 Metrics ---
   Tx Packets : 800 | Rx Packets: 800
   Throughput : 0.8416 Mbps
   Packet Delivery Ratio (PDR) : 100 %
   Average Latency / Delay      : 3.6864 ms
   ```
4. **Command-Line Arguments:**
   `ass7_p2p_latency.cc` is a fixed baseline simulation. If you wish to test variable propagation delays and dual flows, you can copy `A7-UDP-Latency/scratch-programs/udp-latency-experiment-2.cc` which accepts `--delay="10ms"` and `--twoFlows=true`.

---

### Assignment 8: TCP vs. UDP Bottleneck Competition

1. **Copy source to scratch:**
   ```bash
   cd ~/ns3-lab/ns-3.47
   cp /mnt/c/Directory/coding/College_Assignments/sem_5/Computer_Networks/exam/PCN-main/temppcn/A8-TCP-UDP-Bottleneck/source-variants/tcp-udp-bottleneck.cc scratch/ass8_tcp_udp_bottleneck.cc
   ```
2. **Execute simulation:**
   ```bash
   ./ns3 run scratch/ass8_tcp_udp_bottleneck
   ```
3. **Expected Output:**
   ```text
   Flow 1 | Tx: 1399 | Rx: 1307 | Lost: 84 | Throughput: 0.681948 Mbps | PDR: 93.4239 %
   Flow 2 | Tx: 935 | Rx: 934 | Lost: 0 | Throughput: 0.0536284 Mbps | PDR: 99.893 %
   Flow 3 | Tx: 781 | Rx: 529 | Lost: 214 | Throughput: 0.25392 Mbps | PDR: 67.7337 %
   ```
4. **Observation:**
   Flow 1 is TCP, Flow 2 is TCP Ack, Flow 3 is UDP. Under bottleneck conditions, aggressive UDP traffic causes packet drops in TCP, demonstrating bottleneck starvation and drop-tail queue dynamics.

---

### Assignment 9: TCP Congestion Control Evaluation

This program accepts the `--tcpVariant` parameter to compare congestion control algorithms.

1. **Copy source to scratch:**
   ```bash
   cd ~/ns3-lab/ns-3.47
   cp /mnt/c/Directory/coding/College_Assignments/sem_5/Computer_Networks/exam/PCN-main/temppcn/A9-TCP-Congestion-Control/source-variants/tcp-congestion-control.cc scratch/ass9_tcp_congestion.cc
   ```

2. **Execute with TCP Cubic (Default):**
   ```bash
   ./ns3 run "scratch/ass9_tcp_congestion --tcpVariant=TcpCubic"
   ```
   **Output:**
   ```text
   === Algorithm: TcpCubic ===
   Throughput : 9.89806 Mbps
   Avg Delay  : 48.8262 ms
   Packets Rx : 18941 | Tx: 19043
   ```

3. **Execute with TCP NewReno:**
   ```bash
   ./ns3 run "scratch/ass9_tcp_congestion --tcpVariant=TcpNewReno"
   ```
   **Output:**
   ```text
   === Algorithm: TcpNewReno ===
   Throughput : 9.86037 Mbps
   Avg Delay  : 48.9171 ms
   Packets Rx : 18876 | Tx: 18962
   ```

4. **Execute with TCP BBR:**
   ```bash
   ./ns3 run "scratch/ass9_tcp_congestion --tcpVariant=TcpBbr"
   ```

> [!TIP]
> **Why Quoting Matters:**  
> When passing arguments to a scratch program, enclosing the target and its arguments in quotes (`"scratch/ass9_tcp_congestion --tcpVariant=TcpNewReno"`) tells the `./ns3` wrapper script that `--tcpVariant` belongs to your C++ executable, not to `./ns3` itself.

---

### Assignment 11: Wireless Networks with Node Mobility

Assignment 11 has two verified programs:

#### Program A: Baseline Fixed Topology with 4 Mobile Stations
```bash
cd ~/ns3-lab/ns-3.47
cp /mnt/c/Directory/coding/College_Assignments/sem_5/Computer_Networks/exam/PCN-main/temppcn/A11-Wireless-Mobility/source-variants/wireless-mobility.cc scratch/ass11_wireless_mobility.cc

# If using NS-3.47, ensure line 50 has WIFI_STANDARD_80211n
./ns3 run scratch/ass11_wireless_mobility
```
**Output:**
```text
Flow 1 | Tx: 488 | Rx: 488 | Throughput: 0.456334 Mbps | PDR: 100 % | Delay: 1.02679 ms
Flow 2 | Tx: 488 | Rx: 488 | Throughput: 0.456334 Mbps | PDR: 100 % | Delay: 1.04014 ms
Flow 3 | Tx: 488 | Rx: 488 | Throughput: 0.456334 Mbps | PDR: 100 % | Delay: 1.04415 ms
Flow 4 | Tx: 488 | Rx: 488 | Throughput: 0.456334 Mbps | PDR: 100 % | Delay: 1.01697 ms
```

#### Program B: Technology Comparison at Variable Speeds
This program accepts:
- `--tech=WiFi` | `Bluetooth` | `Zigbee` (Default: `WiFi`)
- `--speed=1` | `5` | `10` (m/s) (Default: `1.0`)
- `--simTime=30.0` (Default: `30.0`)

```bash
cd ~/ns3-lab/ns-3.47
cp /mnt/c/Directory/coding/College_Assignments/sem_5/Computer_Networks/exam/PCN-main/temppcn/A11-Wireless-Mobility/scratch-programs/wireless-mobility-experiment.cc scratch/ass11_wireless_comparison.cc

# Run with WiFi at 1 m/s:
./ns3 run "scratch/ass11_wireless_comparison --tech=WiFi --speed=1"

# Run with Bluetooth at 5 m/s:
./ns3 run "scratch/ass11_wireless_comparison --tech=Bluetooth --speed=5"

# Run with Zigbee at 10 m/s:
./ns3 run "scratch/ass11_wireless_comparison --tech=Zigbee --speed=10"
```

**Sample Output (Bluetooth @ 5 m/s):**
```text
===== tech=Bluetooth speed=5m/s =====
Tx=3624 Rx=336 | Throughput=21.1332Kbps | PacketLoss=90.7285% | AvgDelay=2.36956ms | AvgSignal=-76.208dBm
```

---

## 7. Run Every Required Assignment

Use these copy-paste terminal blocks to execute your experiments quickly and verify all baseline programs.

### Option 1: Individual Run Commands

```bash
cd ~/ns3-lab/ns-3.47

# Run Assignment 7 (UDP Latency)
./ns3 run scratch/ass7_p2p_latency

# Run Assignment 8 (TCP/UDP Bottleneck)
./ns3 run scratch/ass8_tcp_udp_bottleneck

# Run Assignment 9 (TCP Congestion Control)
./ns3 run "scratch/ass9_tcp_congestion --tcpVariant=TcpCubic"
./ns3 run "scratch/ass9_tcp_congestion --tcpVariant=TcpNewReno"
./ns3 run "scratch/ass9_tcp_congestion --tcpVariant=TcpBbr"

# Run Assignment 11 (Wireless Fixed Topology)
./ns3 run scratch/ass11_wireless_mobility

# Run Assignment 11 (Technology Comparison Baseline)
./ns3 run "scratch/ass11_wireless_comparison --tech=WiFi --speed=1"
```

---

### Option 2: Sequential Baseline Execution Block

Run all baseline assignments sequentially. If any program fails, the command stops immediately (`set -e`) and prints which stage failed:

```bash
cd ~/ns3-lab/ns-3.47 && {
    echo "=========================================="
    echo ">>> Running Assignment 7 (UDP Latency)..."
    ./ns3 run scratch/ass7_p2p_latency || { echo "FAILED at Assignment 7"; exit 1; }

    echo "=========================================="
    echo ">>> Running Assignment 8 (TCP/UDP Bottleneck)..."
    ./ns3 run scratch/ass8_tcp_udp_bottleneck || { echo "FAILED at Assignment 8"; exit 1; }

    echo "=========================================="
    echo ">>> Running Assignment 9 (TCP Congestion - Cubic)..."
    ./ns3 run "scratch/ass9_tcp_congestion --tcpVariant=TcpCubic" || { echo "FAILED at Assignment 9 Cubic"; exit 1; }

    echo "=========================================="
    echo ">>> Running Assignment 11 (Wireless Mobility Base)..."
    ./ns3 run scratch/ass11_wireless_mobility || { echo "FAILED at Assignment 11 Mobility"; exit 1; }

    echo "=========================================="
    echo ">>> Running Assignment 11 (Wireless Comparison - WiFi @ 1m/s)..."
    ./ns3 run "scratch/ass11_wireless_comparison --tech=WiFi --speed=1" || { echo "FAILED at Assignment 11 Comparison"; exit 1; }

    echo "=========================================="
    echo ">>> ALL BASELINE EXPERIMENTS COMPLETED SUCCESSFULLY! <<<"
}
```

---

### Option 3: Assignment 11 Complete Technology & Speed Sweep

This block tests all 9 combinations of technology (`WiFi`, `Bluetooth`, `Zigbee`) and mobile node speeds (`1 m/s`, `5 m/s`, `10 m/s`):

```bash
cd ~/ns3-lab/ns-3.47 && {
    echo "===== STARTING ASSIGNMENT 11 WIRELESS SWEEP ====="
    for tech in WiFi Bluetooth Zigbee; do
        for speed in 1 5 10; do
            echo "--------------------------------------------------"
            echo "Running: Tech=$tech | Speed=${speed}m/s"
            ./ns3 run "scratch/ass11_wireless_comparison --tech=$tech --speed=$speed" || {
                echo "ERROR running Tech=$tech Speed=$speed";
                exit 1;
            }
        done
    done
    echo "--------------------------------------------------"
    echo "===== ASSIGNMENT 11 SWEEP FINISHED SUCCESSFULLY! ====="
}
```

---

## 8. Common Errors and Troubleshooting Table

| Error / Symptom | What It Means | Immediate Solution & Action |
|---|---|---|
| `./ns3: No such file or directory` | You are running the command from the wrong folder (e.g. inside `temppcn` instead of `~/ns3-lab/ns-3.47`). | Change directory to your NS-3 root: <br>`cd ~/ns3-lab/ns-3.47`<br>Confirm with `ls ns3`. |
| `cp: cannot stat '...': No such file or directory` | The source file path in the repository is mistyped, or the directory path is wrong. | Check the source path: <br>`ls -l /mnt/c/Directory/coding/...`<br>Use absolute paths or check with `find . -name "*.cc"`. |
| `Target ... not found` or `scratch/xxx.cc does not exist` | You included `.cc` in `./ns3 run scratch/prog.cc`, or the file has not been copied to `scratch/`. | 1. Omit `.cc`: run `./ns3 run scratch/prog`.<br>2. Verify the file exists in scratch: `ls -l scratch/`. |
| `ninja: no work to do.` | This is **NOT an error**! Ninja is informing you that your C++ code has already been compiled and no files were modified. | No action needed. The simulator executes immediately right after this message. |
| C++ compilation errors (`error: ...`) | Syntax error, missing semicolon, or type mismatch in the `.cc` file inside `scratch/`. | Inspect the file and line number shown in the error: <br>`nano scratch/<filename>.cc`<br>Fix the syntax, save (`Ctrl+O`, `Ctrl+X`), and re-run. |
| `WIFI_STANDARD_80211n_2_4GHZ was not declared` | API constant version mismatch. NS-3.47 unified 802.11n constants. | In `scratch/ass11_wireless_mobility.cc`, change:<br>`wifi.SetStandard(WIFI_STANDARD_80211n_2_4GHZ);`<br>to:<br>`wifi.SetStandard(WIFI_STANDARD_80211n);` |
| `Invalid command-line argument` / `CommandLine Error` | You passed an option that does not exist in that specific program's `cmd.AddValue` calls. | Check valid parameters in the code: <br>`grep -n "AddValue" scratch/<filename>.cc`<br>Only pass supported flags (e.g., `--tcpVariant` for Ass 9; `--tech`, `--speed` for Ass 11 Comparison). |
| Edited `.cc` file, but output doesn't change | You edited the file in your project repository instead of the copy inside `scratch/`. | Remember NS-3 only compiles what is inside `scratch/`! Either re-copy the file or edit `scratch/<filename>.cc` directly. |
| Program executes but receives 0 packets | Simulation completed, but sender and receiver are on different IP subnets, or applications start after simulation ends. | Check node IP assignment (`10.1.1.0/24`) and ensure `clientApps.Start(Seconds(1.0))` occurs before `Simulator::Stop(Seconds(10.0))`. |
| `You need to configure ns-3 first: try ./ns3 configure` | CMake cache was cleared or `./ns3 clean` was run. | Run: <br>`./ns3 configure`<br>Once finished, re-run your `./ns3 run scratch/<program>` command. |

---

## 9. NS-3 Lab Exam: Remember These Commands

Keep this single-page sequence in mind for the lab examination:

```bash
# 1. Navigate to the NS-3 installation root
cd "$NS3_ROOT"

# 2. Check the directory and verify the ns3 launcher
pwd
ls
ls scratch

# 3. Create scratch directory if missing
mkdir -p scratch

# 4. Copy a source file into scratch
cp "/path/to/program.cc" scratch/program.cc

# 5. Run the program (omit .cc)
./ns3 run scratch/program

# 6. Run with supported command-line arguments
./ns3 run "scratch/program --option=value"

# 7. Inspect or edit a source file
nano scratch/program.cc
```

> [!NOTE]
> `"$NS3_ROOT"` and `"/path/to/program.cc"` are placeholders. On your exam computer, replace them with the actual paths (for example: `cd ~/ns3-lab/ns-3.47` and `cp ... scratch/ass7_p2p_latency.cc`).

---

### Golden Decision Rule

- **If you already have the source file:** Copy it to `scratch/` using `cp <path_to_source> scratch/<target_name>.cc`.
- **If you must write code during the exam:** Create the `.cc` file directly inside `scratch/` using `nano scratch/<target_name>.cc`.
- **In both cases:** Always run `./ns3 run scratch/<target_name>` **from the NS-3 installation root**.
