#include "network/rsakeygenerator.h"

#include "openssl/rsa.h"
#include "openssl/bn.h"
#include "openssl/evp.h"
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <QRandomGenerator>

bool RsaKeyGenerator::m_seeded = false;

QByteArray RsaKeyGenerator::generatePrivateKey(bool & success)
{
    using BN_ptr = std::unique_ptr<BIGNUM, decltype(&::BN_free)>;
    using RSA_ptr = std::unique_ptr<RSA, decltype(&::RSA_free)>;
    using EVP_PKEY_ptr = std::unique_ptr<EVP_PKEY, decltype(&::EVP_PKEY_free)>;
    using BIO_MEM_ptr = std::unique_ptr<BIO, decltype(&::BIO_free)>;
    constexpr int bits = 2048;
    if (!m_seeded)
    {
        seedRng();
    }
    success = false;
    QByteArray data;
    RSA_ptr keys(RSA_new(), ::RSA_free);
    BN_ptr bn(BN_new(), ::BN_free);
    BN_set_word(bn.get(), RSA_F4);
    auto rc = RSA_generate_key_ex(keys.get(), bits, bn.get(), nullptr);
    if (rc == 1)
    {
        BIO_MEM_ptr privateBio(BIO_new(BIO_s_mem()), ::BIO_free);
        PEM_write_bio_RSAPrivateKey(privateBio.get(), keys.get(), nullptr, nullptr, 0, nullptr, nullptr);
        auto privateKeyLength = BIO_pending(privateBio.get());
        char privateKey[2048];
        BIO_read(privateBio.get(), privateKey, privateKeyLength);
        privateKey[privateKeyLength] = '\0';
        data = QByteArray(privateKey);
        success = true;
    }
    return data;
}

void RsaKeyGenerator::seedRng()
{
    QRandomGenerator rng = QRandomGenerator::securelySeeded();
    constexpr int size = 64;
    quint8 buf[size];
    for (qint32 i = 0; i < size; ++i)
    {
        buf[i] = rng.bounded(0, 256);
    }
    RAND_seed(buf, size);
    m_seeded = true;
}
