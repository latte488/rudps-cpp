
#ifndef PACKET_LOSS_SIMULATOR_HPP
#define PACKET_LOSS_SIMULATOR_HPP

#include <random>
#include <algorithm>

#include <udp.hpp>

class PacketLossSimulator : public IUDP
{
private:
    UDP m_udp;
    std::mt19937 m_rand;
    uint_fast32_t m_percent;
public:
    explicit PacketLossSimulator(uint_fast32_t percent) noexcept
        : m_udp {}
        , m_rand {48}
        , m_percent {std::min(std::max((uint_fast32_t)0, percent), (uint_fast32_t)100)}
    {
        
    }

    void Bind(const uint16_t port) noexcept
    {
        m_udp.Bind(port);
    }

    void UpdateOfReceivePacket(IReceiverOfPacket& receiver) noexcept override
    {
        m_udp.UpdateOfReceivePacket(receiver);
    }
    
    void UpdateOfSendPacket() noexcept override
    {
        m_udp.UpdateOfSendPacket();
    }

    void Send(std::unique_ptr<SendPacket>&& send_packet_ptr) noexcept override
    {
        if (m_rand() % (uint_fast32_t)100 < m_percent)
        {
            m_udp.Send(std::move(send_packet_ptr));
        }
    }
};

#define TEST
#ifdef TEST

#include <test_echo_client.hpp>

int packet_loss_simulator_test()
{
    PacketLossSimulator simulator(20);
    TestEchoClient{simulator};

    return 0;
}

#endif

#endif

