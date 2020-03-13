
#ifndef DH2048_HPP
#define DH2048_HPP

#include <openssl/dh.h>
#include <openssl/rand.h>

#define DH2048_KEY_SIZE 256

DH *get_dh2048()
{
    static unsigned char dh2048_p[] = 
    {
        0x8D, 0x83, 0x73, 0x41, 0x81, 0x25, 0xD9, 0xEE, 0x53, 0xCC, 0xFB, 0x1E,
        0x7A, 0x7F, 0x10, 0xDC, 0xF2, 0x86, 0xFB, 0xD7, 0x99, 0xCF, 0x04, 0xA2,
        0x00, 0xC4, 0xE5, 0x8E, 0xAD, 0xEC, 0xA1, 0xDE, 0xB7, 0x91, 0xAC, 0x28,
        0xB9, 0xA6, 0xD5, 0xD9, 0x67, 0x8C, 0x1C, 0x37, 0x34, 0x1B, 0x36, 0x86,
        0xA9, 0xED, 0x25, 0x6E, 0xF9, 0xB1, 0x19, 0xF0, 0x2D, 0xE8, 0xE7, 0xD5,
        0xCE, 0xBF, 0x50, 0xA3, 0x75, 0xE1, 0xE7, 0x97, 0x75, 0xDC, 0xCD, 0x8A,
        0x78, 0x87, 0xA7, 0x44, 0xE4, 0xED, 0x79, 0xD7, 0x93, 0xEE, 0xDF, 0x23,
        0xA1, 0xD1, 0x21, 0xF6, 0x68, 0x6E, 0x29, 0x65, 0xCF, 0xD9, 0x5E, 0x71,
        0xEC, 0xDD, 0xB4, 0x47, 0x4E, 0x72, 0x77, 0xDF, 0x47, 0x66, 0xB5, 0xCA,
        0xCB, 0x3D, 0x8D, 0xC0, 0x75, 0x51, 0xBD, 0x58, 0x94, 0xE4, 0xE7, 0x3E,
        0x27, 0xBE, 0x0E, 0xE1, 0xE2, 0xDD, 0x6B, 0xDE, 0xC6, 0x55, 0x34, 0x2D,
        0x3F, 0x81, 0x30, 0x56, 0x1D, 0x89, 0x86, 0xA7, 0xB0, 0xB4, 0x1E, 0x74,
        0xAE, 0x3B, 0xEA, 0x9D, 0xB0, 0xF0, 0x32, 0x44, 0x47, 0x61, 0x5F, 0xA7,
        0x68, 0x03, 0x68, 0xDA, 0xFC, 0x7B, 0x1C, 0xB6, 0x6A, 0x4F, 0xA9, 0x46,
        0xFB, 0x8B, 0xA1, 0x7E, 0xBC, 0x89, 0xEE, 0x40, 0xEA, 0x3C, 0xA6, 0x5F,
        0xE9, 0xBF, 0x8A, 0xB3, 0x52, 0xA2, 0xCA, 0x81, 0xCF, 0x6E, 0xCD, 0xDA,
        0xBD, 0x8F, 0x5E, 0xCD, 0x42, 0x37, 0x28, 0xF5, 0x9A, 0x5A, 0x96, 0x27,
        0x9A, 0x5D, 0xE3, 0xB9, 0x4C, 0xD5, 0x7B, 0xD0, 0x55, 0xE4, 0x3F, 0x97,
        0xE9, 0xE8, 0xAC, 0x63, 0xF0, 0x25, 0x83, 0xF0, 0x8D, 0x52, 0x63, 0x81,
        0x5B, 0x78, 0x90, 0x1D, 0x1A, 0x4E, 0x31, 0x25, 0xDF, 0xE5, 0x1D, 0x7C,
        0x8B, 0x3E, 0xA8, 0x27, 0x84, 0x83, 0x55, 0x90, 0xB9, 0x95, 0xFC, 0x52,
        0x45, 0x60, 0x02, 0x2B,
    };

    static unsigned char dh2048_g[] = 
    {
        0x05,
    };

    DH *dh;

    if ((dh = DH_new()) == NULL) 
    {
        return(NULL);
    }

    DH_set0_pqg
    (
        dh,
        BN_bin2bn(dh2048_p, sizeof(dh2048_p), NULL),
        NULL,
        BN_bin2bn(dh2048_g, sizeof(dh2048_g), NULL)
    );

    if ((DH_get0_p(dh) == NULL) || (DH_get0_g(dh) == NULL))
    {
        DH_free(dh); 
        return(NULL);
    }

    return dh;
}


static const char rand_seed[] = "st48kfb,ep*oa<ue+yt4,em.z@nz*:b{9ha-gfqp_ee1}bv";

void setup_rand_seed() noexcept
{
    RAND_seed(rand_seed, sizeof(rand_seed));
}

class DH2048
{
private:
    DH* m_dh;
public:
    uint8_t public_key[DH2048_KEY_SIZE];
    
    explicit DH2048() noexcept
        : m_dh {get_dh2048()}
    {
        if (m_dh == NULL)
        {
            fprintf(stderr, "failed get_dh2048()\n");
            exit(1);
        }

        if (!DH_generate_key(m_dh))
        {
            fprintf(stderr, "failed DH_generator_key()\n");
            exit(1);
        }

        BN_bn2bin(DH_get0_pub_key(m_dh), public_key);
    }

    void ComputeKey(uint8_t* key, uint8_t* remote_public_key) noexcept
    {
        DH_compute_key(key, BN_bin2bn(remote_public_key, DH2048_KEY_SIZE, NULL), m_dh);
    }

    ~DH2048() noexcept
    {
        DH_free(m_dh);
    }
};

#ifdef TEST

int dh2048_test()
{
    setup_rand_seed();

    DH2048 alice;
    uint8_t alice_common_key[DH2048_KEY_SIZE];

    DH2048 bob;
    uint8_t bob_common_key[DH2048_KEY_SIZE];

    alice.ComputeKey(alice_common_key, bob.public_key);
    bob.ComputeKey(bob_common_key, alice.public_key);

    auto f = [&](const char* name, uint8_t* data)
    {
        printf("%s :\n", name);
        for (size_t i = 0; i < DH2048_KEY_SIZE; ++i)
        {
            printf("0x%02x ", data[i]);
            if ((i + 1) % 16 == 0)
            {
                printf("\n");
            }
        }
        printf("\n");
    };

    f("alice public key", alice.public_key);
    f("bob public key", bob.public_key);
    f("alice common key", alice_common_key);
    f("bob common key", bob_common_key);

    return 0;
}

#endif

#endif

