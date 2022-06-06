#include "network/rsacypherhandler.h"

#include <openssl/bn.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include<openssl/err.h>

#include <QRandomGenerator>

#include "coreengine/console.h"

using BIO_MEM_ptr = std::unique_ptr<BIO, decltype(&::BIO_free)>;
using EVP_PKEY_ptr = std::unique_ptr<EVP_PKEY, decltype(&::EVP_PKEY_free)>;
using EVP_PKEY_CTX_ptr = std::unique_ptr<EVP_PKEY_CTX , decltype(&::EVP_PKEY_CTX_free)>;

bool RsaCypherHandler::m_enginesStarted = false;

RsaCypherHandler::RsaCypherHandler()
    : m_keys(RSA_new(), ::RSA_free)
{
    ERR_load_CRYPTO_strings();
    if (!m_enginesStarted)
    {
        ENGINE_load_builtin_engines();
        m_engine = ENGINE_by_id("dynamic");
        if (m_engine != nullptr &&
            ENGINE_set_default_RSA(m_engine) &&
            ENGINE_set_default_DSA(m_engine) &&
            ENGINE_set_default_ciphers(m_engine))
        {
            m_enginesStarted = true;
        }
    }
    else
    {
        m_engine = ENGINE_by_id("dynamic");
    }
    seedRng();
    generateKeys();
}

RsaCypherHandler::~RsaCypherHandler()
{
    RSA_free(m_privateKey);
    RSA_free(m_publicKey);
    ENGINE_finish(m_engine);
    ENGINE_free(m_engine);
}

void RsaCypherHandler::generateKeys()
{
    using BN_ptr = std::unique_ptr<BIGNUM, decltype(&::BN_free)>;
    constexpr int bits = 2048;
    BN_ptr bn(BN_new(), ::BN_free);
    BN_set_word(bn.get(), RSA_F4);
    auto rc = RSA_generate_key_ex(m_keys.get(), bits, bn.get(), nullptr);
    if (rc == 1)
    {
        BIO_MEM_ptr privateBio(BIO_new(BIO_s_mem()), ::BIO_free);
        PEM_write_bio_RSAPrivateKey(privateBio.get(), m_keys.get(), nullptr, nullptr, 0, nullptr, nullptr);
        m_privateKey = PEM_read_bio_RSAPrivateKey(privateBio.get(), nullptr, nullptr, nullptr);

        BIO_MEM_ptr publicBio(BIO_new(BIO_s_mem()), ::BIO_free);
        PEM_write_bio_RSAPublicKey(publicBio.get(), m_keys.get());
        m_publicKey = PEM_read_bio_RSAPublicKey(publicBio.get(), nullptr, nullptr, nullptr);
        if (m_privateKey != nullptr &&
            m_publicKey != nullptr)
        {
            m_ready = true;
        }
    }
}

QString RsaCypherHandler::getPublicKey() const
{
    BIO_MEM_ptr publicBio(BIO_new(BIO_s_mem()), ::BIO_free);
    PEM_write_bio_RSAPublicKey(publicBio.get(), m_keys.get());

    auto publicKeyLength = BIO_pending(publicBio.get());
    char publicKeyArray[512];
    BIO_read(publicBio.get(), publicKeyArray, publicKeyLength);
    return QString(QByteArray(publicKeyArray));
}

void RsaCypherHandler::seedRng()
{
    QRandomGenerator rng = QRandomGenerator::securelySeeded();
    constexpr int size = 64;
    quint8 buf[size];
    for (qint32 i = 0; i < size; ++i)
    {
        buf[i] = rng.bounded(0, 256);
    }
    RAND_seed(buf, size);
}

bool RsaCypherHandler::getReady() const
{
    return m_ready;
}

QByteArray RsaCypherHandler::encryptRSA(QString publicKey, QByteArray &data)
{
    QByteArray buffer;
    QByteArray arrPublicKey = publicKey.toLatin1();
    size_t inLength = data.length();
    size_t outLength = 0;
    size_t keyLength = arrPublicKey.length();
    const unsigned char* inData = reinterpret_cast<const unsigned char*>(data.constData());
    const unsigned char* publicKeyData = reinterpret_cast<const unsigned char*>(arrPublicKey.constData());
    bool success = false;
    BIO_MEM_ptr publicBio(BIO_new(BIO_s_mem()), ::BIO_free);
    if (BIO_write(publicBio.get(), publicKeyData, keyLength) > 0)
    {
        RSA_ptr publicRsaKey(PEM_read_bio_RSAPublicKey(publicBio.get(), nullptr, nullptr, nullptr), ::RSA_free);
        EVP_PKEY_ptr key(EVP_PKEY_new(), ::EVP_PKEY_free);
        if (EVP_PKEY_set1_RSA(key.get(), publicRsaKey.get()) > 0)
        {
            EVP_PKEY_CTX_ptr ctx(EVP_PKEY_CTX_new(key.get(), m_engine), ::EVP_PKEY_CTX_free);
            if (ctx.get() != nullptr)
            {
                if (EVP_PKEY_encrypt_init(ctx.get()) > 0 &&
                    EVP_PKEY_CTX_set_rsa_padding(ctx.get(), RSA_PKCS1_OAEP_PADDING) > 0 &&
                    EVP_PKEY_encrypt(ctx.get(), nullptr, &outLength, inData, inLength) > 0)
                {
                    unsigned char* outData = static_cast<unsigned char*>(OPENSSL_malloc(outLength));
                    if (outData != nullptr)
                    {
                        if (EVP_PKEY_encrypt(ctx.get(), outData, &outLength, inData, inLength) > 0)
                        {
                            buffer = QByteArray(reinterpret_cast<char*>(outData), outLength);
                            OPENSSL_free(outData);
                            success = true;
                        }
                    }
                }
            }
        }
    }
    if(!success)
    {
        char errorMsg[512];
        ERR_error_string(ERR_get_error(), errorMsg);
        CONSOLE_PRINT(QString("Could not encyrpt data") + errorMsg, Console::eLogLevels::eWARNING);
    }
    return buffer;
}

QByteArray RsaCypherHandler::decryptRSA(QByteArray &data)
{
    QByteArray buffer;
    const unsigned char* inData = reinterpret_cast<const unsigned char*>(data.constData());
    size_t inLength = data.length();
    size_t outLength = 0;
    bool success = false;
    EVP_PKEY_ptr key(EVP_PKEY_new(), ::EVP_PKEY_free);
    if (EVP_PKEY_set1_RSA(key.get(), m_privateKey) > 0)
    {
        EVP_PKEY_CTX_ptr ctx(EVP_PKEY_CTX_new(key.get(), m_engine), ::EVP_PKEY_CTX_free);
        if (ctx.get() != nullptr)
        {
            if (EVP_PKEY_decrypt_init(ctx.get()) > 0 &&
                EVP_PKEY_CTX_set_rsa_padding(ctx.get(), RSA_PKCS1_OAEP_PADDING) > 0 &&
                EVP_PKEY_decrypt(ctx.get(), nullptr, &outLength, inData, inLength) > 0)
            {
                unsigned char* outData = static_cast<unsigned char*>(OPENSSL_malloc(outLength));
                if (outData != nullptr)
                {
                    if (EVP_PKEY_decrypt(ctx.get(), outData, &outLength, inData, inLength) > 0)
                    {
                        buffer = QByteArray(reinterpret_cast<char*>(outData), outLength);
                        OPENSSL_free(outData);
                        success = true;
                    }
                }
            }
        }
    }
    if(!success)
    {
        char errorMsg[512];
        ERR_error_string(ERR_get_error(), errorMsg);
        CONSOLE_PRINT(QString("Could not decyrpt data") + errorMsg, Console::eLogLevels::eWARNING);
    }
    return buffer;
}
