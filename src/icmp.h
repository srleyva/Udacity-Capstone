#include "ip.h"

class ICMPPacket {
public:
    ICMPPacket(std::unique_ptr<IPPacket> ipPacket) : _ippacket(std::move(ipPacket)) {}

private:
    std::unique_ptr<IPPacket> _ippacket;
};