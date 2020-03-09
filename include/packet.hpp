
#ifndef PACKET_HPP
#define PACKET_HPP

#include <arpa/inet.h>

#include <memory>
#include <vector>

#include <message.hpp>

struct RecvPacket
{
    sockaddr_in address;
    Message message;
};

struct SendPacket
{
    sockaddr_in& address;
    size_t iovs_size;
    std::unique_ptr<iovec[]> iovs;

    explicit SendPacket(sockaddr_in& address_, size_t iovs_size_) noexcept
        : address {address_}
        , iovs_size {iovs_size_}
        , iovs {std::make_unique<iovec[]>(iovs_size)}
    {
        
    }

    virtual ~SendPacket() {}
};

#endif

