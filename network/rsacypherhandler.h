#pragma once

#include <QByteArray>
#include "openssl/rsa.h"
 #include <openssl/evp.h>

class RsaCypherHandler final
{
public:
    RsaCypherHandler();
    ~RsaCypherHandler();

    bool encryptRSA(const QString & publicKey, const QString & message, QString & encryptedKey, QString & encrpytedMessage, QString & iv);
    QByteArray decryptRSA(QByteArray &data);
    /**
     * @brief getReady Indicates if you can use the handler for crypting or not
     * @return
     */
    bool getReady() const;
    /**
     * @brief getPublicKey
     * @return
     */
    QString getPublicKey() const;
    /**
     * @brief printLastError
     */
    void printLastError() const;
private:
    void generateKeys();
    void seedRng() const;
private:
    using RSA_ptr = std::unique_ptr<RSA, decltype(&::RSA_free)>;
    using EVP_CIPHER_CTX_ptr = std::unique_ptr<EVP_CIPHER_CTX, decltype(&::EVP_CIPHER_CTX_free)>;
    RSA_ptr m_keys;
    EVP_CIPHER_CTX_ptr m_rsaEncryptContext;
    EVP_CIPHER_CTX_ptr m_rsaDecryptContext;
    RSA* m_privateKey{nullptr};
    RSA* m_publicKey{nullptr};
    bool m_ready{false};
};

