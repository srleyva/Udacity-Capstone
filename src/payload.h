#ifndef PAYLOAD_H
#define PAYLOAD_H

class PacketPayload {
    virtual void handle() = 0;
};

#endif