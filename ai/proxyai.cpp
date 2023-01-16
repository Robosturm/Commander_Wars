#include "ai/proxyai.h"

#include "coreengine/interpreter.h"

#include "game/gamemap.h"
#include "game/player.h"

#include "menue/gamemenue.h"

#include <QMutexLocker>

ProxyAi::ProxyAi(GameMap* pMap)
    : CoreAI (pMap, GameEnums::AiTypes_ProxyAi, "ProxyAi")
{
#ifdef GRAPHICSUPPORT
    setObjectName("ProxyAi");
#endif
    Interpreter::setCppOwnerShip(this);
    AI_CONSOLE_PRINT("Creating proxy ai", GameConsole::eDEBUG);
}

void ProxyAi::readIni(QString)
{
}


void ProxyAi::init(GameMenue* pMenu)
{
    if (!m_initDone)
    {
        m_initDone = true;
        m_pMenu = pMenu;
        connect(&m_pMenu->getActionPerformer(), &ActionPerformer::sigActionPerformed, this, &CoreAI::nextAction, Qt::QueuedConnection);
        connect(this, &CoreAI::performAction, &m_pMenu->getActionPerformer(), &ActionPerformer::performAction, Qt::QueuedConnection);
    }
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
            CONSOLE_PRINT("Received action from network for player " + QString::number(player), GameConsole::eDEBUG);
            QMutexLocker locker(&m_ActionMutex);
            spGameAction pAction = spGameAction::create(m_pMap);
            pAction->deserializeObject(stream);
            m_ActionBuffer.append(pAction);
            if (m_pMenu != nullptr &&
                !m_pMenu->getActionRunning() &&
                m_pPlayer == m_pMap->getCurrentPlayer())
            {
                spGameAction pAction = m_ActionBuffer.front();
                if (pAction->getSyncCounter() == m_pMenu->getSyncCounter() + 1)
                {
                    m_ActionBuffer.pop_front();
                    AI_CONSOLE_PRINT("Emitting action " + pAction->getActionID() + " for player " + QString::number(m_pPlayer->getPlayerID()) +
                                     " current player is " + QString::number(m_pMap->getCurrentPlayer()->getPlayerID()) +
                                     " with sync counter " + QString::number(pAction->getSyncCounter()), GameConsole::eDEBUG);
                    emit performAction(pAction);
                }
                else
                {
                    AI_CONSOLE_PRINT("Skipping emit action " + pAction->getActionID() + " cause sync counter doesn't match "
                                     "action counter=" + QString::number(pAction->getSyncCounter()) +
                                     " menu counter=" + QString::number(m_pMenu->getSyncCounter() + 1), GameConsole::eDEBUG);
                }
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
            if (pAction->getSyncCounter() == m_pMenu->getSyncCounter() + 1)
            {
                m_ActionBuffer.pop_front();
                AI_CONSOLE_PRINT("Emitting action " + pAction->getActionID() + " for player " + QString::number(m_pPlayer->getPlayerID()) +
                                 " current player is " + QString::number(m_pMap->getCurrentPlayer()->getPlayerID()) +
                                 " with sync counter " + QString::number(pAction->getSyncCounter()), GameConsole::eDEBUG);
                emit performAction(pAction);
            }
            else
            {
                AI_CONSOLE_PRINT("Skipping emit action " + pAction->getActionID() + " cause sync counter doesn't match "
                                 "action counter=" + QString::number(pAction->getSyncCounter()) +
                                 " menu counter=" + QString::number(m_pMenu->getSyncCounter() + 1), GameConsole::eDEBUG);
            }
        }
    }
}
