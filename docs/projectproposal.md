# Fireflow v1.0
### 1. Purpose:
Fireflow is built for detecting DDoS attack on a targeted network. Below is an image on how Fireflow will be deployed on a network.

The first version of Fireflow will detect the following types of attacks:

- SYN Flood
- UDP Flood
- ICMP Flood
- IP Fragmentation Flood

Capturing Engine: PF_RING.

Detection Scheme: Data Mining (K-means algorithm).

IP Standard: IPv4.

And in v1.0 it will be only a cmdline tool. 

For the first version, it will only support Port Mirroring, which means it will copy all packets to a designated server. And only traffics from one chosen server will be copied. Once DDoS attack is detected, Fireflow will notify users through the command line.

### 2. Implementation

**A. Capturing packets:**

Packets are captured from PF_RING. The packet data is then saved into a file for later analysis. Here are the configurations for packet capturing: 



Since DDoS attack products a large amount of data, we need to be selective on what information to be analyzed:

*- SYN Flood:*  
The SYN flooding attacks exploit the TCP’s three-way handshake mechanism and its limitation in maintaining half-open connections. When a server receives a SYN request, it returns a SYN/ACK packet to the client. Until the SYN/ACK packet is acknowledged by the client, the connection remains in halfopen state for a period of up to the TCP connection timeout, which is typically set to 75 seconds. The server has built in its system memory a backlog queue to maintain all half-open connections. Since this backlog queue is of finite size, once the backlog queue limit is reached, all connection requests will be dropped resulting in denial-of-service.
Attackers send SYN packets whose source address ﬁelds are spoofed. The server receiving these SYN packets sends SYN/ACK packets to spoofed addresses. If the spoofed address actually exists, it sends an RST packet for the SYN/ACK packet because it did not send the SYN packet. If there is no host having the spoofed address, however, the SYN/ACK packet is discarded by the network and the server waits in vain for an ACK packet to acknowledge it.

- Detecting the SYN attack:
The data of the SYN attack would be received after the analysis of the captured network packets. Based on this research on [SYN Flooding Detection Mechanism](https://arxiv.org/pdf/1202.1761.pdf), the method that produces the highest accuracy for large traffic makes use of Data Mining Technique to analyze the protocol information in packets. Parameters needed for detecting this attack are:
    - Destionation IP
    - Destionation Port
    - Flag
    - Source 
    - Source 
**B. Analyzing traffic:**
