#include "networkinterface.h"


NetworkInterface::NetworkInterface(std::string prefix, int numInterfaces) {
    for (int i = 0; i < numInterfaces; i++) {
        std::stringstream iname;
        iname << prefix << i;
        viface::VIface* iface = new viface::VIface(iname.str(), false);
        std::stringstream addr;
        addr << "10.0." << i << ".0";
        iface->setIPv4(addr.str()); // IPv4
        iface->setIPv4Netmask("255.255.0.0"); // /24 CIDR
        iface->up();
        std::cout << "Interface UP: [" << iface->getName() << "] IP: " << iface->getIPv4() << " Subnet Mask: " << iface->getIPv4Netmask() << std::endl;
        this->_virtualNics.insert(iface);
    }
}

void NetworkInterface::Run() {
    viface::dispatcher_cb dispatchCallback = std::bind(
        &NetworkInterface::handler,
        this,
        std::placeholders::_1,
        std::placeholders::_2,
        std::placeholders::_3
    );
    viface::dispatch(this->_virtualNics, dispatchCallback);
}

bool NetworkInterface::handler(std::string const &name, uint id, std::vector<uint8_t> &packet) {
        IPPacket ipPacket(packet);
        std::cout << "Interface [" << name << "] ";
        std::cout << "Packet [" << std::setfill('0') << std::setw(2) << ++_count << "]" ;
        ipPacket.Handle();
        return true;
}