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

    ICMPPacket(std::vector<uint8_t> rawPayload) : 
        _rawPayload(rawPayload),
        _type(ICMPType(rawPayload[0])),
        _code(rawPayload[1]),
        _checksum((uint16_t(rawPayload[2]) << 8) | uint16_t(rawPayload[3])) {}

    std::string String();

    ICMPPacket(ICMPType type, uint8_t code, uint16_t checksum, std::vector<uint8_t> rawPayload) :
        _type(type),
        _code(code),
        _checksum(checksum),
        _rawPayload(rawPayload) {
            this->_rawPayload[0] = uint8_t(this->_type);
            this->_rawPayload[1] = uint8_t(this->_code);
        }
    
    std::vector<uint8_t> Raw() { return this->_rawPayload; }

    std::unique_ptr<Payload> Handle();

private:
    std::vector<uint8_t> _rawPayload;
    ICMPType _type;
    uint8_t _code;
    uint16_t _checksum;
};

#endif