#include "proxyai.h"


#include "game/gamemap.h"

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
    // todo connect to tcp socket
}

void ProxyAi::serializeObject(QDataStream& )
{
}

void ProxyAi::deserializeObject(QDataStream&)
{
}

void ProxyAi::recieveData(QByteArray data, NetworkInterface::NetworkSerives service)
{
    QMutexLocker locker(&m_ActionMutex);
    if (m_pPlayer == GameMap::getInstance()->getCurrentPlayer())
    {
        if (service == NetworkInterface::NetworkSerives::Game)
        {
            QDataStream stream(data);
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
