#include "3rd_party/smtpClient/src/mimetext.h"

#include "network/smtpmailsender.h"

#include "coreengine/settings.h"
#include "coreengine/interpreter.h"

SmtpMailSender::SmtpMailSender(QObject *parent)
    : QObject{parent}

{
    Interpreter::setCppOwnerShip(this);
    connect(this, &SmtpMailSender::sigSendMail, this, &SmtpMailSender::sendMail, Qt::QueuedConnection);
}

bool SmtpMailSender::connectToServer(SmtpClient & client)
{
    bool success = false;
    CONSOLE_PRINT("Start connecting to mail server.", GameConsole::eDEBUG);
    client.connectToHost();
    if (client.waitForReadyConnected())
    {
        client.login(Settings::getInstance()->getMailServerUsername(), Settings::getInstance()->getMailServerPassword(), static_cast<SmtpClient::AuthMethod>(Settings::getInstance()->getMailServerAuthMethod()));
        if (client.waitForAuthenticated())
        {
            success = true;
            CONSOLE_PRINT("Connect to mail server.", GameConsole::eDEBUG);
        }
        else
        {
            CONSOLE_PRINT("Unable to login to mail server account.", GameConsole::eWARNING);
        }
    }
    else
    {
        CONSOLE_PRINT("Unable to connect to mail server.", GameConsole::eWARNING);
    }
    return success;
}

void SmtpMailSender::sendMail(quint64 socketId, const QString subject, const QString content, const QString receiverAddress, const QString username, NetworkCommands::PublicKeyActions action)
{
    SmtpClient client(Settings::getInstance()->getMailServerAddress(), Settings::getInstance()->getMailServerPort(), static_cast<SmtpClient::ConnectionType>(Settings::getInstance()->getMailServerConnectionType()));
    bool result = false;
    if (connectToServer(client))
    {
        CONSOLE_PRINT("Sending mail to " + receiverAddress , GameConsole::eDEBUG);
        MimeMessage message;
        EmailAddress sender(Settings::getInstance()->getMailServerSendAddress(), "Commander Wars Server Crew");
        message.setSender(sender);
        EmailAddress to(receiverAddress, username);
        message.addRecipient(to);
        message.setSubject(subject);
        MimeText text;
        text.setText(content);
        message.addPart(&text);
        client.sendMail(message);
        result = client.waitForMailSent();
    }
    else
    {
        CONSOLE_PRINT("Sending mail to " + receiverAddress + " failed.", GameConsole::eDEBUG);
    }
    CONSOLE_PRINT("Emitting mail send result", GameConsole::eDEBUG);
    emit sigMailResult(socketId, receiverAddress, username, result, action);
}
