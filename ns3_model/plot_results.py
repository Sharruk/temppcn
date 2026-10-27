import matplotlib.pyplot as plt
import numpy as np
import os

protocols = ['TCP', 'UDP']

# Simulation Results directly measured from NS-3
throughput = [2.199, 2.053]       # Mbps
delay = [18.27, 20.06]             # ms
pdr = [99.934, 99.941]             # %
packet_loss = [0, 0]               # Packets

output_dir = r"C:\Users\natha\Downloads\cn\assign\ns3_model"

# 1. Combined 4-Subplot Comparison Graph
fig, axs = plt.subplots(2, 2, figsize=(10, 8))
fig.suptitle('NS-3 Performance Comparison: TCP vs UDP (6-Node Network)', fontsize=14, fontweight='bold')

colors = ['#1f77b4', '#ff7f0e']

# Subplot 1: Throughput
axs[0, 0].bar(protocols, throughput, color=colors, width=0.45)
axs[0, 0].set_title('Throughput (Mbps)')
axs[0, 0].set_ylabel('Mbps')
axs[0, 0].set_ylim(0, 3.0)
for i, v in enumerate(throughput):
    axs[0, 0].text(i, v + 0.08, f"{v} Mbps", ha='center', fontweight='bold')

# Subplot 2: End-to-End Delay
axs[0, 1].bar(protocols, delay, color=colors, width=0.45)
axs[0, 1].set_title('Average End-to-End Delay (ms)')
axs[0, 1].set_ylabel('Milliseconds (ms)')
axs[0, 1].set_ylim(0, 25.0)
for i, v in enumerate(delay):
    axs[0, 1].text(i, v + 0.6, f"{v} ms", ha='center', fontweight='bold')

# Subplot 3: Packet Delivery Ratio (PDR)
axs[1, 0].bar(protocols, pdr, color=colors, width=0.45)
axs[1, 0].set_title('Packet Delivery Ratio - PDR (%)')
axs[1, 0].set_ylabel('Percentage (%)')
axs[1, 0].set_ylim(95, 101)
for i, v in enumerate(pdr):
    axs[1, 0].text(i, v + 0.15, f"{v}%", ha='center', fontweight='bold')

# Subplot 4: Packet Loss
axs[1, 1].bar(protocols, packet_loss, color=colors, width=0.45)
axs[1, 1].set_title('Packet Loss Count')
axs[1, 1].set_ylabel('Number of Packets Lost')
axs[1, 1].set_ylim(0, 5)
for i, v in enumerate(packet_loss):
    axs[1, 1].text(i, v + 0.15, f"{v} pkts", ha='center', fontweight='bold')

plt.tight_layout()
plt.savefig(os.path.join(output_dir, "tcp_udp_comparison_graph.png"), dpi=300)
print("Saved comparison graph successfully!")
