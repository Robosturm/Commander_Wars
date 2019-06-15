#include "proxyai.h"

#include "game/gamemap.h"
#include "game/player.h"
#include "game/co.h"

#include "QMutexLocker"

ProxyAi::ProxyAi()
    : CoreAI (BaseGameInputIF::AiTypes::ProxyAi)
{
    Interpreter::setCppOwnerShip(this);
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());

}

void ProxyAi::init()
{
    CoreAI::init();
}

void ProxyAi::connectInterface(NetworkInterface* pNetworkInterface)
{
    connect(pNetworkInterface, &NetworkInterface::recieveData, this, &ProxyAi::recieveData, Qt::QueuedConnection);
}

void ProxyAi::serializeObject(QDataStream& stream)
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
            QMutexLocker locker(&m_ActionMutex);
            GameAction* pAction = new GameAction();
            pAction->deserializeObject(stream);
            m_ActionBuffer.append(pAction);
            if (actionRunning == false)
            {
                actionRunning = true;
                GameAction* pAction = m_ActionBuffer.front();
                m_ActionBuffer.pop_front();
                emit performAction(pAction);
            }
        }
    }
}

void ProxyAi::nextAction()
{
    QMutexLocker locker(&m_ActionMutex);
    actionRunning = false;
    if (m_pPlayer == GameMap::getInstance()->getCurrentPlayer())
    {
        if (m_ActionBuffer.size() > 0)
        {
            actionRunning = true;
            GameAction* pAction = m_ActionBuffer.front();
            m_ActionBuffer.pop_front();
            emit performAction(pAction);
        }
    }
}
