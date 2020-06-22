#ifndef IP_H
#define IP_H

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <vector>

#include "icmp.h"
// #include "payload.h"

class IPPacket {
public:
    enum IpVersion {IPv4, IPv6, Unknown};
    enum L4Proto {ICMP = 0x01, TCP = 0x06};
    enum PortType {Src, Dest};

    // Static Method for parsing raw bytes
    IpVersion static GetVersionFromPacket(std::vector<uint8_t> const &rawPacket);
    L4Proto static GetProtoFromPacket(std::vector<uint8_t> const &rawPacket);
    std::string static IpAddrSrcFromPacket(std::vector<uint8_t> const &packet);
    std::string static IpAddrDestFromPacket(std::vector<uint8_t> const &packet);
    uint16_t static PortFromPacket(std::vector<uint8_t> const &packet, PortType);
    std::vector<uint8_t> static PayloadFromPacket(std::vector<uint8_t> const &packet);

    // Constructor
    IPPacket(std::vector<uint8_t> rawPacket) :
        _rawPacket(rawPacket),
        _version(IPPacket::GetVersionFromPacket(rawPacket)),
        _proto(IPPacket::GetProtoFromPacket(rawPacket)),
        _srcAddress(IPPacket::IpAddrSrcFromPacket(rawPacket)),
        _srcPort(IPPacket::PortFromPacket(rawPacket, PortType::Src)),
        _destAddress(IPPacket::IpAddrDestFromPacket(rawPacket)),
        _destPort(IPPacket::PortFromPacket(rawPacket, IPPacket::PortType::Dest)),
        _rawPayload(IPPacket::PayloadFromPacket(rawPacket)) {
            switch (this->_proto)
            {
            case L4Proto::ICMP:
                // this->_payload = std::make_unique<ICMPPacket>(new ICMPPacket());
                break;
            
            default:
                break;
            }
        }
    
    IPPacket(std::string srcAddr, uint16_t srcPort, std::string destAddr, uint16_t destPort, IpVersion ipVersion, L4Proto proto) : 
        _version(ipVersion),
        _proto(proto),
        _srcAddress(srcAddr),
        _srcPort(srcPort),
        _destAddress(destAddr),
        _destPort(destPort) {
            // TODO: Construct RawBytes
        };

    void Handle() {
        // Construct reponse with L3 concerns
        auto response = IPPacket(
            this->_destAddress, 
            this->_destPort, 
            this->_srcAddress,
            this->_srcPort,
            this->_version,
            this->_proto
        );

        switch (this->_proto)
        {
        case L4Proto::ICMP:
            break;
        
        default:
            break;
        }
    }

    friend std::ostream& operator<<(std::ostream &output, const IPPacket &packet) {
        output << "Src=" << packet._srcAddress << ":" << std::left << std::setfill(' ') << std::setw(5) << packet._srcPort;
        output << "Dest=" << packet._destAddress << ":" << std::left << std::setfill(' ') << std::setw(5) << packet._destPort;
        return output;
    }

    std::vector<uint8_t> Raw() {return this->_rawPacket; }
    IpVersion Version() {return this->_version; }
    L4Proto Proto() {return this->_proto; }
    std::string SrcAddress() {return this->_srcAddress;}
    uint16_t SrcPort() {return this->_srcPort; }
    std::string DestAddress() {return this->_destAddress; }
    uint16_t DestPort() {return this->_destPort; }
    PacketPayload *Payload() { return this->_payload.get(); }
private:
    std::vector<uint8_t> _rawPacket;
    IpVersion _version;
    L4Proto _proto;
    std::string _srcAddress;
    uint16_t _srcPort;
    std::string _destAddress;
    uint16_t _destPort;
    std::unique_ptr<PacketPayload> _payload;
    std::vector<uint8_t> _rawPayload;

    IpVersion getVersion();
    L4Proto setProto();

};

#endif