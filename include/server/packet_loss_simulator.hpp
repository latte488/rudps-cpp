#define TEST
#ifndef SERVER_PACKET_LOSS_SIMULATOR_HPP
#define SERVER_PACKET_LOSS_SIMULATOR_HPP

#include <random>
#include <algorithm>

#include <server/udp.hpp>

namespace server
{

class PacketLossSimulator : public IPacketUpdaterAndIPacketSender
{
private:
    UDP m_udp;
    std::mt19937 m_rand;
    uint_fast32_t m_percent;
public:
    explicit PacketLossSimulator(const uint16_t port, uint_fast32_t percent) noexcept
        : m_udp {port}
        , m_rand {48}
        , m_percent {std::min(std::max((uint_fast32_t)0, percent), (uint_fast32_t)100)}
    {
        
    }

    void RecvUpdate(IPacketReceiver& packet_receiver) noexcept override
    {
        m_udp.RecvUpdate(packet_receiver);
    }
    
    void SendUpdate() noexcept override
    {
        m_udp.SendUpdate();
    }

    void Send(std::unique_ptr<SendPacket>&& send_packet_ptr) noexcept override
    {
        if (m_rand() % (uint_fast32_t)100 < m_percent)
        {
            m_udp.Send(std::move(send_packet_ptr));
        }
    }
};

#ifdef TEST

int test_packet_loss_simulator()
{
    PacketLossSimulator udp(53548, 30);
    TestEchoUpdater updater(udp);

    return 0;
}

#endif

}

#endif

