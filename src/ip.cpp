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

uint16_t IPPacket::PortFromPacket(std::vector<uint8_t> const &packet, IPPacket::PortType type) {
    auto payload = IPPacket::PayloadFromPacket(packet);
    auto byteIndex1 = (type == IPPacket::PortType::Src) ? 0 : 2;
    auto byteIndex2 = (type == IPPacket::PortType::Dest) ? 1 : 3;
    return (uint16_t(payload[byteIndex1]) << 8) | uint16_t(payload[byteIndex2]);
}

std::string IPPacket::IpAddrDestFromPacket(std::vector<uint8_t> const &packet) {
    std::stringstream ipaddr;
    ipaddr << (int)packet[16] << "." << (int)packet[17] << "." << (int)packet[18] << "." << (int)packet[19];
    return ipaddr.str();
}

std::vector<uint8_t> IPPacket::PayloadFromPacket(std::vector<uint8_t> const &packet) {
    int ipHeaderLength = packet[0] & 0x0f;
    return std::vector<uint8_t>(packet.begin() + (ipHeaderLength * 4), packet.end());
}

std::vector<uint8_t> IPPacket::ConstructIPBytesFromString(std::string ipString) {
    std::vector<uint8_t> octetBytes;
    std::stringstream ss(ipString);
    std::string token;
    while (std::getline(ss, token, '.')) {
        octetBytes.push_back(std::stoi(token));
    }
    return octetBytes;
}

std::string IPPacket::String() {
        std::stringstream output;
        output << "Src=" << this->_srcAddress << ":" << std::left << std::setfill(' ') << std::setw(5) << this->_srcPort;
        output << "Dest=" << this->_destAddress << ":" << std::left << std::setfill(' ') << std::setw(5) << this->_destPort;
        output << " Payload: " << this->_payload->String();
        return output.str();
}

std::unique_ptr<IPPacket> IPPacket::Handle() {
    auto response = std::make_unique<IPPacket>(
        this->_destAddress, 
        this->_destPort, 
        this->_srcAddress,
        this->_srcPort,
        this->_version,
        this->_proto,
        this->_rawPacket
    );
    response->_payload = std::move(this->_payload->Handle());
    return response;
}
