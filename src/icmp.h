#ifndef ICMP_H
#define ICMP_H

#include "payload.h"

class ICMPPacket : public Payload {
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
            // if (_checksum != ICMPPacket::GetChecksum(_rawPayload)) {
            //     std::cout << "Checksum mismatch" << this->_checksum << " " << ICMPPacket::GetChecksum(_rawPayload);
            // }
    }

    std::string String() {
        std::stringstream output;
        output << "ICMP Type=" << this->_type;
        output << " Code=" << (int)this->_code;
        output << " Checksum=" << std::setfill(' ') << std::setw(6) << std::left << this->_checksum;
        return output.str();
    }

    ICMPPacket(ICMPType type, uint8_t code, uint16_t checksum, std::vector<uint8_t> bytes) :
        _type(type),
        _code(code),
        _checksum(checksum),
        _bytes(bytes) {}
    
    std::unique_ptr<Payload> Handle() {
        ICMPType responseType;
        uint8_t responseCode;
        switch (this->_type)
        {
        case ICMPType::EchoRequest:
            responseType = ICMPType::EchoReply;
            responseCode = 0;
            break;
        default:
            break;
        }
        return std::make_unique<ICMPPacket>(responseType, responseCode, this->_checksum, this->_bytes);
    }

private:
    std::vector<uint8_t> _rawPayload;
    ICMPType _type;
    uint8_t _code;
    uint16_t _checksum;
    std::vector<uint8_t> _bytes;
};

#endif