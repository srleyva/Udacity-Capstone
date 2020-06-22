#include "networkinterface.h"

int main(int argc, const char* argv[])
{
    NetworkInterface networkInterface("dev", 1);
    networkInterface.Run();
}