#include <chrono>
#include <thread>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <viface/viface.hpp>

#include "ip.h"

class MyDispatcher
{
private:
    int count = 0;

public:
    bool handler(std::string const &name, uint id, std::vector<uint8_t> &packet) {
        IPPacket ipPacket(packet);
        if (ipPacket.Version() == IPPacket::IpVersion::IPv4) {
            if (ipPacket.Proto() == IPPacket::L4Proto::ICMP) {
                std::cout << "IPV4 [ICMP]: [" << ipPacket << "]" << std::endl;
            }
        }
        return true;
    }
       
};

int main(int argc, const char* argv[])
{
    viface::VIface iface("tap0", false, 1);
    iface.setIPv4("10.0.0.1"); // IPv4
    iface.setIPv4Netmask("255.255.0.0"); // /24 CIDR
    iface.up();

    std::set<viface::VIface*> myifaces = {&iface};

    MyDispatcher printer;
    viface::dispatcher_cb mycb = std::bind(
        &MyDispatcher::handler,
        &printer,
        std::placeholders::_1,
        std::placeholders::_2,
        std::placeholders::_3
    );

    std::cout << "calling dispatch: " << iface.getIPv4() << std::endl;
    viface::dispatch(myifaces, mycb);

}