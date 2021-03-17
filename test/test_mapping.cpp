#include <iostream>
#include <gtest/gtest.h>
#include "capture.h"
#include "packet.h"
#include "mapper.h"
using namespace std;
using testing::StaticAssertTypeEq;
packet test_packet1;
packet test_packet2;
vector<packet> packets;
string interface, debuglog, packetlog, cusumlog, substatlog = "";
vector<string> attributes = {"src_ip", "src_port", "flags"};
double window, subwindow = 0;
int estimator, max_files, max_sizelog = 0;
//Remember to declare Capture before Mapper
Capture capture_test(interface, window, subwindow, estimator, attributes);
Mapper test_map;

//packet declaration
TEST(MAPPER, PacketIni)
{
    test_packet2.src_ip = 1234;
    test_packet2.dst_ip = 1234;
    test_packet2.src_port = 1234;
    test_packet2.dst_port = 1234;
    test_packet2.protocol = 1234;
    test_packet2.length = 1234;
    test_packet2.flags = 1234;

    ASSERT_EQ(test_packet2.src_ip, 1234);
    ASSERT_EQ(test_packet2.dst_ip, 1234);
    ASSERT_EQ(test_packet2.src_port, 1234);
    ASSERT_EQ(test_packet2.dst_port, 1234);
    ASSERT_EQ(test_packet2.protocol, 1234);
    ASSERT_EQ(test_packet2.length, 1234);
    ASSERT_EQ(test_packet2.flags, 1234);

    ASSERT_EQ(test_packet1.sample_ratio, 1);
    ASSERT_EQ(test_packet1.dst_ip, 0);
    ASSERT_EQ(test_packet1.src_port, 0);
    ASSERT_EQ(test_packet1.dst_port, 0);
    ASSERT_EQ(test_packet1.protocol, 0);
    ASSERT_EQ(test_packet1.length, 0);
    ASSERT_EQ(test_packet1.flags, 0);
    ASSERT_EQ(test_packet1.ip_fragmented, false);
    ASSERT_EQ(test_packet1.ip_protocol_version, 4);
    packets = {test_packet1, test_packet2};
    ASSERT_EQ(packets.size(), 2);
}

// Mapper::Mapper()
TEST(MAPPER, MapInitializing)
{
    ASSERT_EQ(test_map.getAttributeStatus("src_ip"), true);
    ASSERT_EQ(test_map.getAttributeStatus("src_port"), true);
    ASSERT_EQ(test_map.getAttributeStatus("flags"), true);
}
//Mapper::GetValCountAllHeaders
TEST(MAPPER, CountHeaders)
{
    map<string, map<uint, uint>> tmp = test_map.GetValCountAllHeaders(packets);
    for (const auto &map_pair : tmp){
        for (const auto &header_counter : map_pair.second){
            cout << map_pair.first << ' ' << header_counter.first <<  ' ' << header_counter.second << endl;
        }
    }
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}