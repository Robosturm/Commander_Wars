#pragma once

#include <QString>
#include <QJsonArray>
#include <QJsonDocument>

#include <openssl/evp.h>

#include "multiplayer/networkcommands.h"

class RsaCypherHandler final
{
public:
    RsaCypherHandler();
    ~RsaCypherHandler();
    RsaCypherHandler(const RsaCypherHandler &) = delete;
    RsaCypherHandler & operator=(const RsaCypherHandler &) = delete;

    bool encryptRSA(const QString & publicKey, const QByteArray & message, QByteArray & encryptedKey, QByteArray & encrpytedMessage, QByteArray & iv);
    bool decryptRSA(const QByteArray & encryptedKey, const QByteArray & encrpytedMessage, const QByteArray & iv, QByteArray & decryptedMessage);
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
    /**
     * @brief getPublicKeyMessage
     */
    QByteArray getPublicKeyMessage(NetworkCommands::PublicKeyActions action) const;
    /**
     * @brief RsaCypherHandler::getRequestKeyMessage
     * @param action
     * @return
     */
    QByteArray getRequestKeyMessage(NetworkCommands::PublicKeyActions action) const;
    /**
     * @brief getDecryptedMessage
     * @param encryptedMessage
     * @return
     */
    QByteArray getDecryptedMessage(const QJsonDocument & encryptedMessage);
    /**
     * @brief getEncryptedMessage
     * @param message
     * @return
     */
    QJsonDocument getEncryptedMessage(const QString & publicKey, NetworkCommands::PublicKeyActions action, const QByteArray & message);
    /**
     * @brief toByteArray
     * @param jsonArray
     * @return
     */
    static QByteArray toByteArray(const QJsonArray & jsonArray);
    /**
     * @brief toJsonArray
     * @param byteArray
     * @return
     */
    static QJsonArray toJsonArray(const QByteArray & byteArray);
private:
    void generateKeys();
    void seedRng() const;

private:
    EVP_PKEY* m_privateKey;
    EVP_PKEY* m_publicKey;
    QString m_publicKeyStr;
    bool m_ready{false};
};

