#ifndef PACKET_HPP
#define PACKET_HPP

#include <arpa/inet.h>

#include <memory>
#include <vector>

#include <message.hpp>

struct ReceivePacket
{
    sockaddr_in address;
    Message message;
};

struct SendPacket
{
    sockaddr_in& address;
    std::unique_ptr<std::vector<iovec>> iovs;

    explicit SendPacket(sockaddr_in& address_, size_t iovs_size_) noexcept
        : address {address_}
        , iovs {std::make_unique<std::vector<iovec>>(iovs_size_)}
    {
        
    }

    virtual ~SendPacket() {}
};

#endif

