#include "game/actionperformer.h"
#include "game/gameanimation/gameanimationfactory.h"
#include "ai/coreai.h"
#include "ai/dummyai.h"
#include "ai/trainingdatagenerator.h"

#include "coreengine/mainapp.h"

#include "menue/gamemenue.h"

#include "objects/dialogs/dialogconnecting.h"

ActionPerformer::ActionPerformer(GameMap* pMap, GameMenue* pMenu)
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
    auto mapHash = m_pMap->getMapHash();
    if (m_exit)
    {
        if (m_pMenu != nullptr)
        {
            emit m_pMenu->sigVictory(-1);
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
    if (m_pMenu != nullptr)
    {
        m_pMenu->setSaveAllowed(false);
    }
    Mainapp::getInstance()->pauseRendering();
    if (m_multiplayerSyncData.m_waitingForSyncFinished && m_pMenu != nullptr)
    {
        m_multiplayerSyncData.m_postSyncAction = pGameAction;
        spDialogConnecting pDialogConnecting = MemoryManagement::create<DialogConnecting>(tr("Waiting for Players/Observers to join..."), 1000 * 60 * 5);
        m_pMenu->addChild(pDialogConnecting);
        connect(pDialogConnecting.get(), &DialogConnecting::sigCancel, m_pMenu, &GameMenue::exitGame, Qt::QueuedConnection);
        connect(m_pMenu, &GameMenue::sigSyncFinished, pDialogConnecting.get(), &DialogConnecting::connected, Qt::QueuedConnection);
    }
    else if (pGameAction.get() != nullptr)
    {
        Player* pCurrentPlayer = m_pMap->getCurrentPlayer();
        auto* baseGameInput = pCurrentPlayer->getBaseGameInput();
        bool proxyAi = baseGameInput != nullptr && (baseGameInput->getAiType() == GameEnums::AiTypes_ProxyAi || fromAiPipe || dynamic_cast<DummyAi*>(baseGameInput) != nullptr);
        CONSOLE_PRINT("GameMenue::performAction " + pGameAction->getActionID() + " at X: " + QString::number(pGameAction->getTarget().x())
                      + " at Y: " + QString::number(pGameAction->getTarget().y()) +
                      " is proxy ai " + QString::number(proxyAi), GameConsole::eDEBUG);
        bool multiplayer = false;
        if (m_pMenu != nullptr)
        {
            multiplayer = m_pMenu->getIsMultiplayer(pGameAction);
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
                m_pMenu != nullptr)
            {
                autosave = false;
                m_pMenu->doResyncGame();
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
            m_pMap->getGameRules()->pauseRoundTime();
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
            pGameAction->setRoundTimerTime(m_pMap->getGameRules()->getRoundTimer()->remainingTime());
            if (!pGameAction->getIsLocal() &&
                !fromAiPipe &&
                m_pMenu != nullptr &&
                m_pMenu->getGameStarted())
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
                stream << m_pMap->getCurrentPlayer()->getPlayerID();
                pGameAction->serializeObject(stream);
                emit m_pMenu->getNetworkInterface()->sig_sendData(0, data, NetworkInterface::NetworkSerives::Game, true);
            }
            else if (multiplayer)
            {
                m_pMap->getGameRules()->getRoundTimer()->setInterval(pGameAction->getRoundTimerTime());
            }

            // record action if required
            if (m_pMenu != nullptr)
            {
                m_pMenu->getReplayRecorder().recordAction(pGameAction);
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
        if (pCurrentPlayer != m_pMap->getCurrentPlayer() &&
            m_pMenu != nullptr &&
            autosave)
        {
            auto* baseGameInput = m_pMap->getCurrentPlayer()->getBaseGameInput();
            if (baseGameInput != nullptr &&
                baseGameInput->getAiType() == GameEnums::AiTypes_Human)
            {
                m_pMenu->autoSaveMap();
            }
        }
    }
    Mainapp::getInstance()->continueRendering();
}

bool ActionPerformer::requiresForwarding(const spGameAction & pGameAction) const
{
    Player* pCurrentPlayer = m_pMap->getCurrentPlayer();
    auto* baseGameInput = pCurrentPlayer->getBaseGameInput();
    return m_pMenu != nullptr &&
           m_pMenu->getIsMultiplayer(pGameAction) &&
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
    if (m_pMenu != nullptr &&
        m_pMenu->getGameStarted() &&
        !Settings::getInstance()->getAiSlave() &&
        pGameAction.get() != nullptr &&
        (pGameAction->getActionID() == CoreAI::ACTION_NEXT_PLAYER ||
         pGameAction->getActionID() == CoreAI::ACTION_SWAP_COS))
    {
        CONSOLE_PRINT("Check and update multiTurnMovement" + pGameAction->getActionID(), GameConsole::eDEBUG);

        // check for units that have a multi turn available
        qint32 heigth = m_pMap->getMapHeight();
        qint32 width = m_pMap->getMapWidth();
        Player* pPlayer = m_pMap->getCurrentPlayer();
        for (qint32 y = 0; y < heigth; y++)
        {
            for (qint32 x = 0; x < width; x++)
            {
                Unit* pUnit = m_pMap->getTerrain(x, y)->getUnit();
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
                                UnitPathFindingSystem pfs(m_pMap, pUnit, pPlayer);
                                pfs.setMovepoints(pUnit->getFuel());
                                pfs.explore();
                                auto newPath = pfs.getClosestReachableMovePath(currentMultiTurnPath, movepoints);
                                if (newPath.size() > 0)
                                {
                                    CONSOLE_PRINT("Replacing action with multiTurnMovement action", GameConsole::eDEBUG);
                                    // replace current action with auto moving none moved units
                                    m_pStoredAction = pGameAction;
                                    spGameAction multiTurnMovement = MemoryManagement::create<GameAction>(CoreAI::ACTION_WAIT, m_pMap);
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
                            spGameAction multiTurnMovement = MemoryManagement::create<GameAction>(CoreAI::ACTION_CAPTURE, m_pMap);
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
    if (m_pMenu != nullptr)
    {
        CONSOLE_PRINT("centerMapOnAction()", GameConsole::eDEBUG);
        Unit* pUnit = pGameAction->getTargetUnit();
        Player* pPlayer = m_pMenu->getCurrentViewPlayer();

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

        if (m_pMap->onMap(target.x(), target.y()) &&
            pPlayer->getFieldVisible(target.x(), target.y()) &&
            (pUnit == nullptr ||
             !pUnit->isStealthed(pPlayer)))
        {
            m_pMap->centerMap(target.x(), target.y());
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
    if (m_pCurrentAction.get() != nullptr)
    {
        Unit* pUnit = m_pCurrentAction->getMovementTarget();
        if (pUnit != nullptr)
        {
            pUnit->postAction(m_pCurrentAction.get());
        }
        m_pMap->getCurrentPlayer()->postAction(m_pCurrentAction.get());
        m_pMap->getGameScript()->actionDone(m_pCurrentAction);
    }
    skipAnimations(true);
    if (m_pMenu != nullptr)
    {
        m_pMenu->updateQuickButtons();
    }
}

void ActionPerformer::actionPerformed()
{
    if (m_pMenu != nullptr &&
        m_pMap != nullptr)
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
                m_pMap->syncUnitsAndBuildingAnimations();
            }
            if (m_pMenu != nullptr)
            {
                m_pMenu->updateGameInfo();
            }
            if (GameAnimationFactory::getAnimationCount() == 0)
            {
                CONSOLE_PRINT("Action finished", GameConsole::eDEBUG);
                m_pMap->killDeadUnits();
                m_pMap->getGameRules()->checkVictory();
                m_pMap->getGameRules()->createFogVision();
                m_pMap->getTrainingDataGenerator()->onActionDone(m_pCurrentAction.get());
                m_pCurrentAction.reset();
                m_actionRunning = false;
                m_finishedPerformed = false;
                CONSOLE_PRINT("Storing current map hash", GameConsole::eDEBUG);
                m_mapHash = m_pMap->getMapHash();
                GlobalUtils::setUseSeed(false);
                if (m_pMenu != nullptr &&
                    m_pMenu->getIndespawningMode())
                {
                    m_pMenu->setSaveAllowed(true);
                    m_pMenu->doDespawnSlave();
                }
                else if (m_exit &&
                         m_pMenu != nullptr)
                {
                    CONSOLE_PRINT("ActionPerformer state is exiting game. Emitting exit", GameConsole::eDEBUG);
                    emit m_pMenu->sigVictory(-1);
                }
                else if (!m_pMap->getGameRules()->getVictory())
                {
                    if (!m_pMap->anyPlayerAlive() &&
                        m_pMenu != nullptr)
                    {
                        CONSOLE_PRINT("Forcing exiting the game cause no player is alive", GameConsole::eDEBUG);
                        emit m_pMenu->sigExitGame();
                    }
                    else if (m_pMap->getCurrentPlayer()->getIsDefeated())
                    {
                        onTriggeringActionFinished();
                        CONSOLE_PRINT("Triggering next player cause current player is defeated", GameConsole::eDEBUG);
                        spGameAction pAction = MemoryManagement::create<GameAction>(CoreAI::ACTION_NEXT_PLAYER, m_pMap);
                        performAction(pAction);
                    }
                    else if (m_pStoredAction.get() != nullptr)
                    {
                        onTriggeringActionFinished();
                        performAction(m_pStoredAction);
                    }
                    else
                    {
                        if (m_pMap->getCurrentPlayer()->getBaseGameInput()->getAiType() != GameEnums::AiTypes_ProxyAi)
                        {
                            m_pMap->getGameRules()->resumeRoundTime();
                        }
                        if (m_noTimeOut &&
                            !Settings::getInstance()->getAiSlave())
                        {
                            onTriggeringActionFinished();
                            spGameAction pAction = MemoryManagement::create<GameAction>(CoreAI::ACTION_NEXT_PLAYER, m_pMap);
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
                else if (m_pMenu != nullptr)
                {
                    auto winner = m_pMap->getGameRules()->getVictoryTeam();
                    CONSOLE_PRINT("Game already won  retriggering victory for team " + QString::number(winner), GameConsole::eDEBUG);
                    emit m_pMenu->sigVictory(winner);
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
    if (m_pMenu != nullptr)
    {
        CONSOLE_PRINT("ActionPerformer::onTriggeringActionFinished", GameConsole::eDEBUG);
        m_pMenu->setSaveAllowed(true);
        if (m_pMenu->getSaveMap())
        {
            m_pMenu->doSaveMap();
        }
        m_pMenu->sendOnlineInfo();
    }
}

void ActionPerformer::nextTurnPlayerTimeout()
{
    auto* input = m_pMap->getCurrentPlayer()->getBaseGameInput();
    if (input != nullptr &&
        !Settings::getInstance()->getAiSlave())
    {
        if (input->getAiType() == GameEnums::AiTypes_Human)
        {
            if (m_pCurrentAction.get() == nullptr)
            {
                spGameAction pAction = MemoryManagement::create<GameAction>(CoreAI::ACTION_NEXT_PLAYER, m_pMap);
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

qint64 ActionPerformer::getSyncCounter() const
{
    return m_syncCounter;
}

void ActionPerformer::doTrapping(spGameAction & pGameAction)
{
    CONSOLE_PRINT("GameMenue::doTrapping", GameConsole::eDEBUG);
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
                Unit* pUnit = m_pMap->getTerrain(point.x(), point.y())->getUnit();
                if (pUnit == nullptr || pMoveUnit->getOwner()->isAlly(pUnit->getOwner()))
                {
                    if (i > 0)
                    {
                        spGameAction pTrapAction = MemoryManagement::create<GameAction>(CoreAI::ACTION_TRAP, m_pMap);
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
                        if (m_pMap->getTerrain(point.x(), point.y())->getUnit() != nullptr)
                        {
                            point = currentPoint;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                spGameAction pTrapAction = MemoryManagement::create<GameAction>(CoreAI::ACTION_TRAP, m_pMap);
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

bool ActionPerformer::isTrap(const QString & function, spGameAction pAction, Unit* pMoveUnit, QPoint currentPoint, QPoint previousPoint, qint32 moveCost)
{

    Unit* pUnit = m_pMap->getTerrain(currentPoint.x(), currentPoint.y())->getUnit();

    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({JsThis::getJsThis(pAction.get()),
                       JsThis::getJsThis(pMoveUnit),
                       JsThis::getJsThis(pUnit),
                       currentPoint.x(),
                       currentPoint.y(),
                       previousPoint.x(),
                       previousPoint.y(),
                       moveCost,
                       JsThis::getJsThis(m_pMap),
                      });
    const QString obj = "ACTION_TRAP";
    QJSValue erg = pInterpreter->doFunction(obj, function, args);
    if (erg.isBool())
    {
        return erg.toBool();
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
