#include <QBuffer>
#include <QFile>

#include "network/mainserver.h"

#include "coreengine/settings.h"
#include "coreengine/filesupport.h"
#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"

#include "game/gamemap.h"

#include "multiplayer/networkcommands.h"

spMainServer MainServer::m_pInstance;

MainServer* MainServer::getInstance()
{
    if (m_pInstance.get() == nullptr)
    {
        m_pInstance = spMainServer::create();
    }
    return m_pInstance.get();
}

bool MainServer::exists()
{
    return m_pInstance.get() != nullptr;
}

MainServer::MainServer()
    : QObject(),
      m_updateTimer(this),
      m_pGameServer(spTCPServer::create(this))
{
    CONSOLE_PRINT("Game server launched", Console::eDEBUG);
    Interpreter::setCppOwnerShip(this);
    m_updateTimer.setSingleShot(true);
    m_updateTimer.start(5000);
    moveToThread(Mainapp::getGameServerThread());

    QString javascriptName = "mainServer";
    Interpreter* pInterpreter = Interpreter::getInstance();
    pInterpreter->setGlobal(javascriptName, pInterpreter->newQObject(this));

    connect(m_pGameServer.get(), &TCPServer::recieveData, this, &MainServer::recieveData, Qt::QueuedConnection);
    connect(m_pGameServer.get(), &TCPServer::sigConnected, this, &MainServer::playerJoined, Qt::QueuedConnection);
    connect(this, &MainServer::sigRemoveGame, this, &MainServer::removeGame, Qt::QueuedConnection);
    connect(this, &MainServer::sigStartRemoteGame, this, &MainServer::slotStartRemoteGame, Qt::QueuedConnection);
    connect(&m_updateTimer, &QTimer::timeout, this, &MainServer::sendGameDataUpdate, Qt::QueuedConnection);
    emit m_pGameServer->sig_connect("", Settings::getServerPort());
}

MainServer::~MainServer()
{
    m_pGameServer->disconnectTCP();
    // clean up server and client games.
    for (qint32 i = 0; i < m_games.size(); i++)
    {
        m_games[i]->game.disconnect();
        m_games[i]->process->kill();
        m_games[i]->m_runner.terminate();
        delete m_games[i]->process;
    }
}

void MainServer::recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service)
{
    if (service == NetworkInterface::NetworkSerives::ServerHosting)
    {
        QDataStream stream(&data, QIODevice::ReadOnly);
        QString messageType;
        stream >> messageType;
        CONSOLE_PRINT("Network Server Command received: " + messageType, Console::eDEBUG);
        if (messageType == NetworkCommands::LAUNCHGAMEONSERVER)
        {
            spawnSlaveGame(stream, socketID, data);
        }
        else if (messageType == NetworkCommands::SERVERJOINGAME)
        {
            joinSlaveGame(socketID, stream);
        }
    }
}

void MainServer::joinSlaveGame(quint64 socketID, QDataStream & stream)
{
    bool found = false;
    QString slave;
    stream >> slave;
    CONSOLE_PRINT("Searching for game " + slave + " for socket " + QString::number(socketID) + " to join game.", Console::eDEBUG);
    for (const auto & game : qAsConst(m_games))
    {
        if (game->game.getServerName() == slave)
        {
            if (game->game.getSlaveRunning() &&
                 !game->game.getData().getLaunched())
            {
                game->game.addClient(m_pGameServer->getClient(socketID));
                connect(&(game->game), &NetworkGame::sigDisconnectSocket, m_pGameServer.get(), &TCPServer::disconnectClient, Qt::QueuedConnection);
            }
            found = true;
            break;
        }
    }
    if (!found)
    {
        CONSOLE_PRINT("Failed to find game " + slave + " for socket " + QString::number(socketID) + " to join game. Forcing a disconnection.", Console::eDEBUG);
        m_pGameServer->disconnectClient(socketID);
    }
}

void MainServer::startRemoteGame(const QString & initScript, const QString & id)
{
    emit sigStartRemoteGame(initScript, id);
}

void MainServer::slotStartRemoteGame(QString initScript, QString id)
{
    QByteArray sendData;
    spawnSlave(initScript, Settings::getMods(), id, 0, sendData);
}

void MainServer::spawnSlaveGame(QDataStream & stream, quint64 socketID, QByteArray& data, QString initScript, QString id)
{
    QStringList mods;
    mods = Filesupport::readVectorList<QString, QList>(stream);
    if (validHostRequest(mods))
    {
        spawnSlave(initScript, mods, id, socketID, data);
    }
    else
    {
        CONSOLE_PRINT("Requested invalid mod configuration.", Console::eDEBUG);
        // todo send request denial
    }
}

void MainServer::spawnSlave(const QString & initScript, const QStringList & mods, QString id, quint64 socketID, QByteArray& data)
{
    m_slaveGameIterator++;
    QString slaveName = "Commander_Wars_Slave_" + QString::number(m_slaveGameIterator);
    m_games.append(spInternNetworkGame::create());
    qint32 pos = m_games.size() - 1;
    QString program = "Commander_Wars.exe";
    m_games[pos]->process = new QProcess();
    m_games[pos]->process->setObjectName("Slaveprocess");
    QStringList args;
    args << "-slave";
    args << "-slaveServer";
    args << slaveName;
    args << "-noui"; // comment out for debugging
    args << "-mods";
    args << Settings::getModConfigString(mods);
    args << "-initScript";
    args << initScript;
    QString markername = "temp/" + slaveName + ".marker";
    if (QFile::exists(markername))
    {
        QFile::remove(markername);
    }
    m_games[pos]->game.setDataBuffer(data);
    m_games[pos]->game.setServerName(slaveName);
    m_games[pos]->game.moveToThread(&m_games[pos]->m_runner);
    m_games[pos]->m_runner.start();
    connect(m_games[pos]->process, &QProcess::started, &m_games[pos]->game, &NetworkGame::startAndWaitForInit, Qt::QueuedConnection);
    connect(m_games[pos]->process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), &m_games[pos]->game, &NetworkGame::processFinished, Qt::QueuedConnection);
    connect(&m_games[pos]->game, &NetworkGame::sigDataChanged, this, &MainServer::updateGameData, Qt::QueuedConnection);
    connect(&m_games[pos]->game, &NetworkGame::sigClose, this, &MainServer::closeGame, Qt::QueuedConnection);
    m_games[pos]->game.addClient(m_pGameServer->getClient(socketID));
    m_games[pos]->process->start(program, args);
    m_games[pos]->game.setId(id);
}

bool MainServer::validHostRequest(QStringList mods)
{
    for (qint32 i = 0; i < mods.size(); i++)
    {
        QFile file(mods[i] + "/mod.txt");
        if (!file.exists())
        {
            return false;
        }
    }
    return true;
}

void MainServer::updateGameData()
{
    m_updateGameData = true;
}

void MainServer::sendGameDataUpdate()
{
    if (m_updateGameData)
    {
        m_updateGameData = false;
        sendGameDataToClient(0);
    }
    m_updateTimer.start(5000);
}

void MainServer::playerJoined(qint64 socketId)
{
    sendGameDataToClient(socketId);
}

void MainServer::sendGameDataToClient(qint64 socketId)
{
    QString command = QString(NetworkCommands::SERVERGAMEDATA);
    CONSOLE_PRINT("Sending command " + command, Console::eDEBUG);
    QByteArray block;
    QBuffer buffer(&block);
    buffer.open(QIODevice::WriteOnly);
    QDataStream out(&buffer);
    out << command;
    qint32 sizePos = buffer.pos();
    out << sizePos;
    qint32 count = 0;
    for (const auto & game : qAsConst(m_games))
    {
        if (!game->game.getData().getLaunched() &&
            game->game.getSlaveRunning())
        {
            count++;
            game->game.getData().serializeObject(out);
        }
    }
    buffer.seek(sizePos);
    out << count;
    // send server data to all connected clients
    emit m_pGameServer->sig_sendData(socketId, block, NetworkInterface::NetworkSerives::ServerHosting, false);
}

void MainServer::closeGame(NetworkGame* pGame)
{
    CONSOLE_PRINT("Despawning game: " + pGame->getServerName(), Console::eDEBUG);
    for (qint32 i = 0; i < m_games.size(); i++)
    {
        if (&m_games[i]->game == pGame)
        {
            m_games[i]->game.setSlaveRunning(false);
            m_games[i]->process->kill();
            delete m_games[i]->process;
            connect(&m_games[i]->m_runner, &QThread::finished, [=]()
            {
                emit sigRemoveGame(pGame);
            });
            m_games[i]->m_runner.quit();
            m_updateGameData = true;
            break;
        }
    }
}

void MainServer::removeGame(NetworkGame* pGame)
{

    for (qint32 i2 = 0; i2 < m_games.size(); i2++)
    {
        if (&m_games[i2]->game == pGame)
        {
            while (m_games[i2]->m_runner.isRunning())
            {
                QThread::msleep(1);
            }
            CONSOLE_PRINT("Game has been despawned " + pGame->getServerName(), Console::eDEBUG);
            m_games.removeAt(i2);
            break;
        }
    }
}
