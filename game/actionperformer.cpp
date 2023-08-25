#include "game/actionperformer.h"
#include "game/gameanimation/gameanimationfactory.h"
#include "ai/coreai.h"
#include "ai/dummyai.h"

#include "coreengine/mainapp.h"

#include "menue/gamemenue.h"

#include "objects/dialogs/dialogconnecting.h"

ActionPerformer::ActionPerformer(spWeakGameMap pMap, oxygine::spWeakEventDispatcher pMenu)
    : m_pMenu(pMenu),
      m_pMap(pMap)
{
    Interpreter::setCppOwnerShip(this);
    m_delayedActionPerformedTimer.setSingleShot(true);
    connect(&m_delayedActionPerformedTimer, &QTimer::timeout, this, &ActionPerformer::delayedActionPerformed, Qt::QueuedConnection);
    connect(this, &ActionPerformer::sigPerformAction, this, &ActionPerformer::performAction, Qt::QueuedConnection);
}

bool ActionPerformer::getActionRunning() const
{
    return m_actionRunning;
}

void ActionPerformer::setSyncCounter(qint64 counter)
{
    m_syncCounter = counter;
}

void ActionPerformer::performAction(spGameAction pGameAction, bool fromAiPipe)
{
    CONSOLE_PRINT("Start running action " + pGameAction->getActionID(), GameConsole::eDEBUG);
    spGameMap pMap = m_pMap.lock();
    spGameMenue pMenu = oxygine::safeSpCast<GameMenue>(m_pMenu.lock());
    if (pMap.get() == nullptr)
    {
        return;
    }

    auto mapHash = pMap->getMapHash();
    if (m_exit)
    {
        if (pMenu.get() != nullptr)
        {
            emit pMenu->sigVictory(-1);
        }
        return;
    }
    else if (m_actionRunning)
    {
        CONSOLE_PRINT("Ignoring action request cause an action is currently performed", GameConsole::eWARNING);
        return;
    }
    else if (m_mapHash.length() > 0 && m_mapHash != mapHash)
    {
        CONSOLE_PRINT("An interaction changed the map state outside performing actions.", GameConsole::eERROR);
    }
    m_actionRunning = true;
    bool autosave = true;
    if (pMenu != nullptr)
    {
        pMenu->setSaveAllowed(false);
    }
    if (m_multiplayerSyncData.m_waitingForSyncFinished && pMenu != nullptr)
    {
        m_multiplayerSyncData.m_postSyncAction = pGameAction;
        spDialogConnecting pDialogConnecting = MemoryManagement::create<DialogConnecting>(tr("Waiting for Players/Observers to join..."), 1000 * 60 * 5);
        pMenu->addChild(pDialogConnecting);
        connect(pDialogConnecting.get(), &DialogConnecting::sigCancel, pMenu.get(), &GameMenue::exitGame, Qt::QueuedConnection);
        connect(pMenu.get(), &GameMenue::sigSyncFinished, pDialogConnecting.get(), &DialogConnecting::connected, Qt::QueuedConnection);
    }
    else if (pGameAction.get() != nullptr)
    {
        Player* pCurrentPlayer = pMap->getCurrentPlayer();
        auto* baseGameInput = pCurrentPlayer->getBaseGameInput();
        bool proxyAi = baseGameInput != nullptr && (baseGameInput->getAiType() == GameEnums::AiTypes_ProxyAi || fromAiPipe || dynamic_cast<DummyAi*>(baseGameInput) != nullptr);
        CONSOLE_PRINT("GameMenue::performAction " + pGameAction->getActionID() + " at X: " + QString::number(pGameAction->getTarget().x())
                      + " at Y: " + QString::number(pGameAction->getTarget().y()) +
                      " is proxy ai " + QString::number(proxyAi), GameConsole::eDEBUG);
        Mainapp::getInstance()->pauseRendering();
        bool multiplayer = false;
        if (pMenu != nullptr)
        {
            multiplayer = pMenu->getIsMultiplayer(pGameAction);
        }
        bool asyncMatch =  false;
        bool invalidHash = false;
        if (proxyAi)
        {
            CONSOLE_PRINT("Validating game state", GameConsole::eDEBUG);
            asyncMatch = m_syncCounter + 1 != pGameAction->getSyncCounter();
            invalidHash = pGameAction->getMapHash() != mapHash;
        }
        if (asyncMatch || invalidHash)
        {
            if (asyncMatch)
            {
                CONSOLE_PRINT("Async found cause action sync counter " + QString::number(pGameAction->getSyncCounter()) + " doesn't match map sync counter " + QString::number(m_syncCounter + 1), GameConsole::eDEBUG);
            }
            else
            {
                CONSOLE_PRINT("Async found cause map hash is different from action map hash", GameConsole::eDEBUG);
            }
            if (multiplayer &&
                pMenu != nullptr)
            {
                autosave = false;
                pMenu->doResyncGame();
            }
        }
        if ((!asyncMatch && !invalidHash) || !multiplayer)
        {            
            // perform action
            GlobalUtils::seed(pGameAction->getSeed());
            GlobalUtils::setUseSeed(true);
            if (proxyAi)
            {
                m_syncCounter = pGameAction->getSyncCounter();
            }
            else if (!pGameAction->getIsLocal())
            {
                ++m_syncCounter;
            }
            CONSOLE_PRINT("Updated sync counter to " + QString::number(m_syncCounter), GameConsole::eDEBUG);
            m_pStoredAction.reset();
            pMap->getGameRules()->pauseRoundTime();
            if (!pGameAction->getIsLocal() &&
                baseGameInput != nullptr &&
                !Settings::getInstance()->getAiSlave() &&
                baseGameInput->getAiType() != GameEnums::AiTypes_ProxyAi)
            {
                pGameAction = doMultiTurnMovement(pGameAction);
            }
            Unit * pMoveUnit = pGameAction->getTargetUnit();
            doTrapping(pGameAction);
            // send action to other players if needed
            pGameAction->setSyncCounter(m_syncCounter);
            pGameAction->setMapHash(mapHash);
            pGameAction->setRoundTimerTime(pMap->getGameRules()->getRoundTimer()->remainingTime());
            if (!pGameAction->getIsLocal() &&
                !fromAiPipe &&
                pMenu != nullptr &&
                pMenu->getGameStarted())
            {
                CONSOLE_PRINT("Sending action to ai pipe", GameConsole::eDEBUG);
                emit sigAiProcesseSendAction(pGameAction);
            }
            if (requiresForwarding(pGameAction))
            {
                CONSOLE_PRINT("Sending action to other players with sync counter " + QString::number(m_syncCounter), GameConsole::eDEBUG);
                QByteArray data;
                QDataStream stream(&data, QIODevice::WriteOnly);
                stream.setVersion(QDataStream::Version::Qt_6_5);
                stream << pMap->getCurrentPlayer()->getPlayerID();
                pGameAction->serializeObject(stream);
                emit pMenu->getNetworkInterface()->sig_sendData(0, data, NetworkInterface::NetworkSerives::Game, true);
            }
            else if (multiplayer)
            {
                pMap->getGameRules()->getRoundTimer()->setInterval(pGameAction->getRoundTimerTime());
            }

            // record action if required
            if (pMenu != nullptr)
            {
                pMenu->getReplayRecorder().recordAction(pGameAction);
            }
            if (pMoveUnit != nullptr)
            {
                pMoveUnit->setMultiTurnPath(pGameAction->getMultiTurnPath());
            }

            if (baseGameInput != nullptr)
            {
                baseGameInput->centerCameraOnAction(pGameAction.get());
            }
            pGameAction->perform();
            // clean up the action
            m_pCurrentAction = pGameAction;
            pGameAction.reset();
            skipAnimations(false);
        }
        if (pCurrentPlayer != pMap->getCurrentPlayer() &&
            pMenu != nullptr &&
            autosave)
        {
            auto* baseGameInput = pMap->getCurrentPlayer()->getBaseGameInput();
            if (baseGameInput != nullptr &&
                baseGameInput->getAiType() == GameEnums::AiTypes_Human)
            {
                pMenu->autoSaveMap();
            }
        }
        Mainapp::getInstance()->continueRendering();
    }
}

bool ActionPerformer::requiresForwarding(const spGameAction & pGameAction) const
{
    spGameMap pMap = m_pMap.lock();
    spGameMenue pMenu = oxygine::safeSpCast<GameMenue>(m_pMenu.lock());
    if (pMap.get() == nullptr)
    {
        return false;
    }
    Player* pCurrentPlayer = pMap->getCurrentPlayer();
    auto* baseGameInput = pCurrentPlayer->getBaseGameInput();
    return pMenu != nullptr &&
           pMenu->getIsMultiplayer(pGameAction) &&
           baseGameInput != nullptr &&
           baseGameInput->getAiType() != GameEnums::AiTypes_ProxyAi &&
                                         !pGameAction->getIsLocal();
}

void ActionPerformer::setActionRunning(bool newActionRunning)
{
    m_actionRunning = newActionRunning;
}

bool ActionPerformer::getExit() const
{
    return m_exit;
}

void ActionPerformer::setExit(bool newExit)
{
    m_exit = newExit;
}

spGameAction ActionPerformer::doMultiTurnMovement(spGameAction pGameAction)
{
    spGameMenue pMenu = oxygine::safeSpCast<GameMenue>(m_pMenu.lock());
    spGameMap pMap = m_pMap.lock();
    if (pMenu != nullptr &&
        pMenu->getGameStarted() &&
        pMap != nullptr &&
        !Settings::getInstance()->getAiSlave() &&
        pGameAction.get() != nullptr &&
        (pGameAction->getActionID() == CoreAI::ACTION_NEXT_PLAYER ||
         pGameAction->getActionID() == CoreAI::ACTION_SWAP_COS))
    {
        CONSOLE_PRINT("Check and update multiTurnMovement" + pGameAction->getActionID(), GameConsole::eDEBUG);

        // check for units that have a multi turn available
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
                            qint32 movepoints = pUnit->getMovementpoints(pUnit->getPosition());
                            if (movepoints > 0)
                            {
                                // shorten path
                                UnitPathFindingSystem pfs(pMap.get(), pUnit, pPlayer);
                                pfs.setMovepoints(pUnit->getFuel());
                                pfs.explore();
                                auto newPath = pfs.getClosestReachableMovePath(currentMultiTurnPath, movepoints);
                                if (newPath.size() > 0)
                                {
                                    CONSOLE_PRINT("Replacing action with multiTurnMovement action", GameConsole::eDEBUG);
                                    // replace current action with auto moving none moved units
                                    m_pStoredAction = pGameAction;
                                    spGameAction multiTurnMovement = MemoryManagement::create<GameAction>(CoreAI::ACTION_WAIT, pMap.get());
                                    if (pUnit->getActionList().contains(CoreAI::ACTION_HOELLIUM_WAIT))
                                    {
                                        multiTurnMovement->setActionID(CoreAI::ACTION_HOELLIUM_WAIT);
                                    }
                                    multiTurnMovement->setTarget(pUnit->getPosition());
                                    multiTurnMovement->setMovepath(newPath, pfs.getCosts(newPath));
                                    QVector<QPoint> multiTurnPath;
                                    // still some path ahead?
                                    multiTurnPath = QVector<QPoint>();
                                    if (currentMultiTurnPath.size() > newPath.size())
                                    {
                                        for (qint32 i = 0; i <= currentMultiTurnPath.size() - newPath.size(); i++)
                                        {
                                            multiTurnPath.append(currentMultiTurnPath[i]);
                                        }
                                    }
                                    multiTurnMovement->setMultiTurnPath(multiTurnPath);
                                    return multiTurnMovement;
                                }
                            }
                        }
                        else if (pUnit->getActionList().contains(CoreAI::ACTION_CAPTURE))
                        {
                            spGameAction multiTurnMovement = MemoryManagement::create<GameAction>(CoreAI::ACTION_CAPTURE, pMap.get());
                            multiTurnMovement->setTarget(pUnit->getPosition());
                            if (multiTurnMovement->canBePerformed())
                            {
                                CONSOLE_PRINT("Replacing action with capture action", GameConsole::eDEBUG);
                                m_pStoredAction = pGameAction;
                                return multiTurnMovement;
                            }
                        }
                    }
                }
            }
        }
    }
    return pGameAction;
}

void ActionPerformer::centerMapOnAction(GameAction* pGameAction)
{
    spGameMap pMap = m_pMap.lock();
    spGameMenue pMenu = oxygine::safeSpCast<GameMenue>(m_pMenu.lock());
    if (pMenu != nullptr &&
        pMap != nullptr)
    {
        CONSOLE_PRINT("centerMapOnAction()", GameConsole::eDEBUG);
        Unit* pUnit = pGameAction->getTargetUnit();
        Player* pPlayer = pMenu->getCurrentViewPlayer();

        QPoint target = pGameAction->getTarget();
        if (pUnit != nullptr &&
            !pUnit->isStealthed(pPlayer))
        {
            const auto & path = pGameAction->getMovePath();
            for (const auto & point : path)
            {
                if (pPlayer->getFieldVisible(point.x(), point.y()))
                {
                    target = point;
                    break;
                }
            }
        }

        if (pMap->onMap(target.x(), target.y()) &&
            pPlayer->getFieldVisible(target.x(), target.y()) &&
            (pUnit == nullptr ||
             !pUnit->isStealthed(pPlayer)))
        {
            pMap->centerMap(target.x(), target.y());
        }
    }
}

void ActionPerformer::skipAnimations(bool postAnimation)
{
    CONSOLE_PRINT("skipping Animations", GameConsole::eDEBUG);
    Mainapp::getInstance()->pauseRendering();
    if (GameAnimationFactory::getAnimationCount() > 0)
    {
        GameAnimationFactory::skipAllAnimations();
    }
    if (GameAnimationFactory::getAnimationCount() == 0 && !postAnimation)
    {
        CONSOLE_PRINT("GameMenue -> emitting animationsFinished()", GameConsole::eDEBUG);
        emit GameAnimationFactory::getInstance()->animationsFinished();
    }
    Mainapp::getInstance()->continueRendering();
}

void ActionPerformer::finishActionPerformed()
{
    CONSOLE_PRINT("Doing post action update", GameConsole::eDEBUG);
    m_finishedPerformed = true;
    spGameMap pMap = m_pMap.lock();
    if (m_pCurrentAction.get() != nullptr &&
        pMap != nullptr)
    {
        Unit* pUnit = m_pCurrentAction->getMovementTarget();
        if (pUnit != nullptr)
        {
            pUnit->postAction(m_pCurrentAction.get());
        }
        pMap->getCurrentPlayer()->postAction(m_pCurrentAction.get());
        pMap->getGameScript()->actionDone(m_pCurrentAction);
        m_pCurrentAction.reset();
    }
    skipAnimations(true);
    spGameMenue pMenu = oxygine::safeSpCast<GameMenue>(m_pMenu.lock());
    if (pMenu != nullptr)
    {
        pMenu->updateQuickButtons();
    }
}

void ActionPerformer::actionPerformed()
{
    spGameMap pMap = m_pMap.lock();
    spGameMenue pMenu = oxygine::safeSpCast<GameMenue>(m_pMenu.lock());
    if (pMenu != nullptr &&
        pMap != nullptr)
    {
        if (GameAnimationFactory::getAnimationCount() == 0)
        {
            CONSOLE_PRINT("Action performed", GameConsole::eDEBUG);
            if (!m_finishedPerformed)
            {
                finishActionPerformed();
            }
            if (Settings::getInstance()->getSyncAnimations())
            {
                pMap->syncUnitsAndBuildingAnimations();
            }
            if (pMenu != nullptr)
            {
                pMenu->updateGameInfo();
            }
            if (GameAnimationFactory::getAnimationCount() == 0)
            {
                CONSOLE_PRINT("Action finished", GameConsole::eDEBUG);
                pMap->killDeadUnits();
                pMap->getGameRules()->checkVictory();
                pMap->getGameRules()->createFogVision();
                m_actionRunning = false;
                m_finishedPerformed = false;
                CONSOLE_PRINT("Storing current map hash", GameConsole::eDEBUG);
                m_mapHash = pMap->getMapHash();
                GlobalUtils::setUseSeed(false);
                if (pMenu->getIndespawningMode())
                {
                    pMenu->setSaveAllowed(true);
                    pMenu->doDespawnSlave();
                }
                else if (m_exit &&
                         pMenu != nullptr)
                {
                    CONSOLE_PRINT("ActionPerformer state is exiting game. Emitting exit", GameConsole::eDEBUG);
                    emit pMenu->sigVictory(-1);
                }
                else if (!pMap->getGameRules()->getVictory())
                {
                    if (!pMap->anyPlayerAlive())
                    {
                        CONSOLE_PRINT("Forcing exiting the game cause no player is alive", GameConsole::eDEBUG);
                        emit pMenu->sigExitGame();
                    }
                    else if (pMap->getCurrentPlayer()->getIsDefeated())
                    {
                        onTriggeringActionFinished();
                        CONSOLE_PRINT("Triggering next player cause current player is defeated", GameConsole::eDEBUG);
                        spGameAction pAction = MemoryManagement::create<GameAction>(CoreAI::ACTION_NEXT_PLAYER, pMap.get());
                        performAction(pAction);
                    }
                    else if (m_pStoredAction.get() != nullptr)
                    {
                        onTriggeringActionFinished();
                        performAction(m_pStoredAction);
                    }
                    else
                    {
                        if (pMap->getCurrentPlayer()->getBaseGameInput()->getAiType() != GameEnums::AiTypes_ProxyAi)
                        {
                            pMap->getGameRules()->resumeRoundTime();
                        }
                        if (m_noTimeOut &&
                            !Settings::getInstance()->getAiSlave())
                        {
                            onTriggeringActionFinished();
                            spGameAction pAction = MemoryManagement::create<GameAction>(CoreAI::ACTION_NEXT_PLAYER, pMap.get());
                            performAction(pAction);
                            m_noTimeOut = false;
                        }
                        else
                        {
                            CONSOLE_PRINT("emitting sigActionPerformed()", GameConsole::eDEBUG);
                            quint32 delay = Settings::getInstance()->getPauseAfterAction();
                            if (delay == 0)
                            {
                                onTriggeringActionFinished();
                                emit sigActionPerformed();
                            }
                            else
                            {
                                m_delayedActionPerformedTimer.start(std::chrono::seconds(delay));
                            }
                        }
                    }
                }
                else
                {
                    auto winner = pMap->getGameRules()->getVictoryTeam();
                    CONSOLE_PRINT("Game already won  retriggering victory for team " + QString::number(winner), GameConsole::eDEBUG);
                    emit pMenu->sigVictory(winner);
                }
            }
            else
            {
                CONSOLE_PRINT("Skipping action performed cause finishActionPerformed added animations", GameConsole::eDEBUG);
            }
        }
        else
        {
            CONSOLE_PRINT("Animation finish error. Cause following animations are still active", GameConsole::eERROR);
            GameAnimationFactory::printActiveAnimations();
        }
    }
    else
    {
        CONSOLE_PRINT("Skipping action performed", GameConsole::eDEBUG);
    }
}

void ActionPerformer::onTriggeringActionFinished()
{
    spGameMenue pMenu = oxygine::safeSpCast<GameMenue>(m_pMenu.lock());
    if (pMenu != nullptr)
    {
        CONSOLE_PRINT("ActionPerformer::onTriggeringActionFinished", GameConsole::eDEBUG);
        pMenu->setSaveAllowed(true);
        if (pMenu->getSaveMap())
        {
            pMenu->doSaveMap();
        }
        pMenu->sendOnlineInfo();
    }
}

void ActionPerformer::nextTurnPlayerTimeout()
{    
    spGameMap pMap = m_pMap.lock();
    if (pMap.get() != nullptr)
    {
        auto* input = pMap->getCurrentPlayer()->getBaseGameInput();
        if (input != nullptr &&
            !Settings::getInstance()->getAiSlave())
        {
            if (input->getAiType() == GameEnums::AiTypes_Human)
            {
                if (m_pCurrentAction.get() == nullptr)
                {
                    spGameAction pAction = MemoryManagement::create<GameAction>(CoreAI::ACTION_NEXT_PLAYER, pMap.get());
                    performAction(pAction);
                }
                else
                {
                    m_noTimeOut = true;
                }
            }
            else if (input->getAiType() != GameEnums::AiTypes_Human &&
                     input->getAiType() != GameEnums::AiTypes_ProxyAi &&
                     input->getAiType() != GameEnums::AiTypes_Closed)
            {
                m_noTimeOut = true;
            }
        }
    }
}

qint64 ActionPerformer::getSyncCounter() const
{
    return m_syncCounter;
}

void ActionPerformer::doTrapping(spGameAction & pGameAction)
{
    CONSOLE_PRINT("GameMenue::doTrapping", GameConsole::eDEBUG);
    spGameMap pMap = m_pMap.lock();
    if (pMap.get() != nullptr)
    {
        QVector<QPoint> path = pGameAction->getMovePath();

        Unit * pMoveUnit = pGameAction->getTargetUnit();
        if (path.size() > 1 && pMoveUnit != nullptr)
        {
            if (pGameAction->getRequiresEmptyField())
            {
                QVector<QPoint> trapPathNotEmptyTarget = path;
                qint32 trapPathCostNotEmptyTarget = pGameAction->getCosts();
                QPoint trapPoint = path[0];
                for (qint32 i = 0; i < path.size() - 1; i++)
                {
                    QPoint point = path[i];
                    QPoint prevPoint = path[i + 1];
                    Unit* pUnit = pMap->getTerrain(point.x(), point.y())->getUnit();
                    if (pUnit == nullptr || pMoveUnit->getOwner()->isAlly(pUnit->getOwner()))
                    {
                        if (i > 0)
                        {
                            spGameAction pTrapAction = MemoryManagement::create<GameAction>(CoreAI::ACTION_TRAP, pMap.get());
                            pTrapAction->setMovepath(trapPathNotEmptyTarget, trapPathCostNotEmptyTarget);
                            pTrapAction->writeDataInt32(trapPoint.x());
                            pTrapAction->writeDataInt32(trapPoint.y());
                            pTrapAction->setTarget(pGameAction->getTarget());
                            pGameAction = pTrapAction;
                        }
                        break;
                    }
                    else
                    {
                        trapPoint = point;
                        qint32 moveCost = pMoveUnit->getMovementCosts(point.x(), point.y(),
                                                                      prevPoint.x(), prevPoint.y());
                        trapPathCostNotEmptyTarget -= moveCost;
                        trapPathNotEmptyTarget.removeFirst();
                    }
                }
            }
            path = pGameAction->getMovePath();
            QVector<QPoint> trapPath;
            qint32 trapPathCost = 0;
            for (qint32 i = path.size() - 2; i >= 0; i--)
            {
                // check the movepath for a trap
                QPoint point = path[i];
                QPoint prevPoint = path[i];
                if (i > 0)
                {
                    prevPoint = path[i - 1];
                }
                qint32 moveCost = pMoveUnit->getMovementCosts(point.x(), point.y(),
                                                              prevPoint.x(), prevPoint.y(), true);
                if (isTrap("isTrap", pGameAction, pMoveUnit, point, prevPoint, moveCost))
                {
                    while (trapPath.size() > 1)
                    {
                        QPoint currentPoint = trapPath[0];
                        QPoint previousPoint = trapPath[1];
                        moveCost = pMoveUnit->getMovementCosts(currentPoint.x(), currentPoint.y(),
                                                               previousPoint.x(), previousPoint.y());
                        if (isTrap("isStillATrap", pGameAction, pMoveUnit, currentPoint, previousPoint, moveCost))
                        {
                            trapPathCost -= moveCost;
                            trapPath.pop_front();
                            if (pMap->getTerrain(point.x(), point.y())->getUnit() != nullptr)
                            {
                                point = currentPoint;
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    spGameAction pTrapAction = MemoryManagement::create<GameAction>(CoreAI::ACTION_TRAP, pMap.get());
                    pTrapAction->setMovepath(trapPath, trapPathCost);
                    pMoveUnit->getOwner()->addVisionField(point.x(), point.y(), 1, true);
                    pTrapAction->writeDataInt32(point.x());
                    pTrapAction->writeDataInt32(point.y());
                    pTrapAction->setTarget(pGameAction->getTarget());
                    pGameAction = pTrapAction;
                    break;
                }
                else
                {
                    trapPath.push_front(point);
                    qint32 x = pMoveUnit->Unit::getX();
                    qint32 y = pMoveUnit->Unit::getY();
                    if (point.x() != x ||
                        point.y() != y)
                    {
                        QPoint previousPoint = path[i + 1];
                        trapPathCost += pMoveUnit->getMovementCosts(point.x(), point.y(), previousPoint.x(), previousPoint.y());
                    }
                }
            }
        }
    }
}

bool ActionPerformer::isTrap(const QString & function, spGameAction pAction, Unit* pMoveUnit, QPoint currentPoint, QPoint previousPoint, qint32 moveCost)
{
    spGameMap pMap = m_pMap.lock();
    if (pMap.get() != nullptr)
    {
        Unit* pUnit = pMap->getTerrain(currentPoint.x(), currentPoint.y())->getUnit();

        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValueList args({pInterpreter->newQObject(pAction.get()),
                           pInterpreter->newQObject(pMoveUnit),
                           pInterpreter->newQObject(pUnit),
                           currentPoint.x(),
                           currentPoint.y(),
                           previousPoint.x(),
                           previousPoint.y(),
                           moveCost,
                           pInterpreter->newQObject(pMap.get()),
                           });
        const QString obj = "ACTION_TRAP";
        QJSValue erg = pInterpreter->doFunction(obj, function, args);
        if (erg.isBool())
        {
            return erg.toBool();
        }
    }
    return false;
}

void ActionPerformer::delayedActionPerformed()
{
    if (m_pCurrentAction.get() == nullptr)
    {
        emit sigActionPerformed();
    }
}
