#include "menue/gamemenue.h"

#include "game/player.h"
#include "game/co.h"

#include "menue/victorymenue.h"
#include "coreengine/console.h"
#include "coreengine/audiothread.h"
#include "ai/proxyai.h"

#include "gameinput/humanplayerinput.h"

#include "game/gameanimationfactory.h"

#include "game/unitpathfindingsystem.h"

#include "game/battleanimation.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "objects/filedialog.h"

#include "objects/coinfodialog.h"

#include "objects/dialogvictoryconditions.h"
#include "objects/dialogconnecting.h"
#include "objects/dialogmessagebox.h"
#include "objects/dialogtextinput.h"

#include "coreengine/tweenaddcolorall.h"

#include "multiplayer/networkcommands.h"

#include "wiki/fieldinfo.h"

#include "ingamescriptsupport/genericbox.h"

#include "objects/tableview.h"
#include "objects/dialogattacklog.h"
#include "objects/dialogunitinfo.h"

#include <QFile>
#include <QTime>
#include <qguiapplication.h>

GameMenue* GameMenue::m_pInstance = nullptr;

GameMenue::GameMenue(bool saveGame, spNetworkInterface pNetworkInterface)
    : InGameMenue(),
      m_SaveGame(saveGame)
{
    oxygine::Actor::addChild(GameMap::getInstance());
    loadHandling();
    m_pNetworkInterface = pNetworkInterface;
    loadGameMenue();
    loadUIButtons();
    if (m_pNetworkInterface.get() != nullptr)
    {
        GameMap* pMap = GameMap::getInstance();
        for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
        {
            Player* pPlayer = pMap->getPlayer(i);
            if (pPlayer->getBaseGameInput()->getAiType() == GameEnums::AiTypes_ProxyAi)
            {
                dynamic_cast<ProxyAi*>(pPlayer->getBaseGameInput())->connectInterface(m_pNetworkInterface.get());
            }
        }
        connect(m_pNetworkInterface.get(), &NetworkInterface::sigDisconnected, this, &GameMenue::disconnected, Qt::QueuedConnection);
        connect(m_pNetworkInterface.get(), &NetworkInterface::recieveData, this, &GameMenue::recieveData, Qt::QueuedConnection);
        if (!m_pNetworkInterface->getIsServer())
        {
            QByteArray sendData;
            QDataStream sendStream(&sendData, QIODevice::WriteOnly);
            sendStream << NetworkCommands::CLIENTINITGAME;
            m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
        }
        else
        {
            m_PlayerSockets = dynamic_cast<TCPServer*>(m_pNetworkInterface.get())->getConnectedSockets();
            connect(m_pNetworkInterface.get(), &NetworkInterface::sigConnected, this, &GameMenue::playerJoined, Qt::QueuedConnection);
        }
        spDialogConnecting pDialogConnecting = new DialogConnecting(tr("Waiting for Players"), 1000 * 60 * 5);
        addChild(pDialogConnecting);
        connect(pDialogConnecting.get(), &DialogConnecting::sigCancel, this, &GameMenue::exitGame, Qt::QueuedConnection);
        connect(this, &GameMenue::sigGameStarted, pDialogConnecting.get(), &DialogConnecting::connected, Qt::QueuedConnection);
        connect(this, &GameMenue::sigGameStarted, this, &GameMenue::startGame, Qt::QueuedConnection);

        m_pChat = new Chat(pNetworkInterface, QSize(Settings::getWidth(), Settings::getHeight() - 100));
        m_pChat->setPriority(static_cast<short>(Mainapp::ZOrder::Dialogs));
        m_pChat->setVisible(false);
        addChild(m_pChat);
    }
    else
    {
        gameStarted = true;
        startGame();
    }
}

GameMenue::GameMenue(QString map, bool saveGame)
    : InGameMenue(-1, -1, map),
      gameStarted(true),
      m_SaveGame(saveGame)

{
    oxygine::Actor::addChild(GameMap::getInstance());
    loadHandling();
    loadGameMenue();
    loadUIButtons();
}

GameMenue::GameMenue()
    : InGameMenue()
{
}

void GameMenue::recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service)
{
    if (service == NetworkInterface::NetworkSerives::Multiplayer)
    {
        QDataStream stream(&data, QIODevice::ReadOnly);
        QString messageType;
        stream >> messageType;
        if (messageType == NetworkCommands::CLIENTINITGAME)
        {
            if (m_pNetworkInterface->getIsServer())
            {
                // the given client is ready
                m_ReadySockets.append(socketID);
                QVector<quint64> sockets = dynamic_cast<TCPServer*>(m_pNetworkInterface.get())->getConnectedSockets();
                bool ready = true;
                for (qint32 i = 0; i < sockets.size(); i++)
                {
                    if (!m_ReadySockets.contains(sockets[i]))
                    {
                        ready = false;
                    }
                }
                if (ready)
                {
                    QByteArray sendData;
                    QDataStream sendStream(&sendData, QIODevice::WriteOnly);
                    sendStream << NetworkCommands::STARTGAME;
                    emit m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
                    emit sigGameStarted();
                }
            }
        }
        else if (messageType == NetworkCommands::STARTGAME)
        {
            if (!m_pNetworkInterface->getIsServer())
            {
                emit sigGameStarted();
            }
        }
    }
    else if (service == NetworkInterface::NetworkSerives::Chat)
    {
        if (m_pChat->getVisible() == false)
        {
            oxygine::spTween tween = oxygine::createTween(TweenAddColorAll(QColor(0, 200, 0, 0), false), oxygine::timeMS(1000), -1, true);
            m_ChatButton->addTween(tween);
        }
    }
}

Player* GameMenue::getCurrentViewPlayer()
{
    GameMap* pMap = GameMap::getInstance();
    spPlayer pCurrentPlayer = pMap->getCurrentPlayer();
    if (pCurrentPlayer.get() != nullptr)
    {
        qint32 currentPlayerID = pCurrentPlayer->getPlayerID();
        for (qint32 i = currentPlayerID; i >= 0; i--)
        {
            if (pMap->getPlayer(i)->getBaseGameInput() != nullptr &&
                pMap->getPlayer(i)->getBaseGameInput()->getAiType() == GameEnums::AiTypes_Human)
            {
                return pMap->getPlayer(i);
            }
        }
        for (qint32 i = pMap->getPlayerCount() - 1; i > currentPlayerID; i--)
        {
            if (pMap->getPlayer(i)->getBaseGameInput() != nullptr &&
                pMap->getPlayer(i)->getBaseGameInput()->getAiType() == GameEnums::AiTypes_Human)
            {
                return pMap->getPlayer(i);
            }
        }
        return pCurrentPlayer.get();
    }
    return nullptr;
}

void GameMenue::playerJoined(quint64 socketID)
{
    if (m_pNetworkInterface->getIsServer())
    {
        if (m_PlayerSockets.contains(socketID))
        {
            // reject connection by disconnecting
            emit dynamic_cast<TCPServer*>(m_pNetworkInterface.get())->sigDisconnectClient(socketID);
        }
    }
}

void GameMenue::disconnected(quint64 socketID)
{
    if (m_pNetworkInterface.get() != nullptr)
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->suspendThread();
        bool showDisconnect = false;
        GameMap* pMap = GameMap::getInstance();
        for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
        {
            Player* pPlayer = pMap->getPlayer(i);
            quint64 playerSocketID = pPlayer->getSocketId();
            if (socketID == playerSocketID &&
                !pPlayer->getIsDefeated())
            {
                showDisconnect = true;
                break;
            }
        }

        if (m_pNetworkInterface.get() != nullptr)
        {
            emit m_pNetworkInterface->sig_close();
            m_pNetworkInterface = nullptr;
        }
        if (showDisconnect)
        {
            gameStarted = false;
            spDialogMessageBox pDialogMessageBox = new DialogMessageBox(tr("A player has disconnected from the game! The game will now be stopped. You can save the game and reload the game to continue playing this map."));
            addChild(pDialogMessageBox);
        }
        pApp->continueThread();

    }
}

bool GameMenue::isNetworkGame()
{
    if (m_pNetworkInterface.get() != nullptr)
    {
        return true;
    }
    return false;
}

void GameMenue::loadGameMenue()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    m_pInstance = this;
    if (m_pNetworkInterface.get() != nullptr)
    {
        m_Multiplayer = true;
    }
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        pMap->getPlayer(i)->getBaseGameInput()->init();
    }
    pMap->centerMap(pMap->getMapWidth() / 2, pMap->getMapHeight() / 2);

    // back to normal code
    m_pPlayerinfo = new PlayerInfo();
    m_IngameInfoBar = new IngameInfoBar();
    m_IngameInfoBar->updateMinimap();
    m_pPlayerinfo->updateData();
    addChild(m_IngameInfoBar);
    addChild(m_pPlayerinfo);

    connect(&m_UpdateTimer, &QTimer::timeout, this, &GameMenue::updateTimer, Qt::QueuedConnection);
    connectMap();
    connect(this, &GameMenue::sigExitGame, this, &GameMenue::exitGame, Qt::QueuedConnection);
    connect(this, &GameMenue::sigShowExitGame, this, &GameMenue::showExitGame, Qt::QueuedConnection);
    connect(this, &GameMenue::sigShowSurrenderGame, this, &GameMenue::showSurrenderGame, Qt::QueuedConnection);
    connect(this, &GameMenue::sigSaveGame, this, &GameMenue::saveGame, Qt::QueuedConnection);    
    connect(this, &GameMenue::sigNicknameUnit, this, &GameMenue::nicknameUnit, Qt::QueuedConnection);

    connect(GameAnimationFactory::getInstance(), &GameAnimationFactory::animationsFinished, this, &GameMenue::actionPerformed, Qt::QueuedConnection);
    connect(m_Cursor.get(), &Cursor::sigCursorMoved, m_IngameInfoBar.get(), &IngameInfoBar::updateCursorInfo, Qt::QueuedConnection);
    connect(m_Cursor.get(), &Cursor::sigCursorMoved, this, &GameMenue::cursorMoved, Qt::QueuedConnection);
}

void GameMenue::connectMap()
{
    GameMap* pMap = GameMap::getInstance();
    connect(pMap->getGameRules(), &GameRules::signalVictory, this, &GameMenue::victory, Qt::QueuedConnection);
    connect(pMap->getGameRules()->getRoundTimer(), &Timer::timeout, pMap, &GameMap::nextTurn, Qt::QueuedConnection);
    connect(pMap, &GameMap::signalExitGame, this, &GameMenue::showExitGame, Qt::QueuedConnection);
    connect(pMap, &GameMap::sigSurrenderGame, this, &GameMenue::showSurrenderGame, Qt::QueuedConnection);
    connect(pMap, &GameMap::signalSaveGame, this, &GameMenue::saveGame, Qt::QueuedConnection);
    connect(pMap, &GameMap::sigShowGameInfo, this, &GameMenue::showGameInfo, Qt::QueuedConnection);
    connect(pMap, &GameMap::signalVictoryInfo, this, &GameMenue::victoryInfo, Qt::QueuedConnection);
    connect(pMap, &GameMap::signalShowCOInfo, this, &GameMenue::showCOInfo, Qt::QueuedConnection);
    connect(pMap, &GameMap::sigShowAttackLog, this, &GameMenue::showAttackLog, Qt::QueuedConnection);
    connect(pMap, &GameMap::sigShowUnitInfo, this, &GameMenue::showUnitInfo, Qt::QueuedConnection);
    connect(pMap, &GameMap::sigQueueAction, this, &GameMenue::performAction, Qt::QueuedConnection);
    connect(pMap, &GameMap::sigShowNicknameUnit, this, &GameMenue::showNicknameUnit, Qt::QueuedConnection);
    connect(m_IngameInfoBar->getMinimap(), &Minimap::clicked, pMap, &GameMap::centerMap, Qt::QueuedConnection);
}

void GameMenue::loadUIButtons()
{
    GameMap* pMap = GameMap::getInstance();
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    oxygine::spBox9Sprite pButtonBox = new oxygine::Box9Sprite();
    pButtonBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pButtonBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pButtonBox->setResAnim(pAnim);
    qint32 roundTime = pMap->getGameRules()->getRoundTimeMs();
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    m_CurrentRoundTime = new oxygine::TextField();
    m_CurrentRoundTime->setStyle(style);
    if (roundTime > 0)
    {
        pButtonBox->setSize(286 + 70, 50);
        m_CurrentRoundTime->setPosition(108 + 4, 10);
        pButtonBox->addChild(m_CurrentRoundTime);
        updateTimer();
    }
    else
    {
        pButtonBox->setSize(286, 50);
    }
    pButtonBox->setPosition((Settings::getWidth() - m_IngameInfoBar->getWidth()) / 2 - pButtonBox->getWidth() / 2 + 50, Settings::getHeight() - pButtonBox->getHeight() + 6);
    pButtonBox->setPriority(static_cast<qint16>(Mainapp::ZOrder::Objects));
    addChild(pButtonBox);
    oxygine::spButton saveGame = pObjectManager->createButton(tr("Save"), 130);
    saveGame->setPosition(8, 4);
    saveGame->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigSaveGame();
    });
    pButtonBox->addChild(saveGame);

    oxygine::spButton exitGame = pObjectManager->createButton(tr("Exit"), 130);
    exitGame->setPosition(pButtonBox->getWidth() - 138, 4);
    exitGame->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigShowExitGame();
    });
    pButtonBox->addChild(exitGame);

    pAnim = pObjectManager->getResAnim("panel");
    pButtonBox = new oxygine::Box9Sprite();
    pButtonBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pButtonBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pButtonBox->setResAnim(pAnim);
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    xyTextInfo = new Label(180);
    xyTextInfo->setStyle(style);
    xyTextInfo->setHtmlText("X: 0 Y: 0");
    xyTextInfo->setPosition(8, 8);
    pButtonBox->addChild(xyTextInfo);
    pButtonBox->setSize(200, 50);
    pButtonBox->setPosition((Settings::getWidth() - m_IngameInfoBar->getScaledWidth())  - pButtonBox->getWidth(), 0);
    pButtonBox->setPriority(static_cast<qint16>(Mainapp::ZOrder::Objects));
    addChild(pButtonBox);
    m_UpdateTimer.setInterval(500);
    m_UpdateTimer.setSingleShot(false);
    m_UpdateTimer.start();

    if (m_pNetworkInterface.get() != nullptr)
    {
        pButtonBox = new oxygine::Box9Sprite();
        pButtonBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
        pButtonBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
        pButtonBox->setResAnim(pAnim);
        pButtonBox->setSize(144, 50);
        pButtonBox->setPosition(0, Settings::getHeight() - pButtonBox->getHeight());
        pButtonBox->setPriority(static_cast<qint16>(Mainapp::ZOrder::Objects));
        addChild(pButtonBox);
        m_ChatButton = pObjectManager->createButton(tr("Show Chat"), 130);
        m_ChatButton->setPosition(8, 4);
        m_ChatButton->addClickListener([=](oxygine::Event*)
        {
            m_pChat->setVisible(!m_pChat->getVisible());
            setFocused(!m_pChat->getVisible());
            m_ChatButton->removeTweens();
            m_ChatButton->setAddColor(0, 0, 0, 0);
        });
        pButtonBox->addChild(m_ChatButton);
    }
}

void GameMenue::updateTimer()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    QTimer* pTimer = GameMap::getInstance()->getGameRules()->getRoundTimer();
    qint32 roundTime = pTimer->remainingTime();
    if (!pTimer->isActive())
    {
        roundTime = pTimer->interval();
    }
    if (roundTime < 0)
    {
        roundTime = 0;
    }
    m_CurrentRoundTime->setHtmlText(QTime::fromMSecsSinceStartOfDay(roundTime).toString("hh:mm:ss"));
    pApp->continueThread();
}

bool GameMenue::getGameStarted() const
{
    return gameStarted;
}

GameMenue::~GameMenue()
{
    m_pInstance = nullptr;
}

void GameMenue::editFinishedCanceled()
{
    setFocused(true);
}

GameAction* GameMenue::doMultiTurnMovement(GameAction* pGameAction)
{
    if (pGameAction != nullptr &&
        (pGameAction->getActionID() == CoreAI::ACTION_NEXT_PLAYER ||
         pGameAction->getActionID() == CoreAI::ACTION_SWAP_COS))
    {
        GameMap* pMap = GameMap::getInstance();
        // check for units that have a multi turn avaible
        qint32 heigth = pMap->getMapHeight();
        qint32 width = pMap->getMapWidth();
        Player* pPlayer = pMap->getCurrentPlayer();
        for (qint32 y = 0; y < heigth; y++)
        {
            for (qint32 x = 0; x < width; x++)
            {
                Unit* pUnit = pMap->getTerrain(x, y)->getUnit();
                if (pUnit != nullptr)
                {
                    if ((pUnit->getOwner() == pPlayer) &&
                        (pUnit->getHasMoved() == false))
                    {
                        QVector<QPoint> currentMultiTurnPath = pUnit->getMultiTurnPath();
                        if (currentMultiTurnPath.size() > 0)
                        {
                            // replace current action with auto moving none moved units
                            m_pStoredAction = pGameAction;
                            GameAction* multiTurnMovement = new GameAction(CoreAI::ACTION_WAIT);
                            if (pUnit->getActionList().contains(CoreAI::ACTION_HOELLIUM_WAIT))
                            {
                                multiTurnMovement->setActionID(CoreAI::ACTION_HOELLIUM_WAIT);
                            }
                            multiTurnMovement->setTarget(pUnit->getPosition());
                            UnitPathFindingSystem pfs(pUnit, pPlayer);
                            pfs.setMovepoints(pUnit->getFuel());
                            pfs.explore();
                            qint32 movepoints = pUnit->getMovementpoints(multiTurnMovement->getTarget());
                            // shorten path
                            QVector<QPoint> newPath = pfs.getClosestReachableMovePath(currentMultiTurnPath, movepoints);
                            multiTurnMovement->setMovepath(newPath, pfs.getCosts(newPath));
                            QVector<QPoint> multiTurnPath;
                            // still some path ahead?
                            if (newPath.size() == 0)
                            {
                                multiTurnPath = currentMultiTurnPath;
                            }
                            else if (currentMultiTurnPath.size() > newPath.size())
                            {
                                for (qint32 i = 0; i <= currentMultiTurnPath.size() - newPath.size(); i++)
                                {
                                    multiTurnPath.append(currentMultiTurnPath[i]);
                                }
                            }
                            multiTurnMovement->setMultiTurnPath(multiTurnPath);
                            return multiTurnMovement;
                        }
                        else if (pUnit->getCapturePoints() > 0 && pUnit->getActionList().contains(CoreAI::ACTION_CAPTURE))
                        {
                            m_pStoredAction = pGameAction;
                            GameAction* multiTurnMovement = new GameAction(CoreAI::ACTION_CAPTURE);
                            multiTurnMovement->setTarget(pUnit->getPosition());
                            return multiTurnMovement;
                        }
                    }
                }
            }
        }
    }
    return pGameAction;
}

void GameMenue::performAction(GameAction* pGameAction)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    if (pGameAction != nullptr)
    {
        GameMap* pMap = GameMap::getInstance();
        bool multiplayer = !pGameAction->getIsLocal() &&
                           m_pNetworkInterface.get() != nullptr &&
                                                        gameStarted;

        if (multiplayer &&
            pMap->getCurrentPlayer()->getBaseGameInput()->getAiType() == GameEnums::AiTypes_ProxyAi &&
            m_syncCounter + 1 != pGameAction->getSyncCounter())
        {
            gameStarted = false;
            spDialogMessageBox pDialogMessageBox = new DialogMessageBox(tr("The game is out of sync and can't be continued. The game has been stopped. You can save the game and restart."));
            addChild(pDialogMessageBox);
        }
        else
        {
            if (multiplayer &&
                pMap->getCurrentPlayer()->getBaseGameInput()->getAiType() == GameEnums::AiTypes_ProxyAi)
            {
                m_syncCounter = pGameAction->getSyncCounter();
            }
            m_pStoredAction = nullptr;
            pMap->getGameRules()->pauseRoundTime();
            if (!pGameAction->getIsLocal() &&
                (pMap->getCurrentPlayer()->getBaseGameInput()->getAiType() != GameEnums::AiTypes_ProxyAi))
            {
                pGameAction = doMultiTurnMovement(pGameAction);
            }
            QVector<QPoint> path = pGameAction->getMovePath();
            Unit * pMoveUnit = pGameAction->getTargetUnit();
            if (path.size() > 0 && pMoveUnit != nullptr)
            {
                QVector<QPoint> trapPath;
                qint32 trapPathCost = 0;
                for (qint32 i = path.size() - 1; i >= 0; i--)
                {
                    // check the movepath for a trap
                    QPoint point = path[i];
                    Unit* pUnit = pMap->getTerrain(point.x(), point.y())->getUnit();
                    if ((pUnit != nullptr) &&
                        (pUnit->isStealthed(pMap->getCurrentPlayer())))
                    {
                        while (trapPath.size() > 1)
                        {
                            QPoint currentPoint = trapPath[0];
                            QPoint previousPoint = trapPath[1];
                            Unit* pUnit = pMap->getTerrain(currentPoint.x(), currentPoint.y())->getUnit();
                            qint32 moveCost = pMoveUnit->getMovementCosts(currentPoint.x(), currentPoint.y(),
                                                                          previousPoint.x(), previousPoint.y());
                            if (pUnit != nullptr || moveCost == 0)
                            {
                                trapPathCost -= moveCost;
                                trapPath.pop_front();
                            }
                            else
                            {
                                break;
                            }
                        }
                        GameAction* pTrapAction = new GameAction("ACTION_TRAP");
                        pTrapAction->setMovepath(trapPath, trapPathCost);
                        pMoveUnit->getOwner()->addVisionField(point.x(), point.y(), 1, true);
                        pTrapAction->writeDataInt32(point.x());
                        pTrapAction->writeDataInt32(point.y());
                        pTrapAction->setTarget(pGameAction->getTarget());
                        delete pGameAction;
                        pGameAction = pTrapAction;
                        break;
                    }
                    else
                    {
                        trapPath.push_front(point);
                        if (point.x() != pMoveUnit->getX() ||
                            point.y() != pMoveUnit->getY())
                        {
                            QPoint previousPoint = path[i + 1];
                            trapPathCost += pMoveUnit->getMovementCosts(point.x(), point.y(), previousPoint.x(), previousPoint.y());
                        }
                    }
                }
            }
            // send action to other players if needed
            if (multiplayer &&
                pMap->getCurrentPlayer()->getBaseGameInput()->getAiType() != GameEnums::AiTypes_ProxyAi)
            {
                m_syncCounter++;
                pGameAction->setSyncCounter(m_syncCounter);
                QByteArray data;
                QDataStream stream(&data, QIODevice::WriteOnly);
                stream << pMap->getCurrentPlayer()->getPlayerID();
                pGameAction->serializeObject(stream);
                emit m_pNetworkInterface->sig_sendData(0, data, NetworkInterface::NetworkSerives::Game, true);
            }
            // record action if required
            m_ReplayRecorder.recordAction(pGameAction);
            // perform action
            Mainapp::seed(pGameAction->getSeed());
            Mainapp::setUseSeed(true);
            if (pMoveUnit != nullptr)
            {
                pMoveUnit->setMultiTurnPath(pGameAction->getMultiTurnPath());
            }
            m_CurrentActionUnit = pMoveUnit;
            pGameAction->perform();
            // clean up the action
            delete pGameAction;
            skipAnimations();
            if (pMap->getCurrentPlayer()->getIsDefeated())
            {
                GameAction* pAction = new GameAction();
                pAction->setActionID(CoreAI::ACTION_NEXT_PLAYER);
                performAction(pAction);
            }
        }
    }
    pApp->continueThread();
}

void GameMenue::skipAnimations()
{
    GameMap* pMap = GameMap::getInstance();
    if (GameAnimationFactory::getAnimationCount() == 0)
    {
        GameAnimationFactory::getInstance()->removeAnimation(nullptr);
    }
    else
    {
        qint32 skipAnimations = 0;
        GameEnums::AnimationMode animMode = Settings::getShowAnimations();
        switch (animMode)
        {
            case GameEnums::AnimationMode_OnlyBattleAll:
            case GameEnums::AnimationMode_OnlyBattleOwn:
            case GameEnums::AnimationMode_OnlyBattleAlly:
            case GameEnums::AnimationMode_OnlyBattleEnemy:
            {
                // only skip battle animations
                skipAnimations = 2;
                break;
            }
            case GameEnums::AnimationMode_None:
            {
                skipAnimations = 1;
                break;
            }
            case GameEnums::AnimationMode_All:
            {
                break;
            }
            case GameEnums::AnimationMode_Own:
            {
                // skip animations if the player isn't a human
                if (pMap->getCurrentPlayer()->getBaseGameInput()->getAiType() != GameEnums::AiTypes_Human)
                {
                    skipAnimations = 1;
                }
                break;
            }
            case GameEnums::AnimationMode_Ally:
            {
                Player* pPlayer1 = pMap->getCurrentPlayer();
                Player* pPlayer2 = getCurrentViewPlayer();
                // skip animations if the current player is an enemy of the current view player
                if (pPlayer2->isEnemy(pPlayer1))
                {
                    skipAnimations = 1;
                }
                break;
            }
            case GameEnums::AnimationMode_Enemy:
            {
                Player* pPlayer1 = pMap->getCurrentPlayer();
                Player* pPlayer2 = pMap->getCurrentViewPlayer();
                // skip animations if the current player is a human or it's an ally of the current view player
                if (pMap->getCurrentPlayer()->getBaseGameInput()->getAiType() == GameEnums::AiTypes_Human ||
                    pPlayer2->isAlly(pPlayer1))
                {
                    skipAnimations = 1;
                }
                break;
            }
        }
        // skip all animations
        if (skipAnimations == 1)
        {
            while (GameAnimationFactory::getAnimationCount() > 0)
            {
                GameAnimationFactory::finishAllAnimations();
            }
        }
        // only show battle animations
        else if (skipAnimations == 2)
        {
            bool battleActive = false;
            // skip all none selected battle animations
            while (GameAnimationFactory::getAnimationCount() > 0 && !battleActive)
            {
                qint32 i = 0;
                while (i < GameAnimationFactory::getAnimationCount())
                {
                    GameAnimation* pAnimation = GameAnimationFactory::getAnimation(i);
                    BattleAnimation* pBattleAnimation = dynamic_cast<BattleAnimation*>(pAnimation);
                    if (pBattleAnimation != nullptr)
                    {
                        Unit* pAtkUnit = pBattleAnimation->getAtkUnit();
                        Unit* pDefUnit = pBattleAnimation->getDefUnit();
                        if (animMode == GameEnums::AnimationMode_OnlyBattleAll)
                        {
                            battleActive = true;
                            i++;
                        }
                        else if (animMode == GameEnums::AnimationMode_OnlyBattleOwn)
                        {
                            // only show animation if at least one player is a human
                            if ((pAtkUnit->getOwner()->getBaseGameInput()->getAiType() == GameEnums::AiTypes_Human) ||
                                (pDefUnit != nullptr && pDefUnit->getOwner()->getBaseGameInput()->getAiType() == GameEnums::AiTypes_Human))
                            {
                                battleActive = true;
                                i++;
                            }
                        }
                        else if (animMode == GameEnums::AnimationMode_OnlyBattleAlly)
                        {
                            Player* pPlayer2 = pMap->getCurrentViewPlayer();
                            // only show animation if at least one player is an ally
                            if (pPlayer2->isAlly(pAtkUnit->getOwner()) ||
                                (pDefUnit != nullptr && pPlayer2->isAlly(pDefUnit->getOwner())))
                            {
                                battleActive = true;
                                i++;
                            }
                        }
                        else if (animMode == GameEnums::AnimationMode_OnlyBattleEnemy)
                        {
                            Player* pPlayer2 = pMap->getCurrentViewPlayer();
                            // only show animation if none of the players is human and all units are enemies of the current view player
                            if ((pAtkUnit->getOwner()->getBaseGameInput()->getAiType() != GameEnums::AiTypes_Human) &&
                                pDefUnit != nullptr &&
                                pDefUnit->getOwner()->getBaseGameInput()->getAiType() != GameEnums::AiTypes_Human &&
                                pPlayer2->isEnemy(pAtkUnit->getOwner()) &&
                                pPlayer2->isEnemy(pDefUnit->getOwner()))
                            {
                                battleActive = true;
                                i++;
                            }
                        }
                        // skip animation if it's not a battle animation we want
                        if (!battleActive)
                        {
                            if (!pAnimation->onFinished())
                            {
                                i++;
                            }
                        }
                    }
                    // skip other animations
                    else if (!pAnimation->onFinished())
                    {
                        i++;
                    }
                }
            }
        }
    }
}

void GameMenue::finishActionPerformed()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    if (m_CurrentActionUnit.get() != nullptr)
    {
        m_CurrentActionUnit->postAction();
        m_CurrentActionUnit = nullptr;
    }
    GameMap* pMap = GameMap::getInstance();
    pMap->getGameScript()->actionDone();
    pMap->getGameRules()->checkVictory();
    pMap->getGameRules()->createFogVision();
    pApp->continueThread();
}

void GameMenue::actionPerformed()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    finishActionPerformed();

    m_IngameInfoBar->updateTerrainInfo(m_Cursor->getMapPointX(), m_Cursor->getMapPointY(), true);
    m_IngameInfoBar->updateMinimap();
    m_IngameInfoBar->updatePlayerInfo();
    if (GameAnimationFactory::getAnimationCount() == 0)
    {
        if (m_pStoredAction != nullptr)
        {
            performAction(m_pStoredAction);
        }
        else
        {
            Mainapp::setUseSeed(false);
            GameMap* pMap = GameMap::getInstance();
            pMap->getGameRules()->resumeRoundTime();
            emit sigActionPerformed();
        }
    }
    pApp->continueThread();
}

void GameMenue::cursorMoved(qint32 x, qint32 y)
{
    if (xyTextInfo.get() != nullptr)
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->suspendThread();
        xyTextInfo->setHtmlText("X: " + QString::number(x) + " Y: " + QString::number(y));
        pApp->continueThread();
    }
}

void GameMenue::updatePlayerinfo()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_pPlayerinfo->updateData();
    m_IngameInfoBar->updatePlayerInfo();
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        pMap->getPlayer(i)->updateVisualCORange();
    }
    pApp->continueThread();
}

void GameMenue::victory(qint32 team)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    bool multiplayer = false;

    if (m_pNetworkInterface.get() != nullptr)
    {
        m_pChat->detach();
        m_pChat = nullptr;
        multiplayer = true;
        emit m_pNetworkInterface->sig_close();
        m_pNetworkInterface = nullptr;
    }

    bool exit = true;
    bool humanWin = false;
    // create victory
    if (team >= 0)
    {
        for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
        {
            Player* pPlayer = pMap->getPlayer(i);
            if (pPlayer->getTeam() != team)
            {
                pPlayer->defeatPlayer(nullptr);
            }
            if (pPlayer->getIsDefeated() == false && pPlayer->getBaseGameInput()->getAiType() == GameEnums::AiTypes_Human)
            {
                humanWin = true;
            }
        }
        exit = pMap->getGameScript()->victory(team);

        if (GameAnimationFactory::getAnimationCount() == 0)
        {
            exit = true;
        }
        else
        {
            exit = false;
        }
    }
    if (exit)
    {
        if (pMap->getCampaign() != nullptr)
        {
            pMap->getCampaign()->mapFinished(humanWin);
        }
        Console::print("Leaving Game Menue", Console::eDEBUG);
        oxygine::getStage()->addChild(new VictoryMenue(multiplayer));
        addRef();
        oxygine::Actor::detach();
        deleteLater();
    }
    pApp->continueThread();
}

void GameMenue::showAttackLog()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_Focused = false;

    spDialogAttackLog pAttackLog = new DialogAttackLog(GameMap::getInstance()->getCurrentPlayer());
    connect(pAttackLog.get(), &DialogAttackLog::sigFinished, [=]()
    {
        m_Focused = true;
    });
    addChild(pAttackLog);
    pApp->continueThread();
}

void GameMenue::showUnitInfo()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_Focused = false;

    spDialogUnitInfo pDialogUnitInfo = new DialogUnitInfo(GameMap::getInstance()->getCurrentPlayer());
    connect(pDialogUnitInfo.get(), &DialogUnitInfo::sigFinished, [=]()
    {
        m_Focused = true;
    });
    addChild(pDialogUnitInfo);
    pApp->continueThread();
}

void GameMenue::showGameInfo()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_Focused = false;
    QStringList header = {tr("Player"), tr("Produced"), tr("Lost"), tr("Killed"), tr("Income"), tr("Funds"), tr("Bases")};
    QVector<QStringList> data;
    GameMap* pMap = GameMap::getInstance();
    qint32 totalBuildings = pMap->getBuildingCount("");
    Player* pViewPlayer = pMap->getCurrentViewPlayer();
    if (pViewPlayer != nullptr)
    {
        for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
        {
            QString funds = QString::number(pMap->getPlayer(i)->getFunds());
            if (pViewPlayer->getTeam() != pMap->getPlayer(i)->getTeam() &&
                pMap->getGameRules()->getFogMode() != GameEnums::Fog_Off)
            {
                funds = "?";
            }
            qint32 buildingCount = pMap->getPlayer(i)->getBuildingCount();
            data.append({tr("Player ") + QString::number(i + 1),
                         QString::number(pMap->getGameRecorder()->getBuildedUnits(i)),
                         QString::number(pMap->getGameRecorder()->getLostUnits(i)),
                         QString::number(pMap->getGameRecorder()->getDestroyedUnits(i)),
                         QString::number(pMap->getPlayer(i)->calcIncome()),
                         funds,
                         QString::number(buildingCount)});
            totalBuildings -= buildingCount;
        }
        data.append({tr("Neutral"), "", "", "", "", "", QString::number(totalBuildings)});

        spGenericBox pGenericBox = new GenericBox();
        QSize size(Settings::getWidth() - 40, Settings::getHeight() - 80);
        spPanel pPanel = new Panel(true, size, size);
        pPanel->setPosition(20, 20);
        qint32 width = (Settings::getWidth() - 120 - 5 * (header.size() + 1)) / header.size();
        spTableView pTableView = new TableView(width * header.size() + 5 * (header.size() + 1), data, header, false);
        pTableView->setPosition(20, 20);
        pPanel->addItem(pTableView);
        pPanel->setContentHeigth(pTableView->getHeight() + 40);
        pGenericBox->addItem(pPanel);
        addChild(pGenericBox);
        connect(pGenericBox.get(), &GenericBox::sigFinished, [=]()
        {
            m_Focused = true;
        });
    }
    pApp->continueThread();
}

void GameMenue::showCOInfo()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();

    GameMap* pMap = GameMap::getInstance();
    spCOInfoDialog pCOInfoDialog = new COInfoDialog(pMap->getCurrentPlayer()->getspCO(0), pMap->getspPlayer(pMap->getCurrentPlayer()->getPlayerID()), [=](spCO& pCurrentCO, spPlayer& pPlayer, qint32 direction)
    {
        if (direction > 0)
        {
            if (pCurrentCO.get() == pPlayer->getCO(1) ||
                pPlayer->getCO(1) == nullptr)
            {
                // roll over case
                if (pPlayer->getPlayerID() == pMap->getPlayerCount() - 1)
                {
                    pPlayer = pMap->getspPlayer(0);
                    pCurrentCO = pPlayer->getspCO(0);
                }
                else
                {
                    pPlayer = pMap->getspPlayer(pPlayer->getPlayerID() + 1);
                    pCurrentCO = pPlayer->getspCO(0);
                }
            }
            else
            {
                pCurrentCO = pPlayer->getspCO(1);
            }
        }
        else
        {
            if (pCurrentCO.get() == pPlayer->getCO(0) ||
                pPlayer->getCO(0) == nullptr)
            {
                // select player
                if (pPlayer->getPlayerID() == 0)
                {
                    pPlayer = pMap->getspPlayer(pMap->getPlayerCount() - 1);
                }
                else
                {
                    pPlayer = pMap->getspPlayer(pPlayer->getPlayerID() - 1);
                }
                // select co
                if ( pPlayer->getCO(1) != nullptr)
                {
                    pCurrentCO = pPlayer->getspCO(1);
                }
                else
                {
                    pCurrentCO = pPlayer->getspCO(0);
                }
            }
            else
            {
                pCurrentCO = pPlayer->getspCO(0);
            }
        }
    }, true);
    addChild(pCOInfoDialog);
    setFocused(false);
    connect(pCOInfoDialog.get(), &COInfoDialog::quit, this, &GameMenue::editFinishedCanceled, Qt::QueuedConnection);
    pApp->continueThread();
}

void GameMenue::saveGame()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    QVector<QString> wildcards;
    if (m_pNetworkInterface.get() != nullptr ||
        m_Multiplayer)
    {
        wildcards.append("*.msav");
    }
    else
    {
        wildcards.append("*.sav");
    }
    QString path = QCoreApplication::applicationDirPath() + "/savegames";
    spFileDialog saveDialog = new FileDialog(path, wildcards, GameMap::getInstance()->getMapName());
    this->addChild(saveDialog);
    connect(saveDialog.get(), &FileDialog::sigFileSelected, this, &GameMenue::saveMap, Qt::QueuedConnection);
    setFocused(false);
    connect(saveDialog.get(), &FileDialog::sigCancel, this, &GameMenue::editFinishedCanceled, Qt::QueuedConnection);
    pApp->continueThread();
}

void GameMenue::showSaveAndExitGame()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    QVector<QString> wildcards;
    if (m_pNetworkInterface.get() != nullptr ||
        m_Multiplayer)
    {
        wildcards.append("*.msav");
    }
    else
    {
        wildcards.append("*.sav");
    }
    QString path = QCoreApplication::applicationDirPath() + "/savegames";
    spFileDialog saveDialog = new FileDialog(path, wildcards, GameMap::getInstance()->getMapName());
    this->addChild(saveDialog);
    connect(saveDialog.get(), &FileDialog::sigFileSelected, this, &GameMenue::saveMapAndExit, Qt::QueuedConnection);
    setFocused(false);
    connect(saveDialog.get(), &FileDialog::sigCancel, this, &GameMenue::editFinishedCanceled, Qt::QueuedConnection);
    pApp->continueThread();
}

void GameMenue::victoryInfo()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    spDialogVictoryConditions pVictoryConditions = new DialogVictoryConditions();
    addChild(pVictoryConditions);
    setFocused(false);
    connect(pVictoryConditions.get(), &DialogVictoryConditions::sigFinished, this, &GameMenue::editFinishedCanceled, Qt::QueuedConnection);
    pApp->continueThread();
}

void GameMenue::saveMap(QString filename)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    if (filename.endsWith(".sav") || filename.endsWith(".msav"))
    {
        QFile file(filename);
        file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        QDataStream stream(&file);
        GameMap* pMap = GameMap::getInstance();
        pMap->serializeObject(stream);
        file.close();
        Settings::setLastSaveGame(filename);
    }
    setFocused(true);
    pApp->continueThread();
}

void GameMenue::saveMapAndExit(QString filename)
{
    finishActionPerformed();
    saveMap(filename);
    exitGame();
}

void GameMenue::exitGame()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    gameStarted = false;
    while (GameAnimationFactory::getAnimationCount() > 0)
    {
        GameAnimationFactory::finishAllAnimations();
    }
    victory(-1);
    pApp->continueThread();
}

void GameMenue::startGame()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameAnimationFactory::clearAllAnimations();
    GameMap* pMap = GameMap::getInstance();
    if (!m_SaveGame)
    {
        pMap->startGame();
        pMap->setCurrentPlayer(GameMap::getInstance()->getPlayerCount() - 1);
        GameRules* pRules = pMap->getGameRules();
        pRules->init();
        updatePlayerinfo();
        m_ReplayRecorder.startRecording();
        GameAction* pAction = new GameAction(CoreAI::ACTION_NEXT_PLAYER);
        performAction(pAction);
    }
    else
    {
        pApp->getAudioThread()->clearPlayList();
        pMap->getCurrentPlayer()->loadCOMusic();
        pMap->updateUnitIcons();
        pMap->getGameRules()->createFogVision();
        pApp->getAudioThread()->playRandom();
        updatePlayerinfo();
        if ((m_pNetworkInterface.get() == nullptr ||
            m_pNetworkInterface->getIsServer()) &&
            !gameStarted)
        {
            emit sigActionPerformed();
        }
    }
    gameStarted = true;
    pApp->continueThread();
}

void GameMenue::keyInput(oxygine::KeyEvent event)
{
    InGameMenue::keyInput(event);
    // for debugging
    Qt::Key cur = event.getKey();
    if (m_Focused && m_pNetworkInterface.get() == nullptr)
    {
        if (cur == Settings::getKey_quicksave1())
        {
            saveMap("savegames/quicksave1.sav");
        }
        else if (cur == Settings::getKey_quicksave2())
        {
            saveMap("savegames/quicksave2.sav");
        }
        else if (cur == Settings::getKey_quickload1())
        {
            if (QFile::exists("savegames/quicksave1.sav"))
            {
                Mainapp* pApp = Mainapp::getInstance();
                pApp->suspendThread();
                Console::print("Leaving Game Menue", Console::eDEBUG);
                addRef();
                oxygine::Actor::detach();
                deleteLater();
                GameMenue* pMenue = new GameMenue("savegames/quicksave1.sav", true);
                oxygine::getStage()->addChild(pMenue);
                pApp->getAudioThread()->clearPlayList();
                pMenue->startGame();
                pApp->continueThread();
            }
        }
        else if (cur == Settings::getKey_quickload2())
        {
            if (QFile::exists("savegames/quicksave2.sav"))
            {
                Mainapp* pApp = Mainapp::getInstance();
                pApp->suspendThread();
                Console::print("Leaving Game Menue", Console::eDEBUG);
                addRef();
                oxygine::Actor::detach();
                deleteLater();
                GameMenue* pMenue = new GameMenue("savegames/quicksave1.sav", true);
                oxygine::getStage()->addChild(pMenue);
                pApp->getAudioThread()->clearPlayList();
                pMenue->startGame();
                pApp->continueThread();
            }
        }
        else
        {
            keyInputAll(cur);
        }
    }
    else if (m_Focused)
    {
        keyInputAll(cur);
    }
}

void GameMenue::keyInputAll(Qt::Key cur)
{
    if (cur == Qt::Key_Escape)
    {
        emit sigShowExitGame();
    }
    else if (cur == Settings::getKey_information())
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->suspendThread();
        GameMap* pMap = GameMap::getInstance();
        Player* pPlayer = pMap->getCurrentViewPlayer();
        GameEnums::VisionType visionType = pPlayer->getFieldVisibleType(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
        if (pMap->onMap(m_Cursor->getMapPointX(), m_Cursor->getMapPointY()) &&
            visionType != GameEnums::VisionType_Shrouded)
        {
            Terrain* pTerrain = pMap->getTerrain(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
            Unit* pUnit = pTerrain->getUnit();
            if (pUnit != nullptr && pUnit->isStealthed(pPlayer))
            {
                pUnit = nullptr;
            }
            spFieldInfo fieldinfo = new FieldInfo(pTerrain, pUnit);
            this->addChild(fieldinfo);
            connect(fieldinfo.get(), &FieldInfo::sigFinished, [=]
            {
                setFocused(true);
            });
            setFocused(false);
        }
        pApp->continueThread();
    }
}

qint64 GameMenue::getSyncCounter() const
{
    return m_syncCounter;
}

Chat* GameMenue::getChat() const
{
    return m_pChat.get();
}

void GameMenue::autoScroll()
{
    Mainapp* pApp = Mainapp::getInstance();
    if (QGuiApplication::focusWindow() == pApp &&
        m_Focused)
    {
        QPoint curPos = pApp->mapFromGlobal(pApp->cursor().pos());
        if ((curPos.y() > m_pPlayerinfo->getScaledHeight() && curPos.x() < autoScrollBorder.y()) || // mouse is below the co info part
            (curPos.y() < m_pPlayerinfo->getScaledHeight() && curPos.x() > autoScrollBorder.x() - autoScrollBorder.y() && curPos.x() < autoScrollBorder.x()) || // mouse is in the co info part
            (curPos.x() > autoScrollBorder.x())) // default case
        {
            InGameMenue::autoScroll();
        }
    }
}

void GameMenue::showExitGame()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_Focused = false;
    spDialogMessageBox pExit = new DialogMessageBox(tr("Do you want to exit the current game?"), true);
    connect(pExit.get(), &DialogMessageBox::sigOk, this, &GameMenue::exitGame, Qt::QueuedConnection);
    connect(pExit.get(), &DialogMessageBox::sigCancel, [=]()
    {
        m_Focused = true;
    });
    addChild(pExit);
    pApp->continueThread();
}

void GameMenue::showSurrenderGame()
{
    GameMap* pMap = GameMap::getInstance();
    if (pMap->getCurrentPlayer()->getBaseGameInput()->getAiType() == GameEnums::AiTypes::AiTypes_Human)
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->suspendThread();
        m_Focused = false;
        spDialogMessageBox pSurrender = new DialogMessageBox(tr("Do you want to surrender the current game?"), true);
        connect(pSurrender.get(), &DialogMessageBox::sigOk, this, &GameMenue::surrenderGame, Qt::QueuedConnection);
        connect(pSurrender.get(), &DialogMessageBox::sigCancel, [=]()
        {
            m_Focused = true;
        });
        addChild(pSurrender);
        pApp->continueThread();
    }
}

void GameMenue::surrenderGame()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameAction* pAction = new GameAction();
    pAction->setActionID("ACTION_SURRENDER_INTERNAL");
    performAction(pAction);
    m_Focused = true;
    pApp->continueThread();
}

void GameMenue::showNicknameUnit(qint32 x, qint32 y)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    spUnit pUnit = GameMap::getInstance()->getTerrain(x, y)->getUnit();
    if (pUnit.get() != nullptr)
    {
        spDialogTextInput pDialogTextInput = new DialogTextInput(tr("Nickname for the Unit:"), true, pUnit->getName());
        connect(pDialogTextInput.get(), &DialogTextInput::sigTextChanged, [=](QString value)
        {
            emit sigNicknameUnit(x, y, value);
        });
        connect(pDialogTextInput.get(), &DialogTextInput::sigCancel, [=]()
        {
            m_Focused = true;
        });
        addChild(pDialogTextInput);
        m_Focused = false;
    }
    pApp->continueThread();
}

void GameMenue::nicknameUnit(qint32 x, qint32 y, QString name)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameAction* pAction = new GameAction();
    pAction->setActionID("ACTION_NICKNAME_UNIT_INTERNAL");
    pAction->setTarget(QPoint(x, y));
    pAction->writeDataString(name);
    performAction(pAction);
    m_Focused = true;
    pApp->continueThread();
}
