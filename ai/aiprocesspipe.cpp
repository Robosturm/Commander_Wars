#include <QDataStream>
#include <QCoreApplication>

#include "ai/aiprocesspipe.h"

#include "coreengine/settings.h"
#include "coreengine/console.h"

#include "game/gameaction.h"
#include "game/gamemap.h"
#include "game/actionperformer.h"

#include "menue/gamemenue.h"

#include "3rd_party/oxygine-framework/oxygine/actor/Stage.h"

namespace
{
    const char* const PIPENAME = "CommanderWarsAiPipe";
    const char* const NEWACTIONFORMASTER = "NEWACTIONFORMASTER";
    const char* const NEWACTIONFORSLAVE = "NEWACTIONFORSLAVE";
    const char* const PIPEREADY = "PIPEREADY";
    const char* const STARTGAME = "STARTGAME";
    const char* const GAMESTARTED = "GAMESTARTED";
    const char* const QUITGAME = "QUITGAME";
}

AiProcessPipe::AiProcessPipe()
{
    connect(this, &AiProcessPipe::sigStartPipe, this, &AiProcessPipe::startPipe, Qt::QueuedConnection);
    connect(this, &AiProcessPipe::sigPipeReady, this, &AiProcessPipe::pipeReady, Qt::QueuedConnection);
}

AiProcessPipe::~AiProcessPipe()
{
    if (Settings::getAiSlave())
    {
        m_animationSkipper.restoreAnimationSettings();
    }
}

void AiProcessPipe::startPipe()
{
    QString pipeName = PIPENAME + Settings::getPipeUuid();
    CONSOLE_PRINT("Starting AI-Pipe with name " + pipeName, Console::eDEBUG);
    if (Settings::getAiSlave())
    {
        m_animationSkipper.startSeeking();
        m_pClient = spLocalClient::create(this);
        m_pActiveConnection = m_pClient.get();
    }
    else if (Settings::getSpawnAiProcess())
    {
        m_pServer = spLocalServer::create(this);
        m_pActiveConnection = m_pServer.get();
    }
    if (m_pActiveConnection != nullptr)
    {
        connect(m_pActiveConnection, &NetworkInterface::sigConnected, this, &AiProcessPipe::onConnected, Qt::QueuedConnection);
        connect(m_pActiveConnection, &NetworkInterface::recieveData, this, &AiProcessPipe::recieveData, Qt::QueuedConnection);
        m_pActiveConnection->connectTCP(pipeName, 0, "");
    }
}

void AiProcessPipe::onConnected(quint64 socket)
{
    CONSOLE_PRINT("AI-Pipe connected", Console::eDEBUG);
    if (m_pServer.get() != nullptr)
    {
        m_pServer->pauseListening();
    }
    m_pipeState = PipeState::Connected;
}

void AiProcessPipe::onGameStarted(GameMenue* pMenu)
{
    if (m_pActiveConnection != nullptr &&
        Settings::getSpawnAiProcess() &&
        !Settings::getAiSlave())
    {
        CONSOLE_PRINT("AI-Pipe waiting for ready", Console::eDEBUG);
        while (m_pipeState != PipeState::Ready)
        {
            QCoreApplication::processEvents();
            QThread::msleep(10);
        }
        CONSOLE_PRINT("AI-Pipe preparing the game", Console::eDEBUG);
        m_pipeState = PipeState::PreparingGame;
        m_pMenu = pMenu;
        m_pMap = pMenu->getMap();
        connect(&m_pMenu->getActionPerformer(), &ActionPerformer::sigAiProcesseSendAction, this, &AiProcessPipe::sendActionToSlave, Qt::QueuedConnection);
        connect(this, &AiProcessPipe::sigPerformAction, &m_pMenu->getActionPerformer(), &ActionPerformer::performAction, Qt::QueuedConnection);
        connect(&m_pMenu->getActionPerformer(), &ActionPerformer::sigActionPerformed, this, &AiProcessPipe::nextAction, Qt::QueuedConnection);
        QString command = QString(STARTGAME);
        CONSOLE_PRINT("AI-Pipe sending command " + command, Console::eDEBUG);
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << command;
        stream << true;
        m_pMap->serializeObject(stream);
        emit m_pActiveConnection->sig_sendData(0, data, NetworkInterface::NetworkSerives::AiPipe, false);
        CONSOLE_PRINT("AI-Pipe waiting for ingame", Console::eDEBUG);
        while (m_pipeState != PipeState::Ingame)
        {
            QThread::msleep(10);
            QCoreApplication::processEvents();
        }
        CONSOLE_PRINT("AI-Pipe continue starting the game", Console::eDEBUG);
    }
}

void AiProcessPipe::onQuitGame()
{
    if (m_pActiveConnection != nullptr &&
        Settings::getSpawnAiProcess() &&
        !Settings::getAiSlave())
    {
        QMutexLocker locker(&m_ActionMutex);
        m_pipeState = PipeState::Ready;
        m_pMenu = nullptr;
        m_pMap = nullptr;
        m_ActionBuffer.clear();
        QString command = QString(QUITGAME);
        CONSOLE_PRINT("AI-Pipe sending command " + command, Console::eDEBUG);
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << command;
        emit m_pActiveConnection->sig_sendData(0, data, NetworkInterface::NetworkSerives::AiPipe, false);
    }
}

void AiProcessPipe::quit()
{
    if (m_pActiveConnection != nullptr)
    {
        m_pActiveConnection->disconnectTCP();
        m_pServer = nullptr;
        m_pClient = nullptr;
        m_pActiveConnection = nullptr;
    }
}

void AiProcessPipe::recieveData(quint64, QByteArray data, NetworkInterface::NetworkSerives service)
{
    if (service == NetworkInterface::NetworkSerives::AiPipe)
    {
        QDataStream stream(&data, QIODevice::ReadOnly);
        QString messageType;
        stream >> messageType;
        CONSOLE_PRINT("AI-Pipe command received: " + messageType, Console::eDEBUG);
        if (messageType == NEWACTIONFORMASTER)
        {
            onNewActionForMaster(stream);
        }
        else if (messageType == NEWACTIONFORSLAVE)
        {
            onNewAction(stream);
        }
        else if (messageType == PIPEREADY)
        {
            m_pipeState = PipeState::Ready;
        }
        else if (messageType == GAMESTARTED)
        {
            m_pipeState = PipeState::Ingame;
        }
        else if (messageType == STARTGAME)
        {
            onStartGame(stream);
        }
        else if (messageType == QUITGAME)
        {
            quitGame();
        }
        else
        {
            CONSOLE_PRINT("AI-Pipe unknown command " + messageType + " received", Console::eDEBUG);
        }
    }
    else
    {
        CONSOLE_PRINT("AI-Pipe received data from none pipe stream", Console::eDEBUG);
    }
}

void AiProcessPipe::sendActionToSlave(spGameAction pAction)
{
    if (m_pipeState == PipeState::Ingame)
    {
        QString command = QString(NEWACTIONFORSLAVE);
        CONSOLE_PRINT("AI-Pipe sending command " + command + " action=" + pAction->getActionID(), Console::eDEBUG);
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << command;
        pAction->serializeObject(stream);
        emit m_pActiveConnection->sig_sendData(0, data, NetworkInterface::NetworkSerives::AiPipe, false);
    }
}

void AiProcessPipe::sendActionToMaster(spGameAction pAction)
{
    if (m_pipeState == PipeState::Ingame)
    {
        QString command = QString(NEWACTIONFORMASTER);
        CONSOLE_PRINT("AI-Pipe sending command " + command + " action=" + pAction->getActionID(), Console::eDEBUG);
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << command;
        pAction->serializeObject(stream);
        emit m_pActiveConnection->sig_sendData(0, data, NetworkInterface::NetworkSerives::AiPipe, false);
    }
}

void AiProcessPipe::onNewActionForMaster(QDataStream & stream)
{
    if (m_pipeState == PipeState::Ingame)
    {
        onNewAction(stream);
    }
}

void AiProcessPipe::onNewAction(QDataStream & stream)
{
    if (m_pipeState == PipeState::Ingame)
    {
        QMutexLocker locker(&m_ActionMutex);
        spGameAction pAction = spGameAction::create(m_pMap);
        pAction->deserializeObject(stream);
        m_ActionBuffer.append(pAction);
        if (m_pMenu.get() != nullptr &&
            !m_pMenu->getActionRunning())
        {
            spGameAction pAction = m_ActionBuffer.front();
            m_ActionBuffer.pop_front();
            AI_CONSOLE_PRINT("Emitting action " + pAction->getActionID() + " for current player is " + QString::number(m_pMap->getCurrentPlayer()->getPlayerID()) +
                             " with sync counter " + QString::number(pAction->getSyncCounter()), Console::eDEBUG);
            emit sigPerformAction(pAction);
        }
    }
}

void AiProcessPipe::onStartGame(QDataStream & stream)
{
    CONSOLE_PRINT("AI-Pipe launching game on slave", Console::eDEBUG);
    bool savegame = false;
    stream >> savegame;
    spGameMap pMap = spGameMap::create<QDataStream &, bool>(stream, savegame);
    spGameMenue pMenu = spGameMenue::create(pMap, savegame, spNetworkInterface());
    oxygine::Stage::getStage()->addChild(pMenu);
    m_pMap = pMap.get();
    m_pMenu = pMenu.get();
    m_pipeState = PipeState::Ingame;
    QString command = QString(GAMESTARTED);
    CONSOLE_PRINT("AI-Pipe sending command " + command +  " current player=" + QString::number(m_pMap->getCurrentPlayer()->getPlayerID()), Console::eDEBUG);
    QByteArray data;
    QDataStream outStream(&data, QIODevice::WriteOnly);
    outStream << command;
    emit m_pActiveConnection->sig_sendData(0, data, NetworkInterface::NetworkSerives::AiPipe, false);
    connect(this, &AiProcessPipe::sigPerformAction, &m_pMenu->getActionPerformer(), &ActionPerformer::performAction, Qt::QueuedConnection);
    connect(&m_pMenu->getActionPerformer(), &ActionPerformer::sigActionPerformed, this, &AiProcessPipe::nextAction, Qt::QueuedConnection);
}

void AiProcessPipe::quitGame()
{
    QMutexLocker locker(&m_ActionMutex);
    m_pipeState = PipeState::Ready;
    if (m_pMenu.get() != nullptr)
    {
        m_pMenu->exitGame();
    }
    m_pMenu = nullptr;
    m_pMap = nullptr;
    m_ActionBuffer.clear();
}

void AiProcessPipe::pipeReady()
{
    m_pipeState = PipeState::Ready;
    QString command = QString(PIPEREADY);
    CONSOLE_PRINT("AI-Pipe sending command " + command, Console::eDEBUG);
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << command;
    emit m_pActiveConnection->sig_sendData(0, data, NetworkInterface::NetworkSerives::AiPipe, false);
}

void AiProcessPipe::nextAction()
{
    if (m_pipeState == PipeState::Ingame)
    {
        QMutexLocker locker(&m_ActionMutex);
        if (m_pMenu.get() != nullptr &&
            !m_pMenu->getActionRunning())
        {
            if (m_ActionBuffer.size() > 0)
            {
                spGameAction pAction = m_ActionBuffer.front();
                m_ActionBuffer.pop_front();
                AI_CONSOLE_PRINT("Emitting action " + pAction->getActionID() + " for current player is " + QString::number(m_pMap->getCurrentPlayer()->getPlayerID()) +
                                 " with sync counter " + QString::number(pAction->getSyncCounter()), Console::eDEBUG);
                emit sigPerformAction(pAction);
            }
        }
    }
}

AiProcessPipe::PipeState AiProcessPipe::getPipeState() const
{
    return m_pipeState;
}

void AiProcessPipe::disconnected(quint64 socket)
{
    CONSOLE_PRINT("AI-Pipe broken", Console::eDEBUG);
    m_pipeState = PipeState::Disconnected;
}
