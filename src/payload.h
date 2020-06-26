#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <string>
#include <vector>

class Payload {
public:
    virtual std::unique_ptr<Payload> Handle() = 0;
    virtual std::vector<uint8_t> Raw() = 0;
    virtual std::string String() = 0;
};

class TODOPayload : public Payload {
public:
    TODOPayload(int proto) : _proto(proto) {}
    std::unique_ptr<Payload> Handle() {
        return std::make_unique<TODOPayload>(this->_proto);
    }

    std::vector<uint8_t> Raw() {
        return this->_rawPayload;
    }

    std::string String() {
        std::stringstream output;
        output << "Proto not implemented: " << this->_proto;
        return output.str();
    }
private:
    int _proto;
    std::vector<uint8_t> _rawPayload{0x00};
};

#endif