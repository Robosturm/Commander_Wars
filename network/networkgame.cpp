#include <QFile>

#include "network/networkgame.h"

#include "multiplayer/networkcommands.h"

#include "coreengine/filesupport.h"
#include "coreengine/interpreter.h"

#include "game/gamemap.h"

NetworkGame::NetworkGame(QObject* pParent)
    : QObject(pParent),
      m_timer(this)
{
    connect(&m_timer, &QTimer::timeout, this, &NetworkGame::checkServerRunning, Qt::QueuedConnection);
}

void NetworkGame::slaveRunning(QDataStream &stream)
{
    QString description;
    stream >> description;
    bool hasPassword = false;
    stream >> hasPassword;
    m_data.setDescription(description);
    m_data.setLocked(hasPassword);
    if (m_Clients.size() == 1)
    {
        sendPlayerJoined(0);
        m_slaveRunning = true;
    }
    else
    {
        closeGame();
    }
}

void NetworkGame::setSlaveRunning(bool slaveRunning)
{
    m_slaveRunning = slaveRunning;
}

const QString & NetworkGame::getId() const
{
    return m_id;
}

void NetworkGame::setId(QString & id)
{
    m_id = id;
}

bool NetworkGame::getSlaveRunning() const
{
    return m_slaveRunning;
}

const NetworkGameData & NetworkGame::getData() const
{
    return m_data;
}

void NetworkGame::startAndWaitForInit()
{
    m_timer.setSingleShot(false);
    m_timer.start(200);
}

void NetworkGame::checkServerRunning()
{
    QString markername = "temp/" + m_serverName + ".marker";
    if (QFile::exists(markername))
    {
        //

        emit m_gameConnection.sig_connect(m_serverName, 0);

        m_timer.stop();
    }
}

void NetworkGame::onConnectToLocalServer(quint64)
{
    CONSOLE_PRINT("onConnectToLocalServer reading game data", Console::eDEBUG);
    m_data.setSlaveName(m_serverName);
    QDataStream stream(&m_dataBuffer, QIODevice::ReadOnly);
    QString messageType;
    stream >> messageType;
    QStringList mods;
    mods = Filesupport::readVectorList<QString, QList>(stream);
    m_data.setMods(mods);
    GameMap::MapHeaderInfo headerInfo;
    GameMap::readMapHeader(stream, headerInfo);
    m_data.setMapName(headerInfo.m_mapName);
    m_data.setMaxPlayers(headerInfo.m_playerCount);

    emit m_gameConnection.sig_sendData(0, m_dataBuffer, NetworkInterface::NetworkSerives::ServerHosting, false);
    // free buffer after it has been send to the server
    m_dataBuffer.clear();
}

QString NetworkGame::getServerName() const
{
    return m_serverName;
}

void NetworkGame::setServerName(const QString &serverName)
{
    m_serverName = serverName;
}

QByteArray NetworkGame::getDataBuffer() const
{
    return m_dataBuffer;
}

void NetworkGame::setDataBuffer(const QByteArray &dataBuffer)
{
    m_dataBuffer = dataBuffer;
}

void NetworkGame::processFinished(int value, QProcess::ExitStatus)
{
    CONSOLE_PRINT("Networkgame Closing game cause slave game has been terminated.", Console::eDEBUG);
    closeGame();
    Interpreter* pInterpreter = Interpreter::getInstance();
    emit pInterpreter->sigNetworkGameFinished(value - 1, m_id);
}

void NetworkGame::closeGame()
{
    if (!m_closing)
    {
        m_closing = true;
        emit sigClose(this);
    }
}
