#ifndef ICMP_H
#define ICMP_H

#include "payload.h"

class ICMPPacket : public PacketPayload {
public:
    // ICMP RFC: https://tools.ietf.org/html/rfc792
    enum ICMPType : uint8_t {
        EchoReply = 0,
        EchoRequest = 8
    };

    // Static methods
    // https://tools.ietf.org/html/rfc792
    uint16_t static GetChecksum(std::vector<uint8_t> rawPayload) {
        std::vector<uint8_t> checksumBytes{rawPayload[0], rawPayload[1], 0, 0};
        checksumBytes.insert(checksumBytes.end(), rawPayload.begin() + 4, rawPayload.end());

        auto checksumCoverage = checksumBytes.size() - 1;
        auto checksum = uint32_t(0);

        for (int i = 0; i < checksumCoverage; i += 2) {
            checksum += uint32_t(checksumBytes[i+1]) << 8 | uint32_t(checksumBytes[i]);
        }

        if (checksumCoverage & 1 == 0) {
            checksum += uint32_t(checksumBytes[checksumCoverage]);
        }

        checksum += checksum >> 16 + (checksum & 0xffff);
        checksum += checksum >> 16;
        return ~checksum;
    }

    ICMPPacket(std::vector<uint8_t> rawPayload) : 
        _rawPayload(rawPayload),
        _type(ICMPType(rawPayload[0])),
        _code(rawPayload[1]),
        _checksum((uint16_t(rawPayload[2]) << 8) | uint16_t(rawPayload[3])),
        _bytes(rawPayload.begin() + 4, rawPayload.end()) {
            // Validate checksum
            if (_checksum != ICMPPacket::GetChecksum(_rawPayload)) {
                throw 6;
            }
    }

    ICMPPacket(ICMPType type, uint8_t code, std::vector<uint8_t> bytes) :
        _type(type),
        _code(code),
        _checksum(ICMPPacket::GetChecksum(bytes)),
        _bytes(bytes) {}
    
    ICMPPacket(ICMPPacket *request) {
        switch (request->_type)
        {
        case ICMPType::EchoRequest:
            this->_type = ICMPType::EchoRequest;
            this->_code = 0;
            break;
        default:
            break;
        }

        this->_bytes = request->_bytes;
    }

private:
    std::vector<uint8_t> _rawPayload;
    ICMPType _type;
    uint8_t _code;
    uint16_t _checksum;
    std::vector<uint8_t> _bytes;
};

#endif