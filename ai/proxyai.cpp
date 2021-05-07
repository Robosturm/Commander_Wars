#include "ai/proxyai.h"

#include "game/gamemap.h"
#include "game/player.h"
#include "game/co.h"

#include "QMutexLocker"

ProxyAi::ProxyAi()
    : CoreAI (GameEnums::AiTypes_ProxyAi)
{
    setObjectName("ProxyAi");
    Interpreter::setCppOwnerShip(this);
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());

}

void ProxyAi::init()
{
    CoreAI::init();
}

void ProxyAi::readIni(QString)
{
}

void ProxyAi::connectInterface(NetworkInterface* pNetworkInterface)
{
    connect(pNetworkInterface, &NetworkInterface::recieveData, this, &ProxyAi::recieveData, Qt::QueuedConnection);
}

void ProxyAi::serializeObject(QDataStream& stream) const
{
    stream << getVersion();
}
void ProxyAi::deserializeObject(QDataStream& stream)
{
    qint32 version;
    stream >> version;
}

void ProxyAi::recieveData(quint64, QByteArray data, NetworkInterface::NetworkSerives service)
{
    if (service == NetworkInterface::NetworkSerives::Game)
    {
        QDataStream stream(data);
        qint32 player = 0;
        stream >> player;
        if (m_pPlayer->getPlayerID() == player)
        {
            Console::print("Received action from network for player " + QString::number(player), Console::eDEBUG);
            QMutexLocker locker(&m_ActionMutex);
            spGameAction pAction = spGameAction::create();
            pAction->deserializeObject(stream);
            m_ActionBuffer.append(pAction);
            if (m_actionRunning == false &&
                m_pPlayer == GameMap::getInstance()->getCurrentPlayer())
            {
                m_actionRunning = true;
                spGameAction pAction = m_ActionBuffer.front();
                m_ActionBuffer.pop_front();
                emit performAction(pAction);
            }
        }
    }
}

void ProxyAi::nextAction()
{
    QMutexLocker locker(&m_ActionMutex);
    m_actionRunning = false;
    if (m_pPlayer == GameMap::getInstance()->getCurrentPlayer())
    {
        if (m_ActionBuffer.size() > 0)
        {
            m_actionRunning = true;
            spGameAction pAction = m_ActionBuffer.front();
            m_ActionBuffer.pop_front();
            emit performAction(pAction);
        }
    }
}
