#include "ip.h"

IPPacket::IpVersion IPPacket::GetVersionFromPacket(std::vector<uint8_t> const &rawPacket) {
    switch (rawPacket[0] >> 4)
    {
    case 4:
        return IPPacket::IpVersion::IPv4;
        break;
    
    case 6:
        return IPPacket::IpVersion::IPv6;
        break;
    }
    return IPPacket::IpVersion::Unknown;
}

IPPacket::L4Proto IPPacket::GetProtoFromPacket(std::vector<uint8_t> const &rawPacket) {
    return IPPacket::L4Proto(rawPacket[9]);
}

std::string IPPacket::IpAddrSrcFromPacket(std::vector<uint8_t> const &packet) {
    std::stringstream ipaddr;
    ipaddr << (int)packet[12] << "." << (int)packet[13] << "." << (int)packet[14] << "." << (int)packet[15];
    return ipaddr.str();
}

std::string IPPacket::IpAddrDestFromPacket(std::vector<uint8_t> const &packet) {
    std::stringstream ipaddr;
    ipaddr << (int)packet[16] << "." << (int)packet[17] << "." << (int)packet[18] << "." << (int)packet[19];
    return ipaddr.str();
}