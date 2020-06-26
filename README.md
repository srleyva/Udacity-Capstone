# CPPND: ICMP Implementation

This is designed to implement the ICMP protocol (not in its entirety) in C++. This utilizes the tun/tap Linus drivers to create virtual interfaces on network (L3) layer. It parses raw network bytes into C++ structs. I left this open to implement the full ICMP, as well as TCP and UDP. I eventually plan to implement the full network stack jsut for fun.

Requirements:

* Respond to one ping from `ping` c util.

## Dependencies for Running Locally
* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* libviface >= 1.1.0
  * Linux:
     ```bash
     $ git clone https://github.com/HPENetworking/libviface.git
     $ mkdir build & cd build
     $ make install
     ```

## Basic Build Instructions

1. Clone and build the libviface repo.
```
git clone https://github.com/HPENetworking/libviface.git
mkdir build & cd build
make install
```

2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `sudo ./NetworkImpl`. Needs sudo as it needs `cap_net_bind_service` to set up virtual interfaces.
5. In a seperate terminal ping an address in the subnet `10.0.0.0/16`

```
ping -c 1 10.0.0.10
```