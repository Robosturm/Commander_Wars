#include "ai/proxyai.h"

#include "game/gamemap.h"
#include "game/player.h"
#include "game/co.h"
#include "menue/gamemenue.h"

#include "QMutexLocker"

ProxyAi::ProxyAi(GameMap* pMap)
    : CoreAI (pMap, GameEnums::AiTypes_ProxyAi)
{
    setObjectName("ProxyAi");
    Interpreter::setCppOwnerShip(this);
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());

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
    stream << static_cast<qint32>(m_ActionBuffer.size());
    for (const auto & action : qAsConst(m_ActionBuffer))
    {
        action->serializeObject(stream);
    }
}
void ProxyAi::deserializeObject(QDataStream& stream)
{
    qint32 version;
    stream >> version;
    if (version > 1)
    {
        qint32 size = 0;
        stream >> size;
        for (qint32 i = 0; i < size; i++)
        {
            spGameAction pAction = spGameAction::create(m_pMap);
            pAction->deserializeObject(stream);
            m_ActionBuffer.append(pAction);
        }
    }
    if (version == 3)
    {
        QString dummy;
        stream >> dummy;
    }
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
            AI_CONSOLE_PRINT("Received action from network for player " + QString::number(player), Console::eDEBUG);
            QMutexLocker locker(&m_ActionMutex);
            spGameAction pAction = spGameAction::create(m_pMap);
            pAction->deserializeObject(stream);
            m_ActionBuffer.append(pAction);
            if (m_pMenu != nullptr &&
                !m_pMenu->getActionRunning() &&
                m_pPlayer == m_pMap->getCurrentPlayer())
            {
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
    if (m_pMenu != nullptr &&
        !m_pMenu->getActionRunning() &&
        m_pPlayer == m_pMap->getCurrentPlayer())
    {
        if (m_ActionBuffer.size() > 0)
        {
            spGameAction pAction = m_ActionBuffer.front();
            m_ActionBuffer.pop_front();
            emit performAction(pAction);
        }
    }
}
