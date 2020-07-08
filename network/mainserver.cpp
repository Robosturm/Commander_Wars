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
    emit m_pGameServer->sig_connect("", Settings::getServerPort());
}

MainServer::~MainServer()
{
    m_pGameServer->sig_close();
    // clean up server and client games.
    for (qint32 i = 0; i < m_Processes.size(); i++)
    {
        m_Client[i]->disconnectTCP();
        m_Processes[i]->kill();
        delete m_Client[i];
        delete m_Processes[i];
    }
}

void MainServer::recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service)
{
    if (service == NetworkInterface::NetworkSerives::ServerHosting)
    {
        QDataStream stream(&data, QIODevice::ReadOnly);
        QString messageType;
        stream >> messageType;
        if (messageType == NetworkCommands::LAUNCHGAMEONSERVER)
        {
            spawnSlaveGame(stream, socketID);
        }
    }
}

void MainServer::spawnSlaveGame(QDataStream & stream, quint64 socketID)
{
    QStringList mods;
    mods = Filesupport::readVectorList<QString, QList>(stream);
    if (validHostRequest(mods))
    {
        quint16 port = getNextPort();
        m_Processes.append(new QProcess());
        m_networkGames.append(new NetworkGameData());
        qint32 pos = m_Processes.size() - 1;
        QString program = "Commander_Wars.exe";
        m_networkGames[pos]->setGamePort(port);
        QStringList args;
        args << "-slave";
        args << "-noui";
        args << "-slaveport";
        args << QString::number(port);
        m_Processes[pos]->start(program, args);
    }
    else
    {

    }
}

quint16 MainServer::getNextPort()
{
    for (quint16 i = Settings::getMinGameServerPort(); i < Settings::getMaxGameServerPort(); i++)
    {
        bool found = false;
        for (qint32 i2 = 0; i2 < m_networkGames.size(); i2++)
        {
            if (m_networkGames[i2]->getGamePort() == i)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            return i;
        }
    }
    return 0;
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
