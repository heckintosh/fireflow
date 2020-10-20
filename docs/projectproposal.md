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
- IP Fragmentation Flood

Capturing Engine: PF_RING.

Detection Scheme: Entropy Computing

IP Standard: IPv4.

And in v1.0 it will be only a cmdline tool. 

For the first version, it will only support Port Mirroring, which means it will copy all packets to a designated server. And only traffics from one chosen server will be copied. Once DDoS attack is detected, Fireflow will notify users through the command line.

### 2. Implementation

**A. Capturing module** *[Assigned to: Nguyễn Đức Anh]*

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

**B. Detection**
We utilize the captured data to detect these attacks. In order to observe randomness in a data set of given network traffic, network packet fields can be selected to observe changes. We do not only focus on one attribute like destination IP only. We also focus on other combinations. For different attack types, we consider different packet fields for entropy calculation. 

In general, we consider destination IP, source IP, destination port, source port and packet size
fields of network packet for entropy calculation. 

But each attack detection method is assigned to each member of the team so it is up to them to implement it, with a restriction that the method is based on entropy computing.

*[Assigned to Nguyễn Đức Anh]*
**- SYN Flood:** 
The SYN flooding attacks exploit the TCP’s three-way handshake mechanism and its limitation in maintaining half-open connections. When a server receives a SYN request, it returns a SYN/ACK packet to the client. Until the SYN/ACK packet is acknowledged by the client, the connection remains in halfopen state for a period of up to the TCP connection timeout, which is typically set to 75 seconds. The server has built in its system memory a backlog queue to maintain all half-open connections. Since this backlog queue is of finite size, once the backlog queue limit is reached, all connection requests will be dropped resulting in denial-of-service.
Attackers send SYN packets whose source address ﬁelds are spoofed. The server receiving these SYN packets sends SYN/ACK packets to spoofed addresses. If the spoofed address actually exists, it sends an RST packet for the SYN/ACK packet because it did not send the SYN packet. If there is no host having the spoofed address, however, the SYN/ACK packet is discarded by the network and the server waits in vain for an ACK packet to acknowledge it.

Detecting the SYN attack:
- Examining a window of **10000** packets.
- The fields to be used for entropy calculations:
    <span style="font-family:Roboto; font-size:2em;">F = {dst_ip, src_ip, dst_port, src_port, length, TTL, TCP flag} </span>
Select sets from these attributes:


*[Assigned to Trần Khánh Dương]*
C. Testing Environment 

