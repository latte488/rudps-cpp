
#ifndef TYPE_DIVIDER_HPP
#define TYPE_DIVIDER_HPP

#include <type_empty_receiver.hpp>

class TypeDivider : public IReceiverOfPacket
{
private:
    std::unique_ptr<IReceiverOfTypePacket> receiver_ptrs[UINT8_MAX + 1];
public:
    explicit TypeDivider() noexcept
        : receiver_ptrs {}
    {
        for (uint8_t i = 0; i < UINT8_MAX; ++i)
        {
            receiver_ptrs[i] = std::make_unique<TypeEmptyReceiver>();
        }
    }

    void SetReceiver(const uint8_t type, std::unique_ptr<IReceiverOfTypePacket>&& receiver_ptr) noexcept
    { 
        receiver_ptrs[type] = std::move(receiver_ptr);
    }

    void Receive(std::unique_ptr<ReceivePacket>&& recv_packet) noexcept override
    {
        auto& receiver = receiver_ptrs[recv_packet->message.data[0]];
        receiver->Receive(std::make_unique<TypeReceivePacket>(std::move(recv_packet)));
    }

    void UpdateOfTypePacket() noexcept
    {
        for (auto& receiver : receiver_ptrs)
        {
            receiver->UpdateOfTypePacket();
        }
    }
};

#endif

