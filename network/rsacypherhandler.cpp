#include "network/rsacypherhandler.h"

#include <openssl/bn.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include<openssl/err.h>

#include <QRandomGenerator>

#include "coreengine/console.h"

using BIO_MEM_ptr = std::unique_ptr<BIO, decltype(&::BIO_free)>;
using EVP_PKEY_ptr = std::unique_ptr<EVP_PKEY, decltype(&::EVP_PKEY_free)>;
using EVP_PKEY_CTX_ptr = std::unique_ptr<EVP_PKEY_CTX , decltype(&::EVP_PKEY_CTX_free)>;

RsaCypherHandler::RsaCypherHandler()
    : m_keys(RSA_new(), ::RSA_free),
      m_rsaEncryptContext(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free),
      m_rsaDecryptContext(EVP_CIPHER_CTX_new(), ::EVP_CIPHER_CTX_free)
{
    ERR_load_CRYPTO_strings();
    seedRng();
    generateKeys();
}

RsaCypherHandler::~RsaCypherHandler()
{
    RSA_free(m_privateKey);
    RSA_free(m_publicKey);
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

void RsaCypherHandler::seedRng() const
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

bool RsaCypherHandler::encryptRSA(const QString & publicKey, const QString & message, QString & encryptedKey, QString & encrpytedMessage, QString & iv)
{
    QByteArray arrPublicKey = publicKey.toLatin1();
    size_t keyLength = arrPublicKey.length();
    const unsigned char* publicKeyData = reinterpret_cast<const unsigned char*>(arrPublicKey.constData());
    bool success = false;
    BIO_MEM_ptr publicBio(BIO_new(BIO_s_mem()), ::BIO_free);
    if (BIO_write(publicBio.get(), publicKeyData, keyLength) > 0)
    {
        EVP_PKEY* key = PEM_read_bio_PUBKEY(publicBio.get(), nullptr, nullptr, nullptr);
        if (key != nullptr)
        {
            qint32 blockLength = 0;
            qint32 encryptedMessageLength = 0;
            qint32 encryptedKeyLength = 0;
            const unsigned char* inData = reinterpret_cast<const unsigned char*>(message.constData());
            qint32 inLength = message.length();
            unsigned char* internalEncryptedKey = static_cast<unsigned char*>(malloc(EVP_PKEY_size(key)));
            unsigned char* internalIv = static_cast<unsigned char*>(malloc(EVP_MAX_IV_LENGTH));
            unsigned char* internalEncryptedMessage = static_cast<unsigned char*>(malloc(inLength + EVP_MAX_IV_LENGTH));
            if(EVP_SealInit(m_rsaEncryptContext.get(), EVP_aes_256_cbc(), &internalEncryptedKey, &encryptedKeyLength, internalIv, &key, 1) > 0)
            {
                if(EVP_SealUpdate(m_rsaEncryptContext.get(), internalEncryptedMessage + encryptedMessageLength, &blockLength, inData, inLength) > 0)
                {
                    encryptedMessageLength += blockLength;
                    if(!EVP_SealFinal(m_rsaEncryptContext.get(), internalEncryptedMessage + encryptedMessageLength, &blockLength) > 0)
                    {
                        success = true;
                    }
                }
            }
            free(internalEncryptedKey);
            free(internalIv);
            free(internalEncryptedMessage);
            EVP_PKEY_free(key);
        }
    }
    if(!success)
    {
        printLastError();
    }
    return success;
}

void RsaCypherHandler::printLastError() const
{
    char errorMsg[512];
    ERR_error_string(ERR_get_error(), errorMsg);
    CONSOLE_PRINT(QString("Could not encyrpt data") + errorMsg, Console::eLogLevels::eWARNING);
}

QByteArray RsaCypherHandler::decryptRSA(QByteArray &data)
{
    QByteArray buffer;
//    const unsigned char* inData = reinterpret_cast<const unsigned char*>(data.constData());
//    size_t inLength = data.length();
//    size_t outLength = 0;
//    bool success = false;
//    EVP_PKEY_ptr key(EVP_PKEY_new(), ::EVP_PKEY_free);
//    if (EVP_PKEY_set1_RSA(key.get(), m_privateKey) > 0)
//    {
//        EVP_PKEY_CTX_ptr ctx(EVP_PKEY_CTX_new(key.get(), m_engine), ::EVP_PKEY_CTX_free);
//        if (ctx.get() != nullptr)
//        {
//            if (EVP_PKEY_decrypt_init(ctx.get()) > 0 &&
//                EVP_PKEY_CTX_set_rsa_padding(ctx.get(), RSA_PKCS1_OAEP_PADDING) > 0 &&
//                EVP_PKEY_decrypt(ctx.get(), nullptr, &outLength, inData, inLength) > 0)
//            {
//                unsigned char* outData = static_cast<unsigned char*>(OPENSSL_malloc(outLength));
//                if (outData != nullptr)
//                {
//                    if (EVP_PKEY_decrypt(ctx.get(), outData, &outLength, inData, inLength) > 0)
//                    {
//                        buffer = QByteArray(reinterpret_cast<char*>(outData), outLength);
//                        OPENSSL_free(outData);
//                        success = true;
//                    }
//                }
//            }
//        }
//    }
//    if(!success)
//    {
//        char errorMsg[512];
//        ERR_error_string(ERR_get_error(), errorMsg);
//        CONSOLE_PRINT(QString("Could not decyrpt data") + errorMsg, Console::eLogLevels::eWARNING);
//    }
    return buffer;
}
