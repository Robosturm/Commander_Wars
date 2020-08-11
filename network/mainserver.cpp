#include "mainserver.h"

#include "coreengine/settings.h"
#include "coreengine/filesupport.h"
#include "coreengine/mainapp.h"

#include "multiplayer/networkcommands.h"

MainServer* MainServer::m_pInstance = nullptr;

MainServer* MainServer::getInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new MainServer();
    }
    return m_pInstance;
}

MainServer::MainServer()
    : QObject()
{
    Interpreter::setCppOwnerShip(this);
    moveToThread(Mainapp::getGameServerThread());
    m_pGameServer = new TCPServer();
    connect(m_pGameServer, &TCPServer::recieveData, this, &MainServer::recieveData, Qt::QueuedConnection);
    emit m_pGameServer->sig_connect("", Settings::getServerPort());
}

MainServer::~MainServer()
{
    m_pGameServer->sig_close();
    // clean up server and client games.
    for (qint32 i = 0; i < m_games.size(); i++)
    {
        m_games[i]->game.disconnect();
        m_games[i]->process->kill();
        m_games[i]->m_runner.terminate();
        delete m_games[i]->process;
        delete m_games[i];
    }
}

void MainServer::recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service)
{
    if (service == NetworkInterface::NetworkSerives::ServerHosting)
    {
        QDataStream stream(&data, QIODevice::ReadOnly);
        QString messageType;
        stream >> messageType;
        Console::print("Network Server Command received: " + messageType, Console::eDEBUG);
        if (messageType == NetworkCommands::LAUNCHGAMEONSERVER)
        {
            spawnSlaveGame(stream, socketID, data);
        }
    }
}

void MainServer::spawnSlaveGame(QDataStream & stream, quint64 socketID, QByteArray& data)
{
    QStringList mods;
    mods = Filesupport::readVectorList<QString, QList>(stream);
    if (validHostRequest(mods))
    {
        m_slaveGameIterator++;
        QString slaveName = "Commander_Wars_Slave_" + QString::number(m_slaveGameIterator);
        m_games.append(new stNetworkGame());
        m_networkGames.append(new NetworkGameData());
        qint32 pos = m_games.size() - 1;
        QString program = "Commander_Wars.exe";
        m_games[pos]->process = new QProcess();
        m_networkGames[pos]->setSlaveName(slaveName);
        QStringList args;
        args << "-slave";
        //args << "-noui";
        args << "-slaveServer";
        args << slaveName;
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
        m_games[pos]->game.addClient(m_pGameServer->getClient(socketID));
        m_games[pos]->process->start(program, args);
    }
    else
    {
        Console::print("Requested invalid mod configuration.", Console::eDEBUG);
        // todo send request denial
    }
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
