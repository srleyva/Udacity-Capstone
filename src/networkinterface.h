#ifndef NETWORKINTERFACE_H
#define NETWORKINTERFACE_H

#include <csignal>
#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <thread>

#include <unistd.h>

#include <viface/viface.hpp>

#include "ip.h"

class NetworkInterface
{
private:

    int _count = 0;

    std::map<std::string, std::unique_ptr<viface::VIface>> _nicMap;

    // for use with dispactcher
    std::set<viface::VIface*> _virtualNics;

public:
    NetworkInterface(std::string prefix, int numInterfaces);
    void Run();
    bool handler(std::string const &name, uint id, std::vector<uint8_t> &packet);
       
};
#endif