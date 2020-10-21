# Fireflow v1.0
This proposal will be gradually updated at https://github.com/sanebinary/fireflow/blob/master/docs/projectproposal.md
### Github link: https://github.com/sanebinary/fireflow
### Members:
- Nguyễn Đức Anh
- Đoàn Minh Long
- Trần Việt Anh
- Trần Khánh Dương
- Vũ Long Dũng

### 1. Purpose:
Fireflow is built for detecting DDoS attack on a targeted network. Below is an image on how Fireflow will be deployed on a network.

The first version of Fireflow will detect the following types of attacks:

- SYN Flood
- UDP Flood
- ICMP Flood

Capturing Engine: PF_RING.

Detection Scheme: Entropy Computing

IP Standard: IPv4.

And in v1.0 it will be only a cmdline tool. 

For the first version, it will only support Port Mirroring, which means it will copy all packets to a designated server. And only traffics from one chosen server will be copied. Once DDoS attack is detected, Fireflow will notify users through the command line.

### 2. Implementation

*[Assigned to: Nguyễn Đức Anh]*

####**A. Capturing module** 
Packets are captured from PF_RING. The packet data is then saved into a file for later analysis. Here are the configurations for packet capturing in version 1.0 of Fireflow: 
- Source IP
- Destionation IP
- Source Port
- Destination Port
- Input interface
- Output interface
- IP protocol version
- TTL
- Protocol 
- Packet length
- Number of packets
- Flags
- IP Fragmented
- Timeval ts
- Packet payload full length      

####**B. Monitoring module**
We utilize the captured data to detect these attacks. In order to observe randomness in a data set of given network traffic, network packet fields can be selected to observe changes. We do not only focus on one attribute like destination IP only. We also focus on other combinations. For different attack types, we consider different packet fields for entropy calculation. 

In general, we consider destination IP, source IP, destination port, source port and packet size
fields of network packet for entropy calculation. 

But each attack detection method is assigned to each member of the team so it is up to them to implement it, with a restriction that the method is based on entropy computing.

*[Assigned to Nguyễn Đức Anh]*
**- SYN Flood:** 
The SYN flooding attacks exploit the TCP’s three-way handshake mechanism and its limitation in maintaining half-open connections. When a server receives a SYN request, it returns a SYN/ACK packet to the client. Until the SYN/ACK packet is acknowledged by the client, the connection remains in halfopen state for a period of up to the TCP connection timeout, which is typically set to 75 seconds. The server has built in its system memory a backlog queue to maintain all half-open connections. Since this backlog queue is of finite size, once the backlog queue limit is reached, all connection requests will be dropped resulting in denial-of-service.
Attackers send SYN packets whose source address ﬁelds are spoofed. The server receiving these SYN packets sends SYN/ACK packets to spoofed addresses. If the spoofed address actually exists, it sends an RST packet for the SYN/ACK packet because it did not send the SYN packet. If there is no host having the spoofed address, however, the SYN/ACK packet is discarded by the network and the server waits in vain for an ACK packet to acknowledge it.

Gathering the necessary information for the decision module to decide if there is a SYN flood attack:
- The observation period (window) W of the monitoring module is 1000 packets for the interval of 250 packets.
- The fields to be used for entropy calculations:
    <span style="font-family:Roboto; font-size:1.5em;">F = {dst_ip, src_ip, dst_port, src_port, packet_length} </span>
    Future versions will include several more fields for entropy calculation and the fields will also be combined into sets for calculating entropy as well.
- To calculate entropy we need to get the flow count. In particular, for the SYN attack, packets will only be counted if the Flags are set to: <span style="font-family:Roboto; font-size:1.5em;"> {(SYN); (ACK); (RST); (FIN); (SYN,ACK); (SYN,RST); (FIN/ACK)} </span>
    For example:

    |  dst_ip | Flow count  |
    |---------|-------------|
    |10.0.0.1 |   350       |
    |10.0.0.2 |   350       |
    |10.0.0.3 |   300       |

- Pass the flow count information of all fields to the decision module to determine if there is a SYN attack going on.

*[Assigned to Đoàn Minh Long]*
**- UDP Flood**

*[Assigned to Vũ Long Dũng and Trần Việt Anh]*
**- IMCP Flood**

####**C. Decision module:**

*[Assigned to Nguyễn Đức Anh]*
**- SYN Flood:**
Then using the flow count we can deduce the probability of each element of the attribute during the window W = 1000:

![Equation](https://latex.codecogs.com/gif.latex?P_%7Bdst-ip%7D%5E%7B1%7D%20%3D%20F%28dst-ip1%29/W)

Consecutively calculate the probability for the remaining element of the field. Once we have all the information, it is time to calculate the entropy:

![Equation](https://latex.codecogs.com/gif.latex?E_%7Bdst-ip%7D%20%3D%20-%5Csum_%7Bn%3D0%7D%5E%7BM-1%7DP%28dst-ip_%7Bn%7D%29log%28P%28dst-ip_%7Bn%7D%29%29)

    Just like that, calculate the entropies of all attributes.

We have the entropy values of 5 attributes. With all the information available, determine if there is an attack going on through the following conditions:

Raise alert:
- if 𝐸𝑖(𝑐𝑢𝑟𝑟𝑒𝑛𝑡) - 𝐸𝑖(𝑝𝑟𝑒𝑣) > (2*𝐷𝑖 [𝐶𝑊]𝑠𝑡𝑑𝑒𝑣+𝐷𝑖 [𝐶𝑊]𝑚𝑒𝑎𝑛)
- if 𝐸𝑖(𝑐𝑢𝑟𝑟𝑒𝑛𝑡) - 𝐸𝑖(𝑝𝑟𝑒𝑣) > 𝐷𝑖 [𝐶𝑊]𝑚𝑎𝑥 || 𝐸𝑖(𝑐𝑢𝑟𝑟𝑒𝑛𝑡) > 𝐸𝑖(𝑁 𝑚𝑎𝑥 OR 𝐸𝑖(𝑐𝑢𝑟𝑟𝑒𝑛𝑡) < 𝐸𝑖(𝑁)𝑚𝑖n
- if  𝐸𝑖(𝑐𝑢𝑟𝑟𝑒𝑛𝑡) < 𝐸𝑖(𝑁)𝑚𝑒𝑎𝑛- 𝐸𝑖(𝑁)𝑠𝑡𝑑𝑒𝑣 || 𝐸𝑖(𝑐𝑢𝑟𝑟𝑒𝑛𝑡) > 𝐸𝑖(𝑁)𝑚𝑒𝑎𝑛+ 𝐸𝑖(𝑁)𝑠𝑡𝑑𝑒v

with i = {dst_ip, src_ip, dst_port, src_port, packet_length}

With 𝐷𝑖 [𝐶𝑊]𝑠𝑡𝑑𝑒𝑣, 𝐷𝑖 [𝐶𝑊]𝑚𝑒𝑎𝑛, 𝐷𝑖 [𝐶𝑊]𝑚𝑎𝑥, 𝐸𝑖(𝑁)𝑚𝑒𝑎𝑛 and 𝐸𝑖(𝑁)𝑠𝑡𝑑𝑒v are calculated through normal traffic as threshhold values.

*[Assigned to Đoàn Minh Long]*
**- UDP Flood**

*[Assigned to Vũ Long Dũng and Trần Việt Anh]*
**- IMCP Flood**

*[Assigned to Trần Khánh Dương]*
####**D. Testing Environment** 

