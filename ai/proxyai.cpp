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
    if (m_pPlayer == nullptr)
    {
        CONSOLE_PRINT("Creating proxy ai for player", GameConsole::eDEBUG);
    }
    else
    {
        CONSOLE_PRINT("Creating proxy ai for player " + QString::number(m_pPlayer->getPlayerID()) + " with name " + m_pPlayer->getPlayerNameId(), GameConsole::eDEBUG);
    }
}

void ProxyAi::readIni(QString)
{
}


void ProxyAi::init(GameMenue* pMenu)
{
    if (!m_initDone)
    {
        if (m_pPlayer == nullptr)
        {
            CONSOLE_PRINT("Proxy ai init for player", GameConsole::eDEBUG);
        }
        else
        {
            CONSOLE_PRINT("Proxy ai init for player " + QString::number(m_pPlayer->getPlayerID()) + " with name " + m_pPlayer->getPlayerNameId(), GameConsole::eDEBUG);
        }
        m_initDone = true;
        m_pMenu = pMenu;
        connect(&m_pMenu->getActionPerformer(), &ActionPerformer::sigActionPerformed, this, &CoreAI::nextAction, Qt::QueuedConnection);
        connect(this, &CoreAI::sigPerformAction, &m_pMenu->getActionPerformer(), &ActionPerformer::performAction, Qt::DirectConnection);
    }
}

void ProxyAi::connectInterface(NetworkInterface* pNetworkInterface)
{
    CONSOLE_PRINT("Proxy ai connectInterface for player " + QString::number(m_pPlayer->getPlayerID()) + " with name " + m_pPlayer->getPlayerNameId(), GameConsole::eDEBUG);
    connect(pNetworkInterface, &NetworkInterface::recieveData, this, &ProxyAi::recieveData, NetworkCommands::UNIQUE_DATA_CONNECTION);
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
                if (verifyActionStack())
                {
                    spGameAction pAction = m_ActionBuffer.front();
                    m_ActionBuffer.pop_front();
                    if (pAction->getSyncCounter() == m_pMenu->getSyncCounter() + 1)
                    {
                        AI_CONSOLE_PRINT("Emitting action " + pAction->getActionID() + " for player " + QString::number(m_pPlayer->getPlayerID()) +
                                         " current player is " + QString::number(m_pMap->getCurrentPlayer()->getPlayerID()) +
                                         " with sync counter " + QString::number(pAction->getSyncCounter()), GameConsole::eDEBUG);
                        emit sigPerformAction(pAction);
                    }
                }
                else
                {
                    m_pMenu->doResyncGame();
                }
            }
        }
    }
}

bool ProxyAi::verifyActionStack()
{
    bool valid = true;
    if (m_pPlayer == m_pMap->getCurrentPlayer() &&
        m_ActionBuffer.size() > 0 &&
        m_pMenu->getActionRunning())
    {
        valid = m_ActionBuffer[0]->getSyncCounter() == m_pMenu->getSyncCounter() + 1;
    }
    for (qint32 i = 0; i < m_ActionBuffer.size() - 1 && valid; ++i)
    {
        valid = m_ActionBuffer[i]->getSyncCounter() + 1 == m_ActionBuffer[i + 1]->getSyncCounter();
    }
    return valid;
}

void ProxyAi::nextAction()
{
    QMutexLocker locker(&m_ActionMutex);
    if (m_pMenu != nullptr &&
        !m_pMenu->getActionRunning() &&
        m_pPlayer == m_pMap->getCurrentPlayer())
    {
        if (verifyActionStack())
        {
            if (m_ActionBuffer.size() > 0)
            {
                spGameAction pAction = m_ActionBuffer.front();
                m_ActionBuffer.pop_front();
                if (pAction->getSyncCounter() == m_pMenu->getSyncCounter() + 1)
                {
                    AI_CONSOLE_PRINT("Emitting action " + pAction->getActionID() + " for player " + QString::number(m_pPlayer->getPlayerID()) +
                                     " current player is " + QString::number(m_pMap->getCurrentPlayer()->getPlayerID()) +
                                     " with sync counter " + QString::number(pAction->getSyncCounter()), GameConsole::eDEBUG);
                    emit sigPerformAction(pAction);
                }
            }
        }
        else
        {
            m_pMenu->doResyncGame();
        }
    }
}
