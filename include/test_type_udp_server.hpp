
#ifndef TEST_TYPE_UDP_SERVER_HPP
#define TEST_TYPE_UDP_SERVER_HPP

#define TEST
#ifdef TEST

#include <type_udp.hpp>
#include <test_echo_server.hpp>

struct EchoTypeReceiver : public IReceiverOfTypePacket
{
    ISender& sender;
    explicit EchoTypeReceiver(ISender& sender_) noexcept
        : sender {sender_}
    {

    }

    void Receive(std::unique_ptr<TypeReceivePacket>&& ptr) noexcept
    {
        printf("%u\n", ptr->type);
        printf("%s\n", ptr->data);
        printf("%lu\n", ptr->size);
    
        sender.Send(std::make_unique<TestEchoReply>(std::move(ptr->receive_packet)));
        printf("\n");
    }
};

struct HelloPacket : public SendPacket
{
    std::unique_ptr<ReceivePacket> rp;
    explicit HelloPacket(std::unique_ptr<TypeReceivePacket>&& p) noexcept
        : SendPacket {p->receive_packet->address, 1}
        , rp {std::move(p->receive_packet)}
    {
        static char hello[] = "Hello";
        (*iovs)[0].iov_base = hello;
        (*iovs)[0].iov_len  = sizeof(hello);
    }
};

struct IgnoreHelloTypeReceiver : public IReceiverOfTypePacket
{
    ISender& sender;
    explicit IgnoreHelloTypeReceiver(ISender& sender_) noexcept
        : sender {sender_}
    {
    }

    void Receive(std::unique_ptr<TypeReceivePacket>&& recv_packet_ptr) noexcept
    {
        printf("%u\n", recv_packet_ptr->type);
        printf("%s\n", recv_packet_ptr->data);
        printf("%lu\n", recv_packet_ptr->size);

        sender.Send(std::make_unique<HelloPacket>(std::move(recv_packet_ptr)));
        
        printf("\n");
    }
};

class TestTypeUDPServer
{
private:
    TypeUDP tudp;
public:
    explicit TestTypeUDPServer(IUDP& udp) noexcept
        : tudp {udp}
    {
        tudp.SetReceiver('0', std::make_unique<EchoTypeReceiver>(tudp));
        tudp.SetReceiver('1', std::make_unique<IgnoreHelloTypeReceiver>(tudp));
    
        for (;;)
        {
            tudp.UpdateOfReceivePacket();
            tudp.UpdateOfSendPacket();
        }
    }
};

int test_type_udp_server_test()
{   
    UDP udp;
    udp.Bind(53548);
    TestTypeUDPServer{udp};
    return 0;
}

#endif

#endif

