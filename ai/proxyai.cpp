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

void ProxyAi::init(BaseGamemenu* pMenu)
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
        m_pGameMenue = oxygine::safeCast<GameMenue*>(pMenu);
        connect(m_pMenu->getActionPerformer(), &ActionPerformer::sigActionPerformed, this, &CoreAI::nextAction, Qt::QueuedConnection);
        connect(this, &CoreAI::sigPerformAction, m_pMenu->getActionPerformer(), &ActionPerformer::performAction, Qt::DirectConnection);
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
    for (const auto & action : std::as_const(m_ActionBuffer))
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
            spGameAction pAction = MemoryManagement::create<GameAction>(m_pMap);
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
        stream.setVersion(QDataStream::Version::Qt_6_5);
        qint32 player = 0;
        stream >> player;
        if (m_pPlayer->getPlayerID() == player)
        {
            CONSOLE_PRINT("Received action from network for player " + QString::number(player), GameConsole::eDEBUG);
            QMutexLocker locker(&m_ActionMutex);
            spGameAction pAction = MemoryManagement::create<GameAction>(m_pMap);
            pAction->deserializeObject(stream);
            m_ActionBuffer.append(pAction);
            if (m_pGameMenue != nullptr &&
                !m_pGameMenue->getActionRunning() &&
                m_pPlayer == m_pMap->getCurrentPlayer())
            {
                if (verifyActionStack())
                {
                    doNextAction();
                }
                else
                {
                    onInvalidStack();
                }
            }
        }
    }
}

void ProxyAi::doNextAction()
{
    spGameAction pAction = m_ActionBuffer.front();
    if (pAction->getSyncCounter() == m_pGameMenue->getSyncCounter() + 1)
    {
        CONSOLE_PRINT("Proxy ai emitting action " + pAction->getActionID() + " for player " + QString::number(m_pPlayer->getPlayerID()) +
                          " current player is " + QString::number(m_pMap->getCurrentPlayer()->getPlayerID()) +
                          " with sync counter " + QString::number(pAction->getSyncCounter()), GameConsole::eDEBUG);
        m_ActionBuffer.pop_front();
        emit sigPerformAction(pAction);
    }
}

void ProxyAi::onInvalidStack()
{
    CONSOLE_PRINT("Invalid action stack found", GameConsole::eDEBUG);
    m_ActionBuffer.clear();
    m_pGameMenue->doResyncGame();
}

bool ProxyAi::verifyActionStack()
{
    bool valid = true;
    if (m_ActionBuffer.size() > 0)
    {
        if (m_pPlayer == m_pMap->getCurrentPlayer() &&
            m_ActionBuffer.size() > 0 &&
            m_pGameMenue->getActionRunning())
        {
            valid = m_ActionBuffer[0]->getSyncCounter() == m_pGameMenue->getSyncCounter() + 1;
        }
        for (qint32 i = 0; i < m_ActionBuffer.size() - 1 && valid; ++i)
        {
            valid = m_ActionBuffer[i]->getSyncCounter() + 1 == m_ActionBuffer[i + 1]->getSyncCounter();
        }
    }
    return valid;
}

void ProxyAi::nextAction()
{
    QMutexLocker locker(&m_ActionMutex);
    if (m_pGameMenue != nullptr &&
        !m_pGameMenue->getActionRunning() &&
        m_pPlayer == m_pMap->getCurrentPlayer())
    {
        if (verifyActionStack())
        {
            if (m_ActionBuffer.size() > 0)
            {
                doNextAction();
            }
        }
        else
        {
            onInvalidStack();
        }
    }
}
