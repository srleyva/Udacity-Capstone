#include <iomanip>
#include <sstream>

#include "icmp.h"

std::string ICMPPacket::String() {
        std::stringstream output;
        output << "ICMP Type=" << this->_type;
        output << " Code=" << (int)this->_code;
        output << " Checksum=" << std::setfill(' ') << std::setw(6) << std::left << this->_checksum;
        return output.str();
}

 std::unique_ptr<Payload> ICMPPacket::Handle() {
        ICMPType responseType;
        uint8_t responseCode;
        switch (this->_type)
        {
        case ICMPType::EchoRequest:
            responseType = ICMPType::EchoReply;
            responseCode = 0;
            break;
        case ICMPType::EchoReply:
            responseType = ICMPType::EchoReply;
            responseCode = 0;
            break;
        }
        return std::make_unique<ICMPPacket>(responseType, responseCode, this->_checksum, this->_rawPayload);
    }