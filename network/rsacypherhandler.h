#pragma once

#include <QString>
#include <openssl/evp.h>

class RsaCypherHandler final
{
public:
    RsaCypherHandler();
    ~RsaCypherHandler();

    bool encryptRSA(const QString & publicKey, const QString & message, QByteArray & encryptedKey, QByteArray & encrpytedMessage, QByteArray & iv);
    bool decryptRSA(const QByteArray & encryptedKey, const QByteArray & encrpytedMessage, const QByteArray & iv, QString & decryptedMessage);
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
    EVP_PKEY* m_privateKey;
    EVP_PKEY* m_publicKey;
    QString m_publicKeyStr;
    bool m_ready{false};
};

