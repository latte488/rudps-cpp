
#ifndef RUDPS_CONTACT_HPP
#define RUDPS_CONTACT_HPP

#include <openssl/rand.h>

#include <dh2048.hpp>
#include <interface.hpp>
#include <config.hpp>

#define RUDPS_CONTACT_NONCE_SIZE    12

#define RUDPS_CONTACT_REQUEST_SIZE  \
(                                   \
    RUDPS_TYPE_SIZE                 \
)

#define RUDPS_CONTACT_REPLY_SIZE    \
(                                   \
    RUDPS_TYPE_SIZE                 \
+   RUDPS_CONTACT_NONCE_SIZE        \
+   DH2048_KEY_SIZE                 \
)

struct RUDPSContactRequest : public SendPacket
{
    explicit RUDPSContactRequest
    (
        sockaddr_in& address, 
        uint8_t& type
    ) noexcept
        : SendPacket {address, 1}
    {
        type = RUDPS_TYPE_CONTACT;
        (*iovs)[0].iov_base = &type;
        (*iovs)[0].iov_len  = sizeof(type);
    }
};

struct RUDPSContactReply : public SendPacket
{
    std::unique_ptr<TypeReceivePacket> receive_packet_ptr;

    explicit RUDPSContactReply
    (
        std::unique_ptr<TypeReceivePacket>&& recv_ptr,
        uint8_t& type,
        uint8_t* nonce,
        uint8_t* public_key
    ) noexcept
        : SendPacket {recv_ptr->address, 3}
        , receive_packet_ptr {std::move(recv_ptr)}
    {
        if (RAND_bytes(nonce, RUDPS_CONTACT_NONCE_SIZE))
        {
            auto& ios = *iovs;
            ios[0].iov_base = &type;
            ios[0].iov_len  = sizeof(type);
            ios[1].iov_base = nonce;
            ios[1].iov_len  = RUDPS_CONTACT_NONCE_SIZE;
            ios[2].iov_base = public_key;
            ios[2].iov_len  = DH2048_KEY_SIZE;
        }
    }
};

#endif

