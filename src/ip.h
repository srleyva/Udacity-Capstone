#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <vector>

class IPPacket {
public:
    enum IpVersion {IPv4, IPv6, Unknown};
    enum L4Proto {ICMP = 0x01, TCP = 0x06};

    // Static Method for parsing raw bytes
    IpVersion static GetVersionFromPacket(std::vector<uint8_t> const &rawPacket);
    L4Proto static GetProtoFromPacket(std::vector<uint8_t> const &rawPacket);
    std::string static IpAddrSrcFromPacket(std::vector<uint8_t> const &packet);
    std::string static IpAddrDestFromPacket(std::vector<uint8_t> const &packet);

    // Constructor
    IPPacket(std::vector<uint8_t> rawPacket) : 
        _rawPacket(rawPacket), 
        _version(IPPacket::GetVersionFromPacket(rawPacket)),
        _proto(IPPacket::GetProtoFromPacket(rawPacket)),
        _srcAddress(IPPacket::IpAddrSrcFromPacket(rawPacket)),
        _destAddress(IPPacket::IpAddrDestFromPacket(rawPacket)) {}

    friend std::ostream& operator<<(std::ostream &output, const IPPacket &packet) {
        output << "IP: " << packet._version << " Src: " << packet._srcAddress << " Dest: " << packet._destAddress << " Proto: " << packet._proto;
        return output;
    }

    IpVersion Version() {return this->_version; }
    L4Proto Proto() {return this->_proto; }
    std::string SrcAddress() {return this->_srcAddress;}
    void SetSrcAddress(std::string);
    std::string DestAddress() {return this->_destAddress; }
    void SetDestAddress(std::string);
private:
    std::vector<uint8_t> _rawPacket;
    IpVersion _version;
    L4Proto _proto;
    std::string _srcAddress;
    uint16_t _srcPort;
    std::string _destAddress;
    uint16_t _destport;

    IpVersion getVersion();
    L4Proto setProto();

};