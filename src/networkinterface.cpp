#include "networkinterface.h"


NetworkInterface::NetworkInterface(std::string prefix, int numInterfaces) {
    for (int i = 0; i < numInterfaces; i++) {
        std::stringstream iname;
        iname << prefix << i;
        std::unique_ptr<viface::VIface> iface = std::make_unique<viface::VIface>(iname.str(), false);
        std::stringstream addr;
        addr << "10.0." << i << ".0";
        iface->setIPv4(addr.str()); // IPv4
        iface->setIPv4Netmask("255.255.0.0"); // /24 CIDR
        iface->up();
        std::cout << "Interface UP: [" << iface->getName() << "] IP: " << iface->getIPv4() << " Subnet Mask: " << iface->getIPv4Netmask() << std::endl;
        this->_virtualNics.insert(iface.get());
        this->_nicMap[iname.str()] = std::move(iface);
    }
}

bool NetworkInterface::handler(std::string const &name, uint id, std::vector<uint8_t> &packet) {
        IPPacket ipPacket(packet);
        std::cout << "Interface [" << name << "] " << "Packet [" << std::right << std::setfill('0') << std::setw(2) << ++_count << "]: " << ipPacket << std::endl;
        std::unique_ptr<IPPacket> response = std::move(ipPacket.Handle());
        
        // If TODO Packet no response implemented
        if (response->Raw().size() == 0) {
            std::cout << "Proto Not implemented" << std::endl;
            return true;
        }
        auto responseBytes = response->Raw();
        this->_nicMap[name]->send(responseBytes);
        return true;
}

void NetworkInterface::Run() {
    viface::dispatcher_cb dispatchCallback = std::bind(
        &NetworkInterface::handler,
        this,
        std::placeholders::_1,
        std::placeholders::_2,
        std::placeholders::_3
    );
    std::cout << "Starting Network Interface Controller" << std::endl;
    viface::dispatch(this->_virtualNics, dispatchCallback);
}