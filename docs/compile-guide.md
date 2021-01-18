Compile: 
g++ -o ../bin/fireflow -I../include fireflow.cpp capture.cpp process_packet.cpp -llog4cpp -lpfring -lpcap

START PF_RING:
cd ~/Tools/kernel
sudo insmod pf_ring.ko min_num_slots=65536 enable_tx_capture=0