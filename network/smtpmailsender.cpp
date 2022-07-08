#include "network/smtpmailsender.h"

#include "coreengine/settings.h"

SmtpMailSender::SmtpMailSender(QObject *parent)
    : QObject{parent}

{
    connect(this, &SmtpMailSender::sigSendMail, this, &SmtpMailSender::sendMail, Qt::QueuedConnection);
    connect(this, &SmtpMailSender::sigConnectToServer, this, &SmtpMailSender::connectToServer, Qt::QueuedConnection);
    connect(this, &SmtpMailSender::sigDisconnectFromServer, this, &SmtpMailSender::disconnectFromServer, Qt::QueuedConnection);
}

SmtpMailSender::~SmtpMailSender()
{
    disconnectFromServer();
}

void SmtpMailSender::disconnectFromServer()
{
    if (m_smtpClient != nullptr)
    {
        m_connected = false;
        m_smtpClient->quit();
        delete m_smtpClient;
        m_smtpClient = nullptr;
    }
}

bool SmtpMailSender::getConnected() const
{
    return m_connected;
}

void SmtpMailSender::connectToServer()
{
    if (m_smtpClient == nullptr)
    {
        m_smtpClient = new SmtpClient(Settings::getMailServerAddress(), Settings::getMailServerPort(), static_cast<SmtpClient::ConnectionType>(Settings::getMailServerConnectionType()));
        connect(m_smtpClient, &SmtpClient::disconnected, this, &SmtpMailSender::connectToServer, Qt::QueuedConnection);
    }
    m_smtpClient->connectToHost();
    if (m_smtpClient->waitForReadyConnected())
    {
        m_smtpClient->login(Settings::getMailServerUsername(), Settings::getMailServerPassword(), static_cast<SmtpClient::AuthMethod>(Settings::getMailServerAuthMethod()));
        if (m_smtpClient->waitForAuthenticated())
        {
            m_connected = true;
            CONSOLE_PRINT("Connect to mail server." , Console::eDEBUG);
        }
        else
        {
            CONSOLE_PRINT("Unable to login to mail server account." , Console::eWARNING);
        }
    }
    else
    {
        CONSOLE_PRINT("Unable to connect to mail server." , Console::eWARNING);
    }
}

void SmtpMailSender::onDisconnectFromMailServer()
{
    m_connected = false;
    if (m_smtpClient != nullptr)
    {
        connectToServer();
    }
}

void SmtpMailSender::sendMail(quint64 socketId, const QString & subject, const QString & content, const QString & receiverAddress, const QString & username)
{
    bool result = false;
    if (m_connected)
    {
        CONSOLE_PRINT("Sending mail to " + receiverAddress , Console::eDEBUG);
        MimeMessage message;
        EmailAddress sender(Settings::getMailServerSendAddress(), "Commander Wars Server Crew");
        message.setSender(sender);
        EmailAddress to(receiverAddress, username);
        message.addRecipient(to);
        message.setSubject(subject);
        MimeText text;
        text.setText(content);
        message.addPart(&text);
        m_smtpClient->sendMail(message);
        result = !m_smtpClient->waitForMailSent();
    }
    emit sigMailResult(socketId, receiverAddress, username, result);
}
