#pragma once

#include <QObject>

#include "3rd_party/smtpClient/src/SmtpMime"
#include "multiplayer/networkcommands.h"

class SmtpMailSender : public QObject
{
    Q_OBJECT
public:
    explicit SmtpMailSender(QObject *parent = nullptr);
    ~SmtpMailSender();

    bool getConnected() const;

signals:
    void sigSendMail(quint64 socketId, const QString & subject, const QString & content, const QString & receiverAddress, const QString & username, NetworkCommands::PublicKeyActions action);
    void sigMailResult(quint64 socketId, const QString & receiverAddress, const QString & username, bool result, NetworkCommands::PublicKeyActions action);
    void sigConnectToServer();
    void sigDisconnectFromServer();
private slots:
    void sendMail(quint64 socketId, const QString & subject, const QString & content, const QString & receiverAddress, const QString & username, NetworkCommands::PublicKeyActions action);
    void connectToServer();
    void disconnectFromServer();
    void onDisconnectFromMailServer();
private:
    /**
     * @brief m_smtpClient
     */
    SmtpClient* m_smtpClient{nullptr};
    bool m_connected{false};
};

