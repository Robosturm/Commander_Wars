#include "menue/gamemenue.h"

#include "game/player.h"
#include "game/co.h"

#include "menue/victorymenue.h"
#include "coreengine/console.h"
#include "ai/proxyai.h"

#include "gameinput/humanplayerinput.h"

#include "game/gameanimationfactory.h"

#include "game/unitpathfindingsystem.h"

#include "resource_management/objectmanager.h"
#include "resource_management/fontmanager.h"

#include "objects/filedialog.h"

#include "objects/coinfodialog.h"

#include "objects/dialogvictoryconditions.h"
#include "objects/dialogconnecting.h"
#include "objects/dialogmessagebox.h"

#include "multiplayer/networkcommands.h"

#include "wiki/fieldinfo.h"

#include "ingamescriptsupport/genericbox.h"

#include "objects/tableview.h"

#include <QFile>

GameMenue* GameMenue::m_pInstance = nullptr;

GameMenue::GameMenue(spNetworkInterface pNetworkInterface, bool saveGame)
    : InGameMenue(),
      m_SaveGame(saveGame)
{
    m_pNetworkInterface = pNetworkInterface;
    if (m_pNetworkInterface.get() != nullptr)
    {
        GameMap* pMap = GameMap::getInstance();
        for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
        {
            Player* pPlayer = pMap->getPlayer(i);
            if (pPlayer->getBaseGameInput()->getAiType() == BaseGameInputIF::AiTypes::ProxyAi)
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
        spDialogConnecting pDialogConnecting = new DialogConnecting(tr("Waiting for Players"));
        addChild(pDialogConnecting);
        connect(pDialogConnecting.get(), &DialogConnecting::sigCancel, this, &GameMenue::exitGame, Qt::QueuedConnection);
        connect(this, &GameMenue::sigGameStarted, pDialogConnecting.get(), &DialogConnecting::connected, Qt::QueuedConnection);

    }
    else
    {
        gameStarted = true;
    }
    loadGameMenue();
    startGame();
}

GameMenue::GameMenue(QString map, bool saveGame)
    : InGameMenue(-1, -1, map),
      gameStarted(true),
      m_SaveGame(saveGame)

{    
    loadGameMenue();

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
                    gameStarted = true;
                    QByteArray sendData;
                    QDataStream sendStream(&sendData, QIODevice::WriteOnly);
                    sendStream << NetworkCommands::STARTGAME;
                    emit m_pNetworkInterface->sig_sendData(0, sendData, NetworkInterface::NetworkSerives::Multiplayer, false);
                    emit sigGameStarted();
                    emit sigActionPerformed();
                }
            }
        }
        else if (messageType == NetworkCommands::STARTGAME)
        {
            if (!m_pNetworkInterface->getIsServer())
            {
                gameStarted = true;
                emit sigGameStarted();
                emit sigActionPerformed();
            }
        }
    }
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

void GameMenue::disconnected(quint64)
{
    if (m_pNetworkInterface.get() != nullptr)
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->suspendThread();
        if (m_pNetworkInterface.get() != nullptr)
        {
            emit m_pNetworkInterface->sig_close();
            m_pNetworkInterface = nullptr;
        }
        gameStarted = false;
        spDialogMessageBox pDialogMessageBox = new DialogMessageBox(tr("A player has disconnected from the game! The game will now be stopped. You can save the game and reload the game to continue playing this map."));
        addChild(pDialogMessageBox);
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

    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    oxygine::spBox9Sprite pButtonBox = new oxygine::Box9Sprite();
    pButtonBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pButtonBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pButtonBox->setResAnim(pAnim);
    qint32 roundTime = pMap->getGameRules()->getRoundTimeMs();
    oxygine::TextStyle style = FontManager::getMainFont();
    style.color = oxygine::Color(255, 255, 255, 255);
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.multiline = false;
    m_CurrentRoundTime = new oxygine::TextField();
    m_CurrentRoundTime->setStyle(style);
    if (roundTime > 0)
    {
        pButtonBox->setSize(286 + 70, 50);
        m_CurrentRoundTime->setPosition(138 + 4, 10);
        pButtonBox->addChild(m_CurrentRoundTime);
        updateTimer();
    }
    else
    {
        pButtonBox->setSize(286, 50);
    }
    pButtonBox->setPosition((pApp->getSettings()->getWidth() - m_IngameInfoBar->getWidth()) / 2 - pButtonBox->getWidth() / 2 + 50, pApp->getSettings()->getHeight() - pButtonBox->getHeight() + 6);
    pButtonBox->setPriority(static_cast<qint16>(Mainapp::ZOrder::Objects));
    addChild(pButtonBox);
    oxygine::spButton saveGame = pObjectManager->createButton(tr("Save Game"), 130);
    saveGame->setPosition(8, 4);
    saveGame->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigSaveGame();
    });
    pButtonBox->addChild(saveGame);

    oxygine::spButton exitGame = pObjectManager->createButton(tr("Exit Game"), 130);
    exitGame->setPosition(pButtonBox->getWidth() - 138, 4);
    exitGame->addEventListener(oxygine::TouchEvent::CLICK, [=](oxygine::Event * )->void
    {
        emit sigShowExitGame();
    });
    pButtonBox->addChild(exitGame);

    m_UpdateTimer.setInterval(500);
    m_UpdateTimer.setSingleShot(false);
    m_UpdateTimer.start();
    connect(&m_UpdateTimer, &QTimer::timeout, this, &GameMenue::updateTimer, Qt::QueuedConnection);
    connect(pMap->getGameRules(), &GameRules::signalVictory, this, &GameMenue::victory, Qt::QueuedConnection);
    connect(pMap->getGameRules()->getRoundTimer(), &Timer::timeout, pMap, &GameMap::nextTurn, Qt::QueuedConnection);
    connect(pMap, &GameMap::signalExitGame, this, &GameMenue::showExitGame, Qt::QueuedConnection);
    connect(pMap, &GameMap::sigSurrenderGame, this, &GameMenue::showSurrenderGame, Qt::QueuedConnection);
    connect(pMap, &GameMap::signalSaveGame, this, &GameMenue::saveGame, Qt::QueuedConnection);
    connect(pMap, &GameMap::sigShowGameInfo, this, &GameMenue::showGameInfo, Qt::QueuedConnection);
    connect(this, &GameMenue::sigExitGame, this, &GameMenue::exitGame, Qt::QueuedConnection);
    connect(this, &GameMenue::sigShowExitGame, this, &GameMenue::showExitGame, Qt::QueuedConnection);
    connect(this, &GameMenue::sigShowSurrenderGame, this, &GameMenue::showSurrenderGame, Qt::QueuedConnection);
    connect(this, &GameMenue::sigSaveGame, this, &GameMenue::saveGame, Qt::QueuedConnection);
    connect(pMap, &GameMap::signalVictoryInfo, this, &GameMenue::victoryInfo, Qt::QueuedConnection);
    connect(pMap, &GameMap::signalShowCOInfo, this, &GameMenue::showCOInfo, Qt::QueuedConnection);
    connect(pMap, &GameMap::sigQueueAction, this, &GameMenue::performAction, Qt::QueuedConnection);
    connect(m_IngameInfoBar->getMinimap(), &Minimap::clicked, pMap, &GameMap::centerMap, Qt::QueuedConnection);
    connect(GameAnimationFactory::getInstance(), &GameAnimationFactory::animationsFinished, this, &GameMenue::actionPerformed, Qt::QueuedConnection);
    connect(m_Cursor.get(), &Cursor::sigCursorMoved, m_IngameInfoBar.get(), &IngameInfoBar::updateCursorInfo, Qt::QueuedConnection);
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
    m_CurrentRoundTime->setHtmlText(QTime::fromMSecsSinceStartOfDay(roundTime).toString("hh:mm:ss").toStdString().c_str());
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
    if (pGameAction->getActionID() == CoreAI::ACTION_NEXT_PLAYER ||
        pGameAction->getActionID() == CoreAI::ACTION_SWAP_COS)
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
                           QVector<QPoint> newPath = pfs.getClosestReachableMovePath(currentMultiTurnPath[0], movepoints, true);
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
                           pUnit->setMultiTurnPath(multiTurnPath);
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
    m_pStoredAction = nullptr;
    GameMap* pMap = GameMap::getInstance();
    pMap->getGameRules()->pauseRoundTime();
    if (!pGameAction->getIsLocal() &&
        (pMap->getCurrentPlayer()->getBaseGameInput()->getAiType() != BaseGameInputIF::AiTypes::ProxyAi))
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
                GameAction* pTrapAction = new GameAction("ACTION_TRAP");
                pTrapAction->setMovepath(trapPath, trapPathCost);
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
                    trapPathCost += pMoveUnit->getMovementCosts(point.x(), point.y());
                }
            }
        }
    }
    // send action to other players if needed
    if (!pGameAction->getIsLocal() && m_pNetworkInterface.get() != nullptr &&
        pMap->getCurrentPlayer()->getBaseGameInput()->getAiType() != BaseGameInputIF::AiTypes::ProxyAi)
    {
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << pMap->getCurrentPlayer()->getPlayerID();
        pGameAction->serializeObject(stream);
        emit m_pNetworkInterface->sig_sendData(0, data, NetworkInterface::NetworkSerives::Game, true);
    }
    // perform action
    Mainapp::seed(pGameAction->getSeed());
    Mainapp::setUseSeed(true);
    pGameAction->perform();
    // clean up the action
    delete pGameAction;
    if (GameAnimationFactory::getAnimationCount() == 0)
    {
        GameAnimationFactory::getInstance()->removeAnimation(nullptr);
    }
    else
    {
        bool skipAnimations = false;
        switch (Settings::getShowAnimations())
        {
            case GameEnums::AnimationMode_None:
            {
                skipAnimations = true;
                break;
            }
            case GameEnums::AnimationMode_All:
            {
                break;
            }
            case GameEnums::AnimationMode_Own:
            {
                if (pMap->getCurrentPlayer()->getBaseGameInput()->getAiType() != BaseGameInputIF::AiTypes::Human)
                {
                    skipAnimations = true;
                }
                break;
            }
            case GameEnums::AnimationMode_Ally:
            {
                Player * pPlayer1 = pMap->getCurrentPlayer();
                Player * pPlayer2 = pMap->getCurrentViewPlayer();
                if (pPlayer2->isEnemy(pPlayer1))
                {
                    skipAnimations = true;
                }
                break;
            }
            case GameEnums::AnimationMode_Enemy:
            {
                if (pMap->getCurrentPlayer()->getBaseGameInput()->getAiType() == BaseGameInputIF::AiTypes::Human)
                {
                    skipAnimations = true;
                }
                break;
            }
        }
        if (skipAnimations)
        {
            while (GameAnimationFactory::getAnimationCount() > 0)
            {
                GameAnimationFactory::finishAllAnimations();
            }
        }
    }

    if (pMap->getCurrentPlayer()->getIsDefeated())
    {
        GameAction* pAction = new GameAction();
        pAction->setActionID(CoreAI::ACTION_NEXT_PLAYER);
        performAction(pAction);
    }
    pApp->continueThread();
}

void GameMenue::actionPerformed()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_IngameInfoBar->updateTerrainInfo(m_Cursor->getMapPointX(), m_Cursor->getMapPointY(), true);
    m_IngameInfoBar->updateMinimap();
    m_IngameInfoBar->updatePlayerInfo();
    GameMap* pMap = GameMap::getInstance();
    pMap->getGameScript()->actionDone();
    pMap->getGameRules()->checkVictory();
    pMap->getGameRules()->createFogVision();
    if (GameAnimationFactory::getAnimationCount() == 0)
    {
        if (m_pStoredAction != nullptr)
        {
            performAction(m_pStoredAction);
        }
        else
        {
            Mainapp::setUseSeed(false);
            pMap->getGameRules()->resumeRoundTime();
            emit sigActionPerformed();
        }
    }
    pApp->continueThread();
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
            if (pPlayer->getIsDefeated() == false && pPlayer->getBaseGameInput()->getAiType() == BaseGameInputIF::AiTypes::Human)
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
        oxygine::Actor::detach();
    }
    pApp->continueThread();
}

void GameMenue::showGameInfo()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    m_Focused = false;
    QStringList header = {tr("Player"), tr("Produced"), tr("Lost"), tr("Income"), tr("Funds"), tr("Bases")};
    QVector<QStringList> data;
    GameMap* pMap = GameMap::getInstance();
    qint32 totalBuildings = pMap->getBuildingCount("");
    Player* pViewPlayer = pMap->getCurrentViewPlayer();
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
                     QString::number(pMap->getGameRecorder()->getDestroyedUnits(i)),
                     QString::number(pMap->getPlayer(i)->calcIncome()),
                     funds,
                     QString::number(buildingCount)});
        totalBuildings -= buildingCount;
    }
    data.append({tr("Neutral"), "", "", "", "", QString::number(totalBuildings)});

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
    if (m_pNetworkInterface.get() != nullptr)
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
        pRules->changeWeather(pRules->getWeather(pRules->getStartWeather())->getWeatherId(), pMap->getPlayerCount() + 1);
        pMap->nextTurn();
        updatePlayerinfo();
    }
    else
    {
        pApp->getAudioThread()->clearPlayList();
        pMap->getCurrentPlayer()->loadCOMusic();
        pMap->updateUnitIcons();
        pMap->getGameRules()->createFogVision();        
        pApp->getAudioThread()->playRandom();
        updatePlayerinfo();
        emit sigActionPerformed();
    }
    pApp->continueThread();
}

void GameMenue::keyInput(SDL_Event event)
{
    InGameMenue::keyInput(event);
    // for debugging
    SDL_Keycode cur = event.key.keysym.sym;
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
                oxygine::Actor::detach();
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
                oxygine::Actor::detach();
                GameMenue* pMenue = new GameMenue("savegames/quicksave1.sav", true);
                oxygine::getStage()->addChild(pMenue);
                pApp->getAudioThread()->clearPlayList();
                pMenue->startGame();
                pApp->continueThread();
            }
        }
        else if (cur == Settings::getKey_information())
        {
            Mainapp* pApp = Mainapp::getInstance();
            pApp->suspendThread();
            GameMap* pMap = GameMap::getInstance();
            if (pMap->onMap(m_Cursor->getMapPointX(), m_Cursor->getMapPointY()))
            {
                Terrain* pTerrain = pMap->getTerrain(m_Cursor->getMapPointX(), m_Cursor->getMapPointY());
                spFieldInfo fieldinfo = new FieldInfo(pTerrain, pTerrain->getUnit());
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
    else if (m_Focused)
    {
        if (cur == SDLK_ESCAPE)
        {
            emit sigShowExitGame();
        }
    }
}

void GameMenue::autoScroll()
{
    if (SDL_GetWindowFlags(oxygine::core::getWindow()) & SDL_WINDOW_MOUSE_FOCUS &&
        m_Focused)
    {
        qint32 curX = 0;
        qint32 curY = 0;
        SDL_GetMouseState(&curX, &curY);
        if ((curY > m_pPlayerinfo->getScaledHeight() && curX < autoScrollBorder.y()) || // mouse is below the co info part
            (curY < m_pPlayerinfo->getScaledHeight() && curX > autoScrollBorder.x() - autoScrollBorder.y() && curX < autoScrollBorder.x()) || // mouse is in the co info part
            (curX > autoScrollBorder.x())) // default case
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

void GameMenue::surrenderGame()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
    GameMap* pMap = GameMap::getInstance();
    pMap->getCurrentPlayer()->defeatPlayer(nullptr);
    GameAction* pAction = new GameAction();
    pAction->setActionID(CoreAI::ACTION_NEXT_PLAYER);
    performAction(pAction);
    m_Focused = true;
    pApp->continueThread();
}

