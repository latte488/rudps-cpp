
#ifndef SERVER_PACKET_HPP
#define SERVER_PACKET_HPP

#include <arpa/inet.h>

#include <vector>

#include <message.hpp>

namespace server
{

struct RecvPacket
{
    sockaddr_in address;
    Message message;
};

struct SendPacket
{
    sockaddr_in& address;
    std::vector<iovec> iovs;

    explicit SendPacket(sockaddr_in& address_, size_t iov_size) noexcept
        : address {address_}
        , iovs {iov_size}
    {

    }

    virtual ~SendPacket() {}
};

}

#endif

