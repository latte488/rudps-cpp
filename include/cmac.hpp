
#ifndef CMAC_HPP
#define CMAC_HPP

#include <openssl/cmac.h>

#define CMAC_KEY_SIZE 16
#define CMAC_MAC_SIZE 16

class CMAC
{
private:
    CMAC_CTX* m_cmac_ctx;
    
public:

    explicit CMAC() noexcept
        : m_cmac_ctx {CMAC_CTX_new()}
    {}

    bool Init(uint8_t* key) noexcept
    {
        return CMAC_Init(m_cmac_ctx, key, CMAC_KEY_SIZE, EVP_aes_128_cbc(), NULL) == 1;
    }

    bool Update(void* message, size_t size) noexcept
    {
        return CMAC_Update(m_cmac_ctx, message, size) == 1;
    }

    template<class T>
    bool Update(T& message) noexcept
    {
        return Update(&message, sizeof(T));
    }


    bool Final(uint8_t* mac) noexcept
    {
        size_t size;
        return CMAC_Final(m_cmac_ctx, mac, &size) == 1;
    }

    ~CMAC() noexcept
    {
        CMAC_CTX_free(m_cmac_ctx);
    }
};


#ifdef TEST

#include <string.h>

int cmac_test()
{
    CMAC cmac;
    static uint8_t key[CMAC_KEY_SIZE] = 
    {
        0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,
        0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c,
    };
    static uint8_t message[] = "saigo no negai wo kiiteokure.";
    static uint8_t mac[CMAC_MAC_SIZE];


    if (!cmac.Init(key))
    {
        fprintf(stderr, "failed CMAC::Init()\n");
        return 1;
    }
    if (!cmac.Update(message, sizeof(message)))
    {    
        fprintf(stderr, "failed CMAC::Update()\n");
        return 1;
    }
    if (!cmac.Final(mac))
    {
        fprintf(stderr, "failed CMAC::Final()\n");
        return 1;
    }

    printf("mac : ");
    for (size_t i = 0; i < CMAC_MAC_SIZE; ++i)
    {
        printf("%02x ", mac[i]);
    }
    printf("\n");

    return 0;
}

#endif

#endif


