#pragma once

#include <QObject>

#include "3rd_party/smtpClient/src/SmtpMime"
#include "multiplayer/networkcommands.h"

class SmtpMailSender final : public QObject
{
    Q_OBJECT
public:
    explicit SmtpMailSender(QObject *parent = nullptr);
    ~SmtpMailSender() = default;

signals:
    void sigSendMail(quint64 socketId, const QString & subject, const QString & content, const QString & receiverAddress, const QString & username, NetworkCommands::PublicKeyActions action);
    void sigMailResult(quint64 socketId, const QString & receiverAddress, const QString & username, bool result, NetworkCommands::PublicKeyActions action);
private slots:
    void sendMail(quint64 socketId, const QString & subject, const QString & content, const QString & receiverAddress, const QString & username, NetworkCommands::PublicKeyActions action);
private:
    bool connectToServer(SmtpClient & client);
private:
};

