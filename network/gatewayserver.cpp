#include "network/gatewayserver.h"
#include "network/tcpgatewayserver.h"
#include "network/mainserver.h"
#include "network/JsonKeys.h"

#include "coreengine/gameconsole.h"

GatewayServer::GatewayServer(MainServer& parent)
    : QObject(&parent),
    m_pParent(parent)
{
    setObjectName("GatewayServer");
    Interpreter::setCppOwnerShip(this);
}

GatewayServer::~GatewayServer()
{
    CONSOLE_PRINT("~GatewayServer", GameConsole::eDEBUG);
}

void GatewayServer::onLaunchGatewayGameOnServer(quint64 socketID, const QJsonObject & objData)
{
    QString slaveAddress;
    QString slaveSecondaryAddress;
    quint16 slavePort;
    if (m_pParent.getNextFreeSlaveAddress(slaveAddress, slavePort, slaveSecondaryAddress))
    {
        spTcpGatewayServer pGatewayServer = MemoryManagement::create<TcpGatewayServer>(this);
        pGatewayServer->connectTCP(slaveAddress, slavePort, slaveSecondaryAddress);
        connect(pGatewayServer.get(), &TcpGatewayServer::recieveData, &m_pParent, &MainServer::receivedSlaveData, Qt::QueuedConnection);
        m_gatewayServers.append(pGatewayServer);
        auto gameData = objData.value(JsonKeys::JSONKEY_GAMEDATA).toObject();
        QString slaveName = "Commander_Wars_Slave_" + QString::number(m_pParent.getNextSlaveGameIterator());
        auto game = MemoryManagement::create<MainServer::InternNetworkGame>();
        game->game = MemoryManagement::create<NetworkGame>(this, slaveName);
        game->game->setHostingSocket(socketID);
        game->game->getData().fromJson(gameData);
        game->game->getData().setSlaveAddress(slaveAddress);
        game->game->getData().setSlaveSecondaryAddress(slaveSecondaryAddress);
        game->game->getData().setSlavePort(slavePort);
        game->game->getData().setSlaveName(slaveName);
        game->slaveName = slaveName;
        m_pParent.setUuidForGame(game->game->getData());
        TcpGatewayServer* gatewayServer = pGatewayServer.get();
        auto* pGame = game.get();
        connect(pGatewayServer.get(), &TcpGatewayServer::sigDisconnected, this, [this, gatewayServer, pGame](quint64 socket)
        {
            if (disconnected(gatewayServer, socket))
            {
                pGame->game->closeGame();
            }
        }, Qt::QueuedConnection);
        connect(game->game.get(), &NetworkGame::sigClose, &m_pParent, &MainServer::closeGame, Qt::QueuedConnection);

        QJsonObject dummy;
        dummy.insert(JsonKeys::JSONKEY_SLAVENAME, slaveName);
        game->game->slaveRunning(dummy, m_pParent.getGameServer());
        m_pParent.addGame(game);
    }
}

bool GatewayServer::disconnected(TcpGatewayServer* tcpGatewayServer, quint64 socket)
{
    bool ret = false;
    if (tcpGatewayServer->getGatewayHost() == socket)
    {
        ret = true;
        tcpGatewayServer->disconnect();
        for (qint32 i = 0; i < m_gatewayServers.size(); ++i)
        {
            if (m_gatewayServers[i].get() == tcpGatewayServer)
            {
                m_gatewayServers.removeAt(i);
                break;
            }
        }
    }
    return ret;
}
