#include "coreengine/globalutils.h"
#include "network/filepeer.h"
#include "network/JsonKeys.h"
#include "network/networkInterface.h"
#include "multiplayer/networkcommands.h"

#include <QJsonObject>
#include <QJsonDocument>

FilePeer::FilePeer(NetworkInterface* pNetworkInterface, const QString & filePath, qint64 socketId)
    : m_pNetworkInterface(pNetworkInterface),
    m_filePath(filePath),
    m_file{filePath},
    m_connectSocket{socketId}
{    
}

void FilePeer::startUpload()
{
    m_file.open(QIODevice::ReadOnly);
    sendNextPacket();
}

void FilePeer::startDownload(const QString & command)
{
    m_file.remove();
    m_file.open(QIODevice::WriteOnly);
    QJsonObject sendMessage;
    sendMessage.insert(JsonKeys::JSONKEY_COMMAND, command);
    sendMessage.insert(JsonKeys::JSONKEY_REPLAYFILE, m_filePath);
    QJsonDocument doc(sendMessage);
    CONSOLE_PRINT("Sending command " + doc.object().value(JsonKeys::JSONKEY_COMMAND).toString() + " to socket " + QString::number(m_connectSocket), GameConsole::eDEBUG);
    emit m_pNetworkInterface->sig_sendData(m_connectSocket, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
}

bool FilePeer::sendNextPacket()
{
    static constexpr qint32 PACKAGE_SIZE = 1024 * 30;
    char data[PACKAGE_SIZE];
    auto bytesRead = m_fileStream.readRawData(data, PACKAGE_SIZE);
    QByteArray sendData(data, bytesRead);
    QJsonObject sendMessage;
    sendMessage.insert(JsonKeys::JSONKEY_COMMAND, NetworkCommands::FILEPACKET);
    sendMessage.insert(JsonKeys::JSONKEY_BYTEDATA, GlobalUtils::toJsonArray(sendData));
    sendMessage.insert(JsonKeys::JSONKEY_SENDBYTES, m_file.pos());
    sendMessage.insert(JsonKeys::JSONKEY_AVAILABLEBYTES, m_file.size());
    sendMessage.insert(JsonKeys::JSONKEY_ATEND, m_file.atEnd());
    QJsonDocument doc(sendMessage);
    CONSOLE_PRINT("Sending command " + doc.object().value(JsonKeys::JSONKEY_COMMAND).toString() + " to socket " + QString::number(m_connectSocket), GameConsole::eDEBUG);
    emit m_pNetworkInterface->sig_sendData(m_connectSocket, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    return m_file.atEnd();
}

void FilePeer::receivedPacket(const QJsonObject &objData)
{
    auto data = GlobalUtils::toByteArray(objData.value(JsonKeys::JSONKEY_BYTEDATA).toArray());
    m_fileStream.writeRawData(data.constData(), data.size());
    bool atEnd = objData.value(JsonKeys::JSONKEY_ATEND).toBool();
    emit sigDownloadInfo(objData.value(JsonKeys::JSONKEY_SENDBYTES).toInteger(),
                         objData.value(JsonKeys::JSONKEY_AVAILABLEBYTES).toInteger(),
                         atEnd);
    if (!atEnd)
    {
        QJsonObject sendMessage;
        sendMessage.insert(JsonKeys::JSONKEY_COMMAND, NetworkCommands::REQUESTFILEPACKET);
        QJsonDocument doc(sendMessage);
        CONSOLE_PRINT("Sending command " + doc.object().value(JsonKeys::JSONKEY_COMMAND).toString() + " to socket " + QString::number(m_connectSocket), GameConsole::eDEBUG);
        emit m_pNetworkInterface->sig_sendData(m_connectSocket, doc.toJson(QJsonDocument::Compact), NetworkInterface::NetworkSerives::ServerHostingJson, false);
    }
}
