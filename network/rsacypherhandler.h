#pragma once

#include <QByteArray>
#include "openssl/rsa.h"
#include <openssl/engine.h>

class RsaCypherHandler final
{
public:
    RsaCypherHandler();
    ~RsaCypherHandler();

    QByteArray encryptRSA(QString publicKey, QByteArray &data);
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
private:
    void generateKeys();
    void seedRng();

private:
    using RSA_ptr = std::unique_ptr<RSA, decltype(&::RSA_free)>;
    ENGINE* m_engine{nullptr};
    RSA_ptr m_keys;
    RSA* m_privateKey{nullptr};
    RSA* m_publicKey{nullptr};
    bool m_ready{false};
    static bool m_enginesStarted;
};

