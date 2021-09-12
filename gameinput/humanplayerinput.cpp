#include "gameinput/humanplayerinput.h"
#include "gameinput/markedfielddata.h"

#include "game/gamemap.h"
#include "game/gameaction.h"
#include "game/terrain.h"
#include "game/building.h"
#include "game/player.h"
#include "game/co.h"
#include "game/gameanimation/gameanimationfactory.h"

#include "resource_management/gamemanager.h"
#include "resource_management/fontmanager.h"
#include "resource_management/objectmanager.h"

#include "coreengine/mainapp.h"
#include "coreengine/audiothread.h"
#include "coreengine/interpreter.h"
#include "coreengine/globalutils.h"

#include "ai/coreai.h"

HumanPlayerInput::HumanPlayerInput()
    : BaseGameInputIF(GameEnums::AiTypes_Human)
{    
    setObjectName("HumanPlayerInput");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

void HumanPlayerInput::init()
{
    Mainapp* pApp = Mainapp::getInstance();
    spGameMenue pMenu = GameMenue::getInstance();
    spGameMap pMap = GameMap::getInstance();
    if (pMenu.get() != nullptr)
    {
        connect(pMenu.get(), &GameMenue::sigRightClickDown, this, &HumanPlayerInput::rightClickDown, Qt::QueuedConnection);
        connect(pMenu.get(), &GameMenue::sigRightClickUp, this, &HumanPlayerInput::rightClickUp, Qt::QueuedConnection);
        connect(pMenu.get(), &GameMenue::sigLeftClick, this, &HumanPlayerInput::leftClick, Qt::QueuedConnection);
        connect(pMenu.get(), &GameMenue::sigActionPerformed, this, &HumanPlayerInput::autoEndTurn, Qt::QueuedConnection);
        connect(pMap.get(), &GameMap::sigZoomChanged, this, &HumanPlayerInput::zoomChanged, Qt::QueuedConnection);
        connect(pApp, &Mainapp::sigKeyDown, this, &HumanPlayerInput::keyDown, Qt::QueuedConnection);
        connect(pMenu->getCursor(), &Cursor::sigCursorMoved, this, &HumanPlayerInput::cursorMoved, Qt::QueuedConnection);
        connect(this, &HumanPlayerInput::performAction, pMenu.get(), &GameMenue::performAction, Qt::QueuedConnection);
        m_Fields.reserve(pMap->getMapWidth() * pMap->getMapHeight() / 4);
        m_FieldPoints.reserve(pMap->getMapWidth() * pMap->getMapHeight() / 4);
    }
}

HumanPlayerInput::~HumanPlayerInput()
{
    m_pGameAction = nullptr;
    m_pUnitPathFindingSystem = nullptr;
}

void HumanPlayerInput::rightClickUp(qint32, qint32)
{
    if (GameMap::getInstance()->getCurrentPlayer() == m_pPlayer ||
        m_pPlayer == nullptr)
    {
        if (m_FieldPoints.size() > 0 && m_pGameAction.get() == nullptr)
        {
            cleanUpInput();
        }
        
    }
}

void HumanPlayerInput::rightClickDown(qint32 x, qint32 y)
{
    if (!GameMap::getInstance()->onMap(x, y))
    {
        return;
    }
    bool isViewPlayer = (GameMap::getInstance()->getCurrentViewPlayer() == m_pPlayer);

    if (GameMap::getInstance()->getCurrentPlayer() == m_pPlayer ||
        m_pPlayer == nullptr)
    {
        if (GameAnimationFactory::getAnimationCount() > 0)
        {
            GameAnimationFactory::finishAllAnimations();
            if (GameAnimationFactory::getAnimationCount() == 0)
            {
                emit GameAnimationFactory::getInstance()->animationsFinished();
            }
        }
        else if (m_pGameAction.get() != nullptr)
        {
            Mainapp::getInstance()->getAudioThread()->playSound("cancel.wav");
            if ((m_pGameAction->getInputStep() > 0) ||
                (m_pGameAction->getActionID() != ""))
            {
                // todo implement go back steps
                cancelActionInput();
            }
            else if (m_CurrentMenu.get() == nullptr)
            {
                cancelSelection(x, y);
            }
            else
            {
                cancelActionInput();
            }
            
        }
        else
        {
            if (m_FieldPoints.size() == 0 && m_pGameAction.get() == nullptr)
            {
                showAttackableFields(x, y);
            }
        }
    }
    else if (isViewPlayer)
    {
        if (GameAnimationFactory::getAnimationCount() > 0)
        {
            GameAnimationFactory::finishAllAnimations();
            if (GameAnimationFactory::getAnimationCount() == 0)
            {
                emit GameAnimationFactory::getInstance()->animationsFinished();
            }
        }
        else
        {
            cancelActionInput();
        }
    }
    else
    {
        // do nothing
    }
}

void HumanPlayerInput::cancelSelection(qint32 x, qint32 y)
{
    Console::print("HumanPlayerInput::cancelSelection", Console::eDEBUG);
    Unit* pUnit = m_pGameAction->getTargetUnit();
    if (pUnit != nullptr && !pUnit->getHasMoved() &&
        m_pUnitPathFindingSystem.get() != nullptr)
    {
        qint32 costs = m_pUnitPathFindingSystem->getTargetCosts(x, y);
        if (m_pUnitPathFindingSystem->getCosts(m_ArrowPoints) != costs &&
            costs > 0)
        {
            m_ArrowPoints = m_pUnitPathFindingSystem->getPath(x, y);
            createCursorPath(x, y);
        }
        else
        {
            cleanUpInput();
        }
    }
    else
    {
        cleanUpInput();
    }
}

void HumanPlayerInput::cancelActionInput()
{
    Console::print("HumanPlayerInput::cancelActionInput", Console::eDEBUG);
    Unit* pUnit = nullptr;
    if (m_pGameAction.get() != nullptr)
    {
        pUnit = m_pGameAction->getTargetUnit();
    }
    spGameMenue pMenu = GameMenue::getInstance();
    if (pMenu.get() != nullptr)
    {
        pMenu->getCursor()->changeCursor("cursor+default");
    }
    if ((pUnit != nullptr) &&
        (!pUnit->isStealthed(m_pPlayer)))
    {
        // go one step back :)
        clearMenu();
        createMarkedMoveFields();
        // reset action
        m_pGameAction->reset();
    }
    else
    {
        cleanUpInput();
    }
}

void HumanPlayerInput::showAttackableFields(qint32 x, qint32 y)
{
    Mainapp::getInstance()->pauseRendering();
    clearMarkedFields();
    // try to show fire ranges :)
    spGameMap pMap = GameMap::getInstance();
    Unit* pUnit = pMap->getTerrain(x, y)->getUnit();
    if ((pUnit != nullptr) &&
        (!pUnit->isStealthed(m_pPlayer)))
    {
        if (pUnit->hasAmmo1() || pUnit->hasAmmo2())
        {
            qint32 maxRange = pUnit->getMaxRange(pUnit->getPosition());
            qint32 minRange = pUnit->getMinRange(pUnit->getPosition());
            spQmlVectorPoint pPoints = spQmlVectorPoint(GlobalUtils::getCircle(minRange, maxRange));
            Mainapp::getInstance()->getAudioThread()->playSound("selectunit.wav");
            UnitPathFindingSystem pfs(pMap->getTerrain(x, y)->getUnit(), m_pPlayer);
            pfs.explore();
            QVector<QPoint> points = pfs.getAllNodePoints();
            for (qint32 i = 0; i < points.size(); i++)
            {
                if (pUnit->canMoveAndFire(QPoint(x, y)) ||
                    (points[i].x() == x && points[i].y() == y))
                {

                    for (qint32 i2 = 0; i2 < pPoints->size(); i2++)
                    {
                        QPoint target = pPoints->at(i2) + points[i];
                        if (!m_FieldPoints.contains(QVector3D(target.x(), target.y(), 1)))
                        {
                            createMarkedField(target, QColor(255, 0, 0), Terrain::DrawPriority::MarkedFieldMap);
                        }
                    }
                }
            }
        }
    }
    else
    {
        Building* pBuilding = pMap->getTerrain(x, y)->getBuilding();
        if (pBuilding != nullptr)
        {
            spQmlVectorPoint pPoints = spQmlVectorPoint(pBuilding->getActionTargetFields());
            QPoint targetOffset = pBuilding->getActionTargetOffset();
            QPoint buildingPos(pBuilding->Building::getX(), pBuilding->Building::getY());
            if (pPoints.get() != nullptr && pPoints->size() > 0)
            {
                Mainapp::getInstance()->getAudioThread()->playSound("selectunit.wav");
                for (qint32 i = 0; i < pPoints->size(); i++)
                {
                    createMarkedField(buildingPos + targetOffset + pPoints->at(i), QColor(255, 0, 0), Terrain::DrawPriority::MarkedFieldMap);
                }
            }
        }
    }
    syncMarkedFields();
    Mainapp::getInstance()->continueRendering();
}

void HumanPlayerInput::syncMarkedFields()
{
    for (auto & field : m_Fields)
    {
        auto & tweens = field->getTweens();
        for (auto & pTween : tweens)
        {
            pTween->reset();
            pTween->start(*field);
        }
    }
}

void HumanPlayerInput::cleanUpInput()
{
    Console::print("HumanPlayerInput::cleanUpInput", Console::eDEBUG);
    clearMenu();
    m_pGameAction = nullptr;
    m_pUnitPathFindingSystem = nullptr;
    clearMarkedFields();
    deleteArrow();
    spGameMenue pMenue = GameMenue::getInstance();
    if (pMenue.get() != nullptr)
    {
        Cursor* pCursor = pMenue->getCursor();
        pCursor->changeCursor("cursor+default");
        pCursor->resetCursorRangeOutline();
        cursorMoved(pCursor->getMapPointX(), pCursor->getMapPointY());
    }
}

void HumanPlayerInput::clearMenu()
{
    if (m_CurrentMenu.get() != nullptr)
    {
        spGameMenue pMenue = GameMenue::getInstance();
        if (pMenue.get() != nullptr)
        {
            pMenue->setFocused(true);
        }
        m_CurrentMenu->detach();
        m_CurrentMenu = nullptr;
    }
}

void HumanPlayerInput::clearMarkedFields()
{
    Mainapp::getInstance()->pauseRendering();
    for (qint32 i = 0; i < m_Fields.size(); i++)
    {
        m_Fields[i]->detach();
    }
    m_FieldPoints.clear();
    m_Fields.clear();
    m_pMarkedFieldData = nullptr;
    if (m_ZInformationLabel.get() != nullptr)
    {
        m_ZInformationLabel->detach();
        m_ZInformationLabel = nullptr;
    }
    for (auto & fields : m_InfoFields)
    {
        fields->detach();
    }
    m_InfoFields.clear();
    Mainapp::getInstance()->continueRendering();
}

void HumanPlayerInput::leftClick(qint32 x, qint32 y)
{
    Console::print("humanplayer input leftClick() with X " + QString::number(x) + " Y " + QString::number(y), Console::eDEBUG);
    spGameMenue pMenu = GameMenue::getInstance();
    Cursor* pCursor = pMenu->getCursor();
    if (pMenu.get() != nullptr &&
        GameAnimationFactory::getAnimationCount() == 0)
    {
        spGameMap pMap = GameMap::getInstance();
        bool isViewPlayer = (GameMap::getInstance()->getCurrentViewPlayer() == m_pPlayer);
        if (!pMap->onMap(x, y))
        {
            // do nothing
        }
        else if (!pMenu->getFocused())
        {
            if (m_CurrentMenu.get() != nullptr && Settings::getSimpleDeselect())
            {
                Mainapp::getInstance()->getAudioThread()->playSound("cancel.wav");
                cancelActionInput();
            }
        }
        else if (pMap->getCurrentPlayer() == m_pPlayer ||
                 m_pPlayer == nullptr)
        {
            if (m_pMarkedFieldData.get() != nullptr &&
                (!m_pMarkedFieldData->getShowZData() ||
                 (!Settings::getTouchScreen() ||
                  (m_lastClickPoint.x() == x &&
                   m_lastClickPoint.y() == y))))
            {
                // did we select a marked field?
                if (m_pMarkedFieldData->getAllFields())
                {
                    markedFieldSelected(QPoint(x, y));
                }
                else
                {
                    QVector<QPoint>* pFields = m_pMarkedFieldData->getPoints();
                    for (qint32 i = 0; i < pFields->size(); i++)
                    {
                        if ((pFields->at(i).x() == x) &&
                            (pFields->at(i).y() == y))
                        {
                            markedFieldSelected(QPoint(x, y));
                            break;
                        }
                    }
                }
            }
            // no action selected
            else if (m_Fields.size() > 0 && m_pGameAction.get() == nullptr)
            {
                // do nothing
                // some one spawned them to give some info hints player needs to remove them by canceling.
            }
            else if (m_pGameAction.get() == nullptr)
            {
                // prepare action
                m_pGameAction = spGameAction::create();
                if (m_pPlayer != nullptr)
                {
                    m_pGameAction->setPlayer(m_pPlayer->getPlayerID());
                }
                m_pGameAction->setTarget(QPoint(x, y));
                spGameMap pMap = GameMap::getInstance();
                Unit* pUnit = pMap->getTerrain(x, y)->getUnit();
                if ((pUnit != nullptr) &&
                    (!pUnit->isStealthed(m_pPlayer)))
                {
                    selectUnit(x, y);
                }
                else if (m_pPlayer != nullptr)
                {
                    Building* pBuilding = pMap->getTerrain(x, y)->getBuilding();
                    QStringList actions;
                    QStringList possibleActions;
                    if ((pBuilding != nullptr) &&
                        (pBuilding->getOwner() == m_pPlayer))
                    {
                        actions = pBuilding->getActionList();
                        for (qint32 i = 0; i < actions.size(); i++)
                        {
                            if (m_pGameAction->canBePerformed(actions[i]))
                            {
                                possibleActions.append(actions[i]);
                            }
                        }
                    }
                    if (possibleActions.size() > 0)
                    {
                        Mainapp::getInstance()->getAudioThread()->playSound("selectunit.wav");
                        if ((possibleActions.size() == 1) &&
                            (!m_pGameAction->isFinalStep(possibleActions[0])))
                        {
                            // skip show select action menu
                            m_pGameAction->setActionID(possibleActions[0]);
                            getNextStepData();
                        }
                        else
                        {
                            if (possibleActions.size() > 0)
                            {
                                createActionMenu(possibleActions, x, y);
                            }
                        }
                    }
                    else
                    {
                        std::chrono::milliseconds time = std::chrono::milliseconds(m_doubleClickTime.elapsed());
                        if ((time < std::chrono::milliseconds(1000) && m_doubleClickTime.isValid())
                            || !Settings::getTouchScreen())
                        {
                            actions = getEmptyActionList();
                            possibleActions.clear();
                            for (qint32 i = 0; i < actions.size(); i++)
                            {
                                if (m_pGameAction->canBePerformed(actions[i], true))
                                {
                                    possibleActions.append(actions[i]);
                                }
                            }
                            if (possibleActions.size() > 0)
                            {
                                Mainapp::getInstance()->getAudioThread()->playSound("selectunit.wav");
                                createActionMenu(possibleActions, x, y);
                            }
                            else
                            {
                                cleanUpInput();
                            }
                            m_doubleClickTime.invalidate();
                        }
                        else
                        {
                            m_doubleClickTime.start();
                            m_pGameAction = nullptr;
                        }
                    }
                }
                else
                {
                    m_pGameAction = nullptr;
                }
            }
            // we want to select an action
            else if (m_pPlayer != nullptr &&
                     m_pGameAction.get() != nullptr &&
                     m_pGameAction->getActionID() == "")
            {
                if ((m_pUnitPathFindingSystem.get() != nullptr) &&
                    (m_pUnitPathFindingSystem->isReachable(x, y)) &&
                    (m_CurrentMenu.get() == nullptr))
                {
                    Unit* pUnit = m_pGameAction->getTargetUnit();
                    if (pUnit != nullptr)
                    {
                        m_pGameAction->setMovepath(m_ArrowPoints, m_pGameAction->getCosts());
                        QStringList actions;
                        QStringList possibleActions;
                        if (m_pUnitPathFindingSystem->getCosts(m_ArrowPoints) > pUnit->getMovementpoints(m_pGameAction->getTarget()))
                        {
                            if (pUnit->getActionList().contains(CoreAI::ACTION_HOELLIUM_WAIT))
                            {
                                actions.append(CoreAI::ACTION_HOELLIUM_WAIT);
                            }
                            else if (pUnit->getActionList().contains(CoreAI::ACTION_WAIT))
                            {
                                actions.append(CoreAI::ACTION_WAIT);
                            }
                        }
                        else
                        {
                            // we want to do something with this unit :)
                            actions = pUnit->getActionList();
                        }
                        for (qint32 i = 0; i < actions.size(); i++)
                        {
                            if (m_pGameAction->canBePerformed(actions[i]))
                            {
                                possibleActions.append(actions[i]);
                            }
                        }
                        if (possibleActions.size() > 0)
                        {
                            createActionMenu(possibleActions, x, y);
                        }
                    }
                }
                else
                {
                    Mainapp::getInstance()->getAudioThread()->playSound("cancel.wav");
                    cleanUpInput();
                }
            }
            else
            {
                //cleanUpInput();
            }
            
        }
        else if (isViewPlayer)
        {
            // prepare action
            m_pGameAction = spGameAction::create();
            m_pGameAction->setTarget(QPoint(x, y));
            QStringList actions = getViewplayerActionList();
            QStringList possibleActions;
            for (qint32 i = 0; i < actions.size(); i++)
            {
                if (m_pGameAction->canBePerformed(actions[i], true))
                {
                    possibleActions.append(actions[i]);
                }
            }
            if (possibleActions.size() > 0)
            {
                Mainapp::getInstance()->getAudioThread()->playSound("selectunit.wav");
                createActionMenu(possibleActions, x, y);
            }
        }
        else
        {
            // do nothing
        }
        m_lastClickPoint = QPoint(pCursor->getMapPointX(), pCursor->getMapPointY());
    }
}

void HumanPlayerInput::markedFieldSelected(QPoint point)
{    
    m_pGameAction->writeDataInt32(point.x());
    m_pGameAction->writeDataInt32(point.y());
    clearMarkedFields();
    m_pGameAction->setInputStep(m_pGameAction->getInputStep() + 1);
    if (m_pGameAction->isFinalStep())
    {
        // if not perform action
        finishAction();
    }
    else
    {
        // else introduce next step
        getNextStepData();
    }
    
}

void HumanPlayerInput::menuItemSelected(QString itemID, qint32 cost)
{
    Console::print("HumanPlayerInput::menuItemSelected", Console::eDEBUG);
    if (m_pGameAction.get() != nullptr)
    {
        // we're currently selecting the action for this action
        if (m_pGameAction->getActionID() == "")
        {
            // set the id
            m_pGameAction->setActionID(itemID);
            m_pGameAction->setCosts(m_pGameAction->getCosts() + cost);
        }
        // we want to append some data to the action
        else
        {
            m_pGameAction->writeDataString(itemID);
            // increase costs and input step
            m_pGameAction->setCosts(m_pGameAction->getCosts() + cost);
            m_pGameAction->setInputStep(m_pGameAction->getInputStep() + 1);
        }
        // check if the action needs further information
        if (m_pGameAction->isFinalStep())
        {
            // if not perform action
            finishAction();
        }
        else
        {
            // else introduce next step
            getNextStepData();
        }
    }
}

void HumanPlayerInput::getNextStepData()
{
    Mainapp::getInstance()->pauseRendering();
    Console::print("HumanPlayerInput::getNextStepData", Console::eDEBUG);
    clearMenu();
    clearMarkedFields();
    spGameMenue pMenu = GameMenue::getInstance();
    if (pMenu.get() != nullptr)
    {
        Cursor* pCursor = pMenu->getCursor();
        pCursor->changeCursor("cursor+default");
        pCursor->resetCursorRangeOutline();
        QString stepType = m_pGameAction->getStepInputType();
        if (stepType.toUpper() == "MENU")
        {
            Console::print("HumanPlayerInput::getNextStepData show menu", Console::eDEBUG);
            spMenuData pData = m_pGameAction->getMenuStepData();
            if (pData->validData())
            {
                m_CurrentMenu = spHumanPlayerInputMenu::create(pData->getTexts(), pData->getActionIDs(), pData->getIconList(), pData->getCostList(), pData->getEnabledList());
                attachActionMenu(m_pGameAction->getActionTarget().x(), m_pGameAction->getActionTarget().y());
            }
        }
        else if (stepType.toUpper() == "FIELD")
        {
            Console::print("HumanPlayerInput::getNextStepData show fields", Console::eDEBUG);
            spMarkedFieldData pData = m_pGameAction->getMarkedFieldStepData();
            QVector<QPoint>* pFields = pData->getPoints();
            for (qint32 i = 0; i < pFields->size(); i++)
            {
                createMarkedField(pFields->at(i), pData->getColor(), Terrain::DrawPriority::MarkedFieldMap);
            }
            syncMarkedFields();
            m_pMarkedFieldData = pData;
            spCursorData pCursordata = m_pGameAction->getStepCursor();
            pMenu->getCursor()->changeCursor(pCursordata->getCursor(), pCursordata->getXOffset(), pCursordata->getYOffset(), pCursordata->getScale());
            if (!m_pMarkedFieldData->getAllFields())
            {
                nextMarkedField();
            }
        }
        else
        {
            Console::print("Unknown step type detected. This will lead to an undefined behaviour. Action " + m_pGameAction->getActionID() + " at step " + QString::number(m_pGameAction->getInputStep()), Console::eERROR);
        }
    }
    Mainapp::getInstance()->continueRendering();
}

void HumanPlayerInput::finishAction()
{
    Console::print("HumanPlayerInput::finishAction", Console::eDEBUG);
    if (m_pGameAction.get() != nullptr)
    {
        Unit* pUnit = m_pGameAction->getTargetUnit();
        if (pUnit != nullptr && m_pUnitPathFindingSystem.get() != nullptr)
        {
            qint32 movepoints = pUnit->getMovementpoints(m_pGameAction->getTarget());
            if (m_pUnitPathFindingSystem->getCosts(m_ArrowPoints) > movepoints)
            {
                // shorten path
                QVector<QPoint> newPath = m_pUnitPathFindingSystem->getClosestReachableMovePath(m_ArrowPoints, movepoints);
                m_pGameAction->setMovepath(newPath, m_pUnitPathFindingSystem->getCosts(newPath));
                QVector<QPoint> multiTurnPath;
                for (qint32 i = 0; i <= m_ArrowPoints.size() - newPath.size(); i++)
                {
                    multiTurnPath.append(m_ArrowPoints[i]);
                }
                m_pGameAction->setMultiTurnPath(multiTurnPath);
            }
            else
            {
                // reset path in all other cases
                pUnit->setMultiTurnPath(QVector<QPoint>());
            }
        }

        spGameMap pMap = GameMap::getInstance();
        bool isViewPlayer = (pMap->getCurrentViewPlayer() == m_pPlayer);
        if (pMap->getCurrentPlayer() == m_pPlayer)
        {
            emit performAction(m_pGameAction);
        }
        else if (isViewPlayer)
        {
            if (m_pGameAction->getIsLocal())
            {
                m_pGameAction->perform();
            }
            m_pGameAction = nullptr;
        }
        else
        {
            // do nothing
        }
        m_pGameAction = nullptr;
    }
    cleanUpInput();
}

void HumanPlayerInput::createActionMenu(QStringList actionIDs, qint32 x, qint32 y)
{
    Console::print("HumanPlayerInput::createActionMenu", Console::eDEBUG);
    clearMarkedFields();
    MenuData data;

    for (qint32 i = 0; i < actionIDs.size(); i++)
    {
        data.addData(GameAction::getActionText(actionIDs[i]), actionIDs[i], GameAction::getActionIcon(actionIDs[i]));
    }
    m_CurrentMenu = spHumanPlayerInputMenu::create(data.getTexts(), actionIDs, data.getIconList());
    attachActionMenu(x, y);
}

void HumanPlayerInput::attachActionMenu(qint32 x, qint32 y)
{
    spGameMenue pMenu = GameMenue::getInstance();
    if (pMenu.get() != nullptr)
    {
        spGameMap pMap = GameMap::getInstance();
        oxygine::spSlidingActorNoClipRect pMapSliding = pMenu->getMapSliding();
        oxygine::spActor pMapSlidingActor = pMenu->getMapSlidingActor();
        float posX = x * GameMap::getImageSize() * pMap->getZoom() + pMap->getX() + pMapSliding->getX() + pMapSlidingActor->getX();
        if (posX + m_CurrentMenu->getWidth() > Settings::getWidth() - 40 - pMenu->getGameInfoBar()->getWidth())
        {
            posX = Settings::getWidth() - m_CurrentMenu->getWidth() - 40 - pMenu->getGameInfoBar()->getWidth();
        }
        if (posX < 10)
        {
            posX = 10;
        }
        float posY = y * GameMap::getImageSize() * pMap->getZoom() + pMap->getY() + pMapSliding->getY() + pMapSlidingActor->getY();
        if (posY < 10)
        {
            posY = 10;
        }
        else if (posY + m_CurrentMenu->getHeight() > Settings::getHeight())
        {
            posY = Settings::getHeight() - m_CurrentMenu->getHeight() - 10;
        }
        m_CurrentMenu->setPosition(posX, posY);
        pMenu->addChild(m_CurrentMenu);
        m_CurrentMenu->moveMouseToItem(0, 0);
        pMenu->setFocused(false);
        connect(m_CurrentMenu.get(), &HumanPlayerInputMenu::sigItemSelected, this, &HumanPlayerInput::menuItemSelected, Qt::QueuedConnection);
        connect(m_CurrentMenu.get(), &HumanPlayerInputMenu::sigCanceled, this, &HumanPlayerInput::rightClickDown, Qt::QueuedConnection);
    }
}

void HumanPlayerInput::selectUnit(qint32 x, qint32 y)
{
    Console::print("Selecting unit", Console::eDEBUG);
    Mainapp::getInstance()->getAudioThread()->playSound("selectunit.wav");
    spGameMap pMap = GameMap::getInstance();
    Unit* pUnit = pMap->getTerrain(x, y)->getUnit();
    m_pUnitPathFindingSystem = spUnitPathFindingSystem::create(pUnit, m_pPlayer);
    if ((pUnit->getOwner() == m_pPlayer) &&
        pUnit->getActionList().contains(CoreAI::ACTION_WAIT))
    {
        qint32 points = pUnit->getMovementpoints(QPoint(x, y)) * static_cast<qint32>(Settings::getMultiTurnCounter());
        if (pUnit->getFuel() < points)
        {
            points = pUnit->getFuel();
        }
        m_pUnitPathFindingSystem->setMovepoints(points);
    }
    else
    {
        m_pUnitPathFindingSystem->setMovepoints(pUnit->getMovementpoints(QPoint(x, y)));
    }
    qint32 maxRange = pUnit->getMaxRange(pUnit->getPosition());
    if (maxRange > 1)
    {
        spGameMenue pMenue = GameMenue::getInstance();
        qint32 minRange = pUnit->getMinRange(pUnit->getPosition());
        Cursor* pCursor = pMenue->getCursor();
        if (minRange > 1)
        {
            pCursor->addCursorRangeOutline(minRange - 1, Qt::green);
        }
        pCursor->addCursorRangeOutline(maxRange, Qt::red);
    }
    qint32 infoRange = pUnit->getCursorInfoRange();
    if (infoRange >= 1)
    {
        spGameMenue pMenue = GameMenue::getInstance();
        Cursor* pCursor = pMenue->getCursor();
        pCursor->addCursorRangeOutline(infoRange, Qt::white);
    }
    m_pUnitPathFindingSystem->explore();
    createMarkedMoveFields();
}

void HumanPlayerInput::createMarkedField(QPoint point, QColor color, Terrain::DrawPriority drawPriority)
{
    spGameMap pMap = GameMap::getInstance();
    if (pMap->onMap(point.x(), point.y()))
    {
        oxygine::spSprite pSprite = createMarkedFieldActor(point, color, drawPriority);
        if (drawPriority == Terrain::DrawPriority::MarkedFieldMap)
        {
            m_FieldPoints.append(QVector3D(point.x(), point.y(), 1));
        }
        else
        {
            m_FieldPoints.append(QVector3D(point.x(), point.y(), 0));
        }
        m_Fields.append(pSprite);
    }
}

oxygine::spSprite HumanPlayerInput::createMarkedFieldActor(QPoint point, QColor color, Terrain::DrawPriority drawPriority)
{
    spGameMap pMap = GameMap::getInstance();
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::spSprite pSprite = oxygine::spSprite::create();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("marked+field");
    if (pAnim->getTotalFrames() > 1 && !Settings::getStaticMarkedFields())
    {
        float initFrame = 0;
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim, initFrame, 0), oxygine::timeMS(static_cast<qint32>(pAnim->getTotalFrames() * GameMap::frameTime)), -1);
        pSprite->addTween(tween);
    }
    else
    {
        pSprite->setResAnim(pAnim);
    }
    pSprite->setColor(color);
    // pSprite->setDestRecModifier(oxygine::RectF(0.5f, 0.5f, 0.0f, 0.0f));

    if (drawPriority == Terrain::DrawPriority::MarkedFieldMap)
    {
        pSprite->setScale((GameMap::getImageSize()) / pAnim->getWidth());
        pSprite->setPosition(point.x() * GameMap::getImageSize(), point.y() * GameMap::getImageSize());
        pSprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::MarkedFields));
        pMap->addChild(pSprite);
    }
    else
    {
        pSprite->setScale((GameMap::getImageSize()) / pAnim->getWidth());
        pSprite->setPriority(static_cast<qint16>(drawPriority));
        pSprite->setPosition(-(pSprite->getScaledWidth() - GameMap::getImageSize()) / 2, -(pSprite->getScaledHeight() - GameMap::getImageSize()));
        pMap->getSpTerrain(point.x(), point.y())->addChild(pSprite);
    }
    return pSprite;
}

void HumanPlayerInput::createMarkedMoveFields()
{
    Console::print("createMarkedMoveFields()", Console::eDEBUG);
    Mainapp::getInstance()->pauseRendering();
    clearMarkedFields();
    if (m_pUnitPathFindingSystem.get() != nullptr)
    {
        qint32 movementpoints = m_pGameAction->getTargetUnit()->getMovementpoints(m_pGameAction->getTarget());
        QVector<QPoint> points = m_pUnitPathFindingSystem->getAllNodePoints();
        for (qint32 i = 0; i < points.size(); i++)
        {
            if (m_pUnitPathFindingSystem->getTargetCosts(points[i].x(), points[i].y()) > movementpoints)
            {
                createMarkedField(points[i], QColor(128, 0, 255, 255), Terrain::DrawPriority::MarkedField);
            }
            else
            {
                createMarkedField(points[i], QColor(0, 128, 255, 255), Terrain::DrawPriority::MarkedField);
            }
        }
        syncMarkedFields();
    }
    Mainapp::getInstance()->continueRendering();
}

void HumanPlayerInput::cursorMoved(qint32 x, qint32 y)
{
    spGameMenue pMenu = GameMenue::getInstance();
    spGameMap pMap = GameMap::getInstance();
    auto mapPos = pMenu->getMapSlidingActor()->getPosition();
    m_lastMapView = QPoint(mapPos.x, mapPos.y);
    if (x != m_lastCursorPosition.x() ||
        y != m_lastCursorPosition.y())
    {
        if ((pMap->getCurrentPlayer() == m_pPlayer ||
             m_pPlayer == nullptr) &&
            pMap->onMap(x, y))
        {
            Console::print("HumanPlayerInput::cursorMoved" , Console::eDEBUG);
            if (m_pMarkedFieldData.get() != nullptr)
            {
                if (m_pMarkedFieldData->getShowZData())
                {
                    // marked field?
                    if (m_pMarkedFieldData->getPoints()->contains(QPoint(x, y)))
                    {
                        if (m_ZInformationLabel.get() != nullptr)
                        {
                            m_ZInformationLabel->detach();
                            m_ZInformationLabel = nullptr;
                        }
                        QPoint field(x, y);
                        const MarkedFieldData::ZInformation* pData = nullptr;
                        for (qint32 i = 0; i < m_pMarkedFieldData->getPoints()->size(); i++)
                        {
                            if (m_pMarkedFieldData->getPoints()->at(i) == field)
                            {
                                const auto* info = m_pMarkedFieldData->getZInformation();
                                pData = &info->at(i);
                                break;
                            }
                        }
                        if (pData != nullptr)
                        {
                            if (pData->valueNames.size() == 0)
                            {
                                createSimpleZInformation(x, y, pData);
                            }
                            else
                            {
                                createComplexZInformation(x, y, pData);
                            }
                        }
                    }
                    else
                    {
                        if (m_ZInformationLabel.get() != nullptr)
                        {
                            m_ZInformationLabel->detach();
                            m_ZInformationLabel = nullptr;
                        }
                    }
                }
            }
            else if (m_pUnitPathFindingSystem.get() != nullptr)
            {
                if ((m_CurrentMenu.get() == nullptr) && m_pGameAction->getActionID() == "")
                {
                    createCursorPath(x, y);
                }
            }
            else
            {
                Unit* pUnit = pMap->getTerrain(x, y)->getUnit();
                deleteArrow();
                if (pUnit != nullptr)
                {
                    QVector<QPoint> multiTurnPath = pUnit->getMultiTurnPath();
                    if (pUnit->getOwner() == m_pPlayer &&
                        multiTurnPath.size() > 0)
                    {
                        createArrow(multiTurnPath);
                    }
                }
            }

        }
    }
    m_lastCursorPosition = QPoint(x, y);
}

void HumanPlayerInput::createSimpleZInformation(qint32 x, qint32 y, const MarkedFieldData::ZInformation* pData)
{
    Console::print("HumanPlayerInput::createSimpleZInformation " + QString::number(pData->singleValue) , Console::eDEBUG);
    spGameMap pMap = GameMap::getInstance();
    QString labelText = "";
    labelText = QString::number(pData->singleValue) + " %";
    m_ZInformationLabel = oxygine::spActor::create();
    GameManager* pGameManager = GameManager::getInstance();
    oxygine::spSprite pSprite = oxygine::spSprite::create();
    oxygine::ResAnim* pAnim = pGameManager->getResAnim("z_information_label");
    if (pAnim->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), oxygine::timeMS(pAnim->getTotalFrames() * GameMap::frameTime), -1);
        pSprite->addTween(tween);
    }
    else
    {
        pSprite->setResAnim(pAnim);
    }
    oxygine::spSprite pSprite2 = oxygine::spSprite::create();
    oxygine::ResAnim* pAnim2 = pGameManager->getResAnim("z_information_label+mask");

    if (pAnim2->getTotalFrames() > 1)
    {
        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim2), oxygine::timeMS(pAnim2->getTotalFrames() * GameMap::frameTime), -1);
        pSprite2->addTween(tween);
    }
    else
    {
        pSprite2->setResAnim(pAnim2);
    }
    QColor color = m_pMarkedFieldData->getZLabelColor();
    pSprite2->setColor(color.red(), color.green(), color.blue(), color.alpha());
    constexpr float baseScale = 4.0f;
    pSprite->setScale(baseScale);
    pSprite2->setScale(baseScale);
    m_ZInformationLabel->setSize(pAnim->getSize() * baseScale);
    m_ZInformationLabel->addChild(pSprite2);
    m_ZInformationLabel->addChild(pSprite);
    // add text to the label
    oxygine::spClipRectActor clipRec = oxygine::spClipRectActor::create();
    clipRec->setX(4);
    clipRec->setY(0);
    clipRec->setSize(28 * 4, 40);
    oxygine::spTextField textField = oxygine::spTextField::create();
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont72());
    style.color = FontManager::getFontColor();
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    textField->setStyle(style);
    textField->setScale(32.0f / 72.0f);
    textField->setHtmlText(m_pMarkedFieldData->getZLabelText());
    textField->setSize(clipRec->getSize());
    clipRec->addChild(textField);
    m_ZInformationLabel->addChild(clipRec);

    oxygine::spTextField textField2 = oxygine::spTextField::create();
    textField2->setStyle(style);
    textField2->setY(44);
    textField2->setX(10);
    textField2->setSize(clipRec->getSize());
    textField2->setScale(32.0f / 72.0f);
    textField2->setHtmlText(labelText);
    m_ZInformationLabel->addChild(textField2);

    m_ZInformationLabel->setPriority(static_cast<qint32>(Mainapp::ZOrder::FocusedObjects));
    pMap->addChild(m_ZInformationLabel);
    zoomChanged(pMap->getZoom());
}

void HumanPlayerInput::createComplexZInformation(qint32 x, qint32 y, const MarkedFieldData::ZInformation* pData)
{
    spGameMap pMap = GameMap::getInstance();
    QString attackInfo = "Info: ";
    for (qint32 i = 0; i < pData->valueNames.size(); ++i)
    {
        attackInfo += " " + pData->valueNames[i] + " ";
        if (i <pData->ownUnitValues.size())
        {
            attackInfo += " own: " + QString::number(pData->ownUnitValues[i]);
        }
        if (i <pData->enemyUnitValues.size())
        {
            attackInfo += " enemy: " + QString::number(pData->enemyUnitValues[i]);
        }
    }
    Console::print("HumanPlayerInput::createComplexZInformation " + attackInfo, Console::eDEBUG);
    ObjectManager* pObjectManager = ObjectManager::getInstance();
    oxygine::spBox9Sprite pBox = oxygine::spBox9Sprite::create();
    oxygine::ResAnim* pAnim = pObjectManager->getResAnim("panel");
    pBox->setVerticalMode(oxygine::Box9Sprite::STRETCHING);
    pBox->setHorizontalMode(oxygine::Box9Sprite::STRETCHING);
    pBox->setResAnim(pAnim);
    constexpr qint32 baseWidth = 90;
    constexpr qint32 textWidth = 60;
    qint32 itemWidth = textWidth;
    if (pData->ownUnitValues.size() > 0)
    {
        itemWidth += baseWidth;
    }
    if (pData->enemyUnitValues.size() > 0)
    {
        itemWidth += baseWidth;
    }
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.color = FontManager::getFontColor();
    style.hAlign = oxygine::TextStyle::HALIGN_MIDDLE;
    style.vAlign = oxygine::TextStyle::VALIGN_TOP;
    pBox->setSize(itemWidth, 10 + 30 * pData->valueNames.size());
    for (qint32 i = 0; i < pData->valueNames.size(); ++i)
    {
        spLabel pLabel = spLabel::create(textWidth);
        pLabel->setStyle(style);
        pLabel->setHtmlText(pData->valueNames[i]);
        pLabel->setPosition(baseWidth, 30 * i + 5);
        pBox->addChild(pLabel);
    }
    if (pData->ownUnitValues.size() > 0)
    {
        oxygine::spColorRectSprite pRect = oxygine::spColorRectSprite::create();
        pRect->setPosition(7, 4);
        pRect->setSize(baseWidth, pBox->getHeight() - 10);
        pRect->setColor(m_pPlayer->getColor());
        pRect->setAlpha(200);
        pBox->addChild(pRect);
        for (qint32 i = 0; i < pData->valueNames.size(); ++i)
        {
            spLabel pLabel = spLabel::create(baseWidth);
            pLabel->setStyle(style);
            if (pData->ownUnitValues[i] < 0)
            {
                pLabel->setHtmlText("-");
            }
            else
            {
                pLabel->setHtmlText(QString::number(pData->ownUnitValues[i]) + " %");
            }
            pLabel->setPosition(7, 30 * i + 5);
            pBox->addChild(pLabel);
        }
    }
    if (pData->enemyUnitValues.size() > 0)
    {
        oxygine::spColorRectSprite pRect = oxygine::spColorRectSprite::create();
        pRect->setSize(baseWidth, pBox->getHeight() - 10);
        pRect->setPosition(pBox->getWidth() - 4 - pRect->getWidth(), 4);
        pRect->setColor(pData->enemyColor);
        pRect->setAlpha(200);
        pBox->addChild(pRect);
        for (qint32 i = 0; i < pData->enemyUnitValues.size(); ++i)
        {
            spLabel pLabel = spLabel::create(baseWidth);
            pLabel->setStyle(style);
            if (pData->enemyUnitValues[i] < 0)
            {
                pLabel->setHtmlText("-");
            }
            else
            {
                pLabel->setHtmlText(QString::number(pData->enemyUnitValues[i]) + " %");
            }
            pLabel->setPosition(pRect->getX(), 30 * i + 5);
            pBox->addChild(pLabel);
        }
    }
    m_ZInformationLabel = pBox;
    m_ZInformationLabel->setPriority(static_cast<qint32>(Mainapp::ZOrder::FocusedObjects));
    pMap->addChild(m_ZInformationLabel);
    zoomChanged(pMap->getZoom());
}

void HumanPlayerInput::zoomChanged(float zoom)
{
    spGameMenue pMenu = GameMenue::getInstance();
    if (m_ZInformationLabel.get() != nullptr &&
        pMenu.get() != nullptr)
    {
        auto* pCursor = pMenu->getCursor();
        m_ZInformationLabel->setScale(1.0f / zoom);
        m_ZInformationLabel->setPosition(pCursor->getMapPointX() * GameMap::getImageSize() + GameMap::getImageSize() / 2 - m_ZInformationLabel->getScaledWidth() / 2,
                                         pCursor->getMapPointY() * GameMap::getImageSize() - 5 - m_ZInformationLabel->getScaledHeight());
    }
}

void HumanPlayerInput::createCursorPath(qint32 x, qint32 y)
{
    Console::print("HumanPlayerInput::createCursorPath", Console::eDEBUG);
    QVector<QPoint> points = m_ArrowPoints;
    QPoint lastPoint = QPoint(-1, -1);
    if (points.size() > 0)
    {
        lastPoint = points[0];
    }
    deleteArrow();
    if (m_pGameAction->getTarget() != QPoint(x, y) &&
        m_pUnitPathFindingSystem.get() != nullptr &&
        !m_pGameAction->getTargetUnit()->getHasMoved() &&
        m_FieldPoints.contains(QVector3D(x, y, 0)))
    {
        if (m_pUnitPathFindingSystem->getCosts(m_pUnitPathFindingSystem->getIndex(x, y), x, y, x, y) >= 0)
        {
            // is it a neighbour field to the last target?
            if (((points.size() > 0) && ((points[0].x() - x + points[0].y() - y) != 0)))
            {
                if ((points.size() > 0) && ((qAbs(points[0].x() - x) + qAbs(points[0].y() - y)) == 1))
                {
                    if (points.contains(QPoint(x, y)))
                    {
                        points = m_pUnitPathFindingSystem->getPath(x, y);
                    }
                    else if (m_pUnitPathFindingSystem->getCosts(m_pUnitPathFindingSystem->getIndex(x, y), x, y, points[0].x(), points[0].y()) >= 0)
                    {
                        points.push_front(QPoint(x, y));
                        qint32 movepoints = m_pGameAction->getTargetUnit()->getMovementpoints(QPoint(x, y));
                        if ((m_pUnitPathFindingSystem->getTargetCosts(x, y)  <= movepoints) &&
                            (m_pUnitPathFindingSystem->getCosts(points)  > movepoints))
                        {
                            // not reachable this way get the ideal path
                            points = m_pUnitPathFindingSystem->getPath(x, y);
                        }
                    }
                    else
                    {
                        points = m_pUnitPathFindingSystem->getPath(x, y);
                    }
                }
                else
                {
                    points = m_pUnitPathFindingSystem->getPath(x, y);
                }
            }
            else if (points.size() == 0)
            {
                points = m_pUnitPathFindingSystem->getPath(x, y);
            }
            else
            {
                // do nothing
            }
            if ((points.size() == 0) ||
                (points[0].x() != x) ||
                (points[0].y() != y))
            {
                points = m_pUnitPathFindingSystem->getPath(x, y);
            }
            m_pGameAction->setCosts(m_pUnitPathFindingSystem->getCosts(points));
            m_ArrowPoints = points;
            createArrow(m_ArrowPoints);
        }
    }
}

QStringList HumanPlayerInput::getEmptyActionList()
{
    Console::print("HumanPlayerInput::getEmptyActionList", Console::eDEBUG);
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue value = pInterpreter->doFunction("ACTION", "getEmptyFieldActions");
    if (value.isString())
    {
        return value.toString().split(",");
    }
    else
    {
        return value.toVariant().toStringList();
    }
}

QStringList HumanPlayerInput::getViewplayerActionList()
{
    Console::print("HumanPlayerInput::getViewplayerActionList", Console::eDEBUG);
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue value = pInterpreter->doFunction("ACTION", "getViewplayerActionList");
    if (value.isString())
    {
        return value.toString().split(",");
    }
    else
    {
        return value.toVariant().toStringList();
    }
}

void HumanPlayerInput::createArrow(QVector<QPoint>& points)
{
    spGameMap pMap = GameMap::getInstance();
    GameManager* pGameManager = GameManager::getInstance();
    for (qint32 i = 0; i < points.size() - 1; i++)
    {
        oxygine::spSprite pSprite = oxygine::spSprite::create();
        oxygine::ResAnim* pAnim = pGameManager->getResAnim("arrow+unit");
        pSprite->setResAnim(pAnim);
        pSprite->setPriority(static_cast<qint16>(points[i].y() + 3));
        pSprite->setScale((GameMap::getImageSize()) / pAnim->getWidth());
        pSprite->setPosition(points[i].x() * GameMap::getImageSize() -(pSprite->getScaledWidth() - GameMap::getImageSize()) / 2,  points[i].y() * GameMap::getImageSize() -(pSprite->getScaledHeight() - GameMap::getImageSize()));
        pMap->addChild(pSprite);
        m_Arrows.append(pSprite);

        if (i > 0)
        {
            // select arrow
            if (((points[i].x() < points[i + 1].x()) && (points[i].x() > points[i - 1].x())) ||
                ((points[i].x() < points[i - 1].x()) && (points[i].x() > points[i + 1].x())))
            {
                pSprite->setColumn(static_cast<qint32>(Arrows::LeftRight));
            }
            else if (((points[i].y() < points[i + 1].y()) && (points[i].y() > points[i - 1].y())) ||
                     ((points[i].y() < points[i - 1].y()) && (points[i].y() > points[i + 1].y())))
            {
                pSprite->setColumn(static_cast<qint32>(Arrows::UpDown));
            }
            else if (((points[i].y() < points[i + 1].y()) && (points[i].x() < points[i - 1].x())) ||
                     ((points[i].y() < points[i - 1].y()) && (points[i].x() < points[i + 1].x())))
            {
                pSprite->setColumn(static_cast<qint32>(Arrows::DownRight));
            }
            else if (((points[i].y() < points[i + 1].y()) && (points[i].x() > points[i - 1].x())) ||
                     ((points[i].y() < points[i - 1].y()) && (points[i].x() > points[i + 1].x())))
            {
                pSprite->setColumn(static_cast<qint32>(Arrows::DownLeft));
            }
            else if (((points[i].y() > points[i + 1].y()) && (points[i].x() < points[i - 1].x())) ||
                     ((points[i].y() > points[i - 1].y()) && (points[i].x() < points[i + 1].x())))
            {
                pSprite->setColumn(static_cast<qint32>(Arrows::UpRight));
            }
            else if (((points[i].y() > points[i + 1].y()) && (points[i].x() > points[i - 1].x())) ||
                     ((points[i].y() > points[i - 1].y()) && (points[i].x() > points[i + 1].x())))
            {
                pSprite->setColumn(static_cast<qint32>(Arrows::UpLeft));
            }
        }
        else
        {
            // final arrow
            if (points[i].x() < points[i + 1].x())
            {
                pSprite->setColumn(static_cast<qint32>(Arrows::Right));
            }
            else if (points[i].x() > points[i + 1].x())
            {
                pSprite->setColumn(static_cast<qint32>(Arrows::Left));
            }
            else if (points[i].y() < points[i + 1].y())
            {
                pSprite->setColumn(static_cast<qint32>(Arrows::Down));
            }
            else if (points[i].y() > points[i + 1].y())
            {
                pSprite->setColumn(static_cast<qint32>(Arrows::Up));
            }
        }
    }
}

void HumanPlayerInput::deleteArrow()
{
    for (qint32 i = 0; i < m_Arrows.size(); i++)
    {
        m_Arrows[i]->detach();
    }
    m_ArrowPoints.clear();
    m_Arrows.clear();
}

void HumanPlayerInput::keyDown(oxygine::KeyEvent event)
{
    spGameMenue pMenu = GameMenue::getInstance();
    if (pMenu.get() != nullptr &&
        GameMap::getInstance()->getCurrentPlayer() == m_pPlayer &&
        pMenu->getFocused())
    {
        if (GameAnimationFactory::getAnimationCount() == 0)
        {
            // for debugging
            Qt::Key cur = event.getKey();
            if (cur == Settings::getKey_next() ||
                cur == Settings::getKey_next2())
            {
                if (m_FieldPoints.size() > 0)
                {
                    nextMarkedField();
                }
                else
                {
                    nextSelectOption();
                }
            }
            else if (cur == Settings::getKey_previous() ||
                     cur == Settings::getKey_previous2())
            {
                if (m_FieldPoints.size() > 0)
                {
                    previousMarkedField();
                }
                else
                {
                    previousSelectOption();
                }
            }
            else if (cur == Settings::getKey_ShowAttackFields() ||
                     cur == Settings::getKey_ShowAttackFields2())
            {
                showSelectedUnitAttackableFields(true);
            }
            else if (cur == Settings::getKey_ShowIndirectAttackFields() ||
                     cur == Settings::getKey_ShowIndirectAttackFields2())
            {
                showSelectedUnitAttackableFields(false);
            }
        }
    }
}

void HumanPlayerInput::showSelectedUnitAttackableFields(bool all)
{
    Console::print("HumanPlayerInput::showSelectedUnitAttackableFields", Console::eDEBUG);
    if (m_pUnitPathFindingSystem.get() != nullptr &&
        m_pGameAction.get() != nullptr &&
        m_CurrentMenu.get() == nullptr)
    {
        if (m_InfoFields.size() > 0)
        {
            for (auto & fields : m_InfoFields)
            {
                fields->detach();
            }
            m_InfoFields.clear();
            for (auto & fields : m_Fields)
            {
                fields->setVisible(true);
            }
        }
        else
        {
            Mainapp::getInstance()->getAudioThread()->playSound("selectunit.wav");
            for (auto & fields : m_Fields)
            {
                fields->setVisible(false);
            }
            Unit* currentUnit = m_pGameAction->getTargetUnit();
            QPoint position = currentUnit->getPosition();
            qint32 distance = currentUnit->getMovementpoints(position);
            distance += currentUnit->getMaxRange(position);

            QVector<QPoint> usedFields;
            spGameMap pMap = GameMap::getInstance();
            for (qint32 x = 0; x < pMap->getMapWidth(); x++)
            {
                for (qint32 y = 0; y < pMap->getMapHeight(); y++)
                {
                    Unit* pUnit = pMap->getTerrain(x, y)->getUnit();
                    if ((pUnit != nullptr) &&
                        m_pPlayer->isEnemyUnit(pUnit) &&
                        (!pUnit->isStealthed(m_pPlayer)))
                    {
                        QPoint unitPositionpUnit(x, y);
                        qint32 currentMaxDistance = distance;
                        currentMaxDistance += pUnit->getMovementpoints(unitPositionpUnit);
                        currentMaxDistance += pUnit->getMaxRange(unitPositionpUnit);
                        if (GlobalUtils::getDistance(unitPositionpUnit, position) <= currentMaxDistance)
                        {
                            if(all || pUnit->getBaseMaxRange() > 1)
                            {
                                showUnitAttackFields(pUnit, usedFields);
                            }
                        }
                    }
                }
            }
        }
    }
    
}

void HumanPlayerInput::showUnitAttackFields(Unit* pUnit, QVector<QPoint> & usedFields)
{
    Mainapp::getInstance()->pauseRendering();
    UnitPathFindingSystem pfs(pUnit, m_pPlayer);
    QVector<QPoint> points;
    QPoint position = pUnit->getPosition();
    bool canMoveAndFire = pUnit->canMoveAndFire(position);
    if (canMoveAndFire)
    {
        pfs.explore();
        points = pfs.getAllNodePoints();
    }
    else
    {
        points.append(position);
    }
    spGameMap pMap = GameMap::getInstance();
    qint32 maxRange = pUnit->getMaxRange(position);
    qint32 minRange = pUnit->getMinRange(position);
    spQmlVectorPoint pPoints = spQmlVectorPoint(GlobalUtils::getCircle(minRange, maxRange));
    for (qint32 i = 0; i < points.size(); i++)
    {
        if (canMoveAndFire ||
            (points[i].x() == position.x() && points[i].y() == position.y()))
        {
            for (qint32 i2 = 0; i2 < pPoints->size(); i2++)
            {
                QPoint target = pPoints->at(i2) + points[i];
                if (pMap->onMap(target.x(), target.y()) &&
                    !usedFields.contains(QPoint(target.x(), target.y())))
                {
                    usedFields.append(target);
                    m_InfoFields.append(createMarkedFieldActor(target, QColor(255, 0, 0), Terrain::DrawPriority::MarkedFieldMap));
                }
            }
        }
    }
    syncMarkedFields();
    Mainapp::getInstance()->continueRendering();
}

void HumanPlayerInput::nextMarkedField()
{
    spGameMap pMap = GameMap::getInstance();
    spGameMenue pGameMenue = GameMenue::getInstance();
    bool center = Settings::getCenterOnMarkedField();
    if (pGameMenue.get() != nullptr)
    {
        qint32 width = pMap->getMapWidth();
        qint32 heigth = pMap->getMapHeight();
        qint32 startX = pGameMenue->getCursor()->getMapPointX();
        qint32 startY = pGameMenue->getCursor()->getMapPointY();
        qint32 x = startX + 1;
        qint32 y = startY;
        bool found = false;
        while (y  < heigth && !found)
        {
            while (x < width && !found)
            {
                for (qint32 i = 0; i < m_FieldPoints.size(); i++)
                {
                    if (x == static_cast<qint32>(m_FieldPoints[i].x()) &&
                        (y == static_cast<qint32>(m_FieldPoints[i].y())))
                    {
                        if (center)
                        {
                            pMap->centerMap(x, y);
                        }
                        pGameMenue->calcNewMousePosition(x, y);
                        found = true;
                        break;
                    }
                }
                x++;
            }
            y++;
            x = 0;
        }
        x = 0;
        y = 0;
        while (y  < heigth && !found )
        {
            while (x < width && !found)
            {
                for (qint32 i = 0; i < m_FieldPoints.size(); i++)
                {
                    if (x == static_cast<qint32>(m_FieldPoints[i].x()) &&
                        (y == static_cast<qint32>(m_FieldPoints[i].y())))
                    {
                        if (center)
                        {
                            pMap->centerMap(x, y);
                        }
                        pGameMenue->calcNewMousePosition(x, y);
                        found = true;
                        break;
                    }
                }
                x++;
                if ((y == startY && x == startX))
                {
                    found = true;
                }
            }
            y++;
            x = 0;
        }
    }
}

void HumanPlayerInput::previousMarkedField()
{
    spGameMap pMap = GameMap::getInstance();
    spGameMenue pGameMenue = GameMenue::getInstance();
    if (pGameMenue.get() != nullptr)
    {
        qint32 width = pMap->getMapWidth();
        qint32 heigth = pMap->getMapHeight();
        qint32 startX = pGameMenue->getCursor()->getMapPointX();
        qint32 startY = pGameMenue->getCursor()->getMapPointY();
        qint32 x = startX - 1;
        qint32 y = startY;
        bool found = false;
        bool center = Settings::getCenterOnMarkedField();
        while (y  >= 0 && !found)
        {
            while (x >= 0 && !found)
            {
                for (qint32 i = 0; i < m_FieldPoints.size(); i++)
                {
                    if (x == static_cast<qint32>(m_FieldPoints[i].x()) &&
                        (y == static_cast<qint32>(m_FieldPoints[i].y())))
                    {
                        if (center)
                        {
                            pMap->centerMap(x, y);
                        }
                        pGameMenue->calcNewMousePosition(x, y);
                        found = true;
                        break;
                    }
                }
                x--;
            }
            y--;
            x = width - 1;
        }
        x = width - 1;
        y = heigth - 1;
        while (y  >= 0 && !found )
        {
            while (x >= 0 && !found)
            {
                for (qint32 i = 0; i < m_FieldPoints.size(); i++)
                {
                    if (x == static_cast<qint32>(m_FieldPoints[i].x()) &&
                        (y == static_cast<qint32>(m_FieldPoints[i].y())))
                    {
                        if (center)
                        {
                            pMap->centerMap(x, y);
                        }
                        pGameMenue->calcNewMousePosition(x, y);
                        found = true;
                        break;
                    }
                }
                x--;
                if ((y == startY && x == startX))
                {
                    found = true;
                }
            }
            y--;
            x = width - 1;
        }
    }
}

void HumanPlayerInput::nextSelectOption()
{
    spGameMap pMap = GameMap::getInstance();
    spGameMenue pGameMenue = GameMenue::getInstance();
    if (pGameMenue.get() != nullptr)
    {
        qint32 width = pMap->getMapWidth();
        qint32 heigth = pMap->getMapHeight();
        qint32 startX = pGameMenue->getCursor()->getMapPointX();
        qint32 startY = pGameMenue->getCursor()->getMapPointY();
        qint32 x = startX + 1;
        qint32 y = startY;
        bool found = false;
        bool center = Settings::getCenterOnMarkedField();
        if (pMap->onMap(startX, startY))
        {
            while (y  < heigth && !found)
            {
                while (x < width && !found)
                {
                    Unit* pUnit = pMap->getTerrain(x, y)->getUnit();
                    Building* pBuilding = pMap->getTerrain(x, y)->getBuilding();
                    if (pUnit != nullptr &&
                        pUnit->getOwner() == m_pPlayer &&
                        !pUnit->getHasMoved())
                    {
                        if (center)
                        {
                            pMap->centerMap(x, y);
                        }
                        pGameMenue->calcNewMousePosition(x, y);
                        found = true;
                    }
                    else if ((pBuilding != nullptr) &&
                             (pBuilding->getOwner() == m_pPlayer))
                    {
                        GameAction action;
                        action.setTarget(QPoint(x, y));
                        QStringList actions = pBuilding->getActionList();
                        for (qint32 i = 0; i < actions.size(); i++)
                        {
                            if (action.canBePerformed(actions[i]))
                            {
                                if (center)
                                {
                                    pMap->centerMap(x, y);
                                }
                                pGameMenue->calcNewMousePosition(x, y);
                                found = true;
                                break;
                            }
                        }
                    }
                    x++;
                }
                y++;
                x = 0;
            }
            x = 0;
            y = 0;
            while (y  < heigth && !found )
            {
                while (x < width && !found)
                {
                    Unit* pUnit = pMap->getTerrain(x, y)->getUnit();
                    Building* pBuilding = pMap->getTerrain(x, y)->getBuilding();
                    if (pUnit != nullptr &&
                        pUnit->getOwner() == m_pPlayer &&
                        !pUnit->getHasMoved())
                    {
                        if (center)
                        {
                            pMap->centerMap(x, y);
                        }
                        pGameMenue->calcNewMousePosition(x, y);
                        found = true;
                    }
                    else if ((pBuilding != nullptr) &&
                             (pBuilding->getOwner() == m_pPlayer))
                    {
                        GameAction action;
                        action.setTarget(QPoint(x, y));
                        QStringList actions = pBuilding->getActionList();
                        for (qint32 i = 0; i < actions.size(); i++)
                        {
                            if (action.canBePerformed(actions[i]))
                            {
                                if (center)
                                {
                                    pMap->centerMap(x, y);
                                }
                                pGameMenue->calcNewMousePosition(x, y);
                                found = true;
                                break;
                            }
                        }
                    }
                    x++;
                    if ((y == startY && x == startX))
                    {
                        found = true;
                    }
                }
                y++;
                x = 0;
            }
        }
    }
}

void HumanPlayerInput::previousSelectOption()
{
    spGameMap pMap = GameMap::getInstance();
    spGameMenue pGameMenue = GameMenue::getInstance();
    if (pGameMenue.get() != nullptr)
    {
        qint32 width = pMap->getMapWidth();
        qint32 heigth = pMap->getMapHeight();
        qint32 startX = pGameMenue->getCursor()->getMapPointX();
        qint32 startY = pGameMenue->getCursor()->getMapPointY();
        qint32 x = startX - 1;
        qint32 y = startY;
        bool found = false;
        bool center = Settings::getCenterOnMarkedField();
        if (pMap->onMap(startX, startY))
        {
            while (y  >= 0 && !found)
            {
                while (x >= 0 && !found)
                {
                    Unit* pUnit = pMap->getTerrain(x, y)->getUnit();
                    Building* pBuilding = pMap->getTerrain(x, y)->getBuilding();
                    if (pUnit != nullptr &&
                        pUnit->getOwner() == m_pPlayer &&
                        !pUnit->getHasMoved())
                    {
                        if (center)
                        {
                            pMap->centerMap(x, y);
                        }
                        pGameMenue->calcNewMousePosition(x, y);
                        found = true;
                    }
                    else if ((pBuilding != nullptr) &&
                             (pBuilding->getOwner() == m_pPlayer))
                    {
                        GameAction action;
                        action.setTarget(QPoint(x, y));
                        QStringList actions = pBuilding->getActionList();
                        for (qint32 i = 0; i < actions.size(); i++)
                        {
                            if (action.canBePerformed(actions[i]))
                            {
                                if (center)
                                {
                                    pMap->centerMap(x, y);
                                }
                                pGameMenue->calcNewMousePosition(x, y);
                                found = true;
                                break;
                            }
                        }
                    }
                    x--;
                }
                y--;
                x = width - 1;
            }
            x = width - 1;
            y = heigth - 1;
            while (y  >= 0 && !found )
            {
                while (x >= 0 && !found)
                {
                    Unit* pUnit = pMap->getTerrain(x, y)->getUnit();
                    Building* pBuilding = pMap->getTerrain(x, y)->getBuilding();
                    if (pUnit != nullptr &&
                        pUnit->getOwner() == m_pPlayer &&
                        !pUnit->getHasMoved())
                    {
                        if (center)
                        {
                            pMap->centerMap(x, y);
                        }
                        pGameMenue->calcNewMousePosition(x, y);
                        found = true;
                    }
                    else if ((pBuilding != nullptr) &&
                             (pBuilding->getOwner() == m_pPlayer))
                    {
                        GameAction action;
                        action.setTarget(QPoint(x, y));
                        QStringList actions = pBuilding->getActionList();
                        for (qint32 i = 0; i < actions.size(); i++)
                        {
                            if (action.canBePerformed(actions[i]))
                            {
                                if (center)
                                {
                                    pMap->centerMap(x, y);
                                }
                                pGameMenue->calcNewMousePosition(x, y);
                                found = true;
                                break;
                            }
                        }
                    }
                    if ((y == startY && x == startX))
                    {
                        found = true;
                    }
                    x--;
                }
                y--;
                x = width - 1;
            }
        }
    }
}

void HumanPlayerInput::autoEndTurn()
{
    spGameMap pMap = GameMap::getInstance();
    if (m_pPlayer != nullptr &&
        pMap.get() != nullptr &&
        pMap->getCurrentPlayer() == m_pPlayer)
    {
        Console::print("HumanPlayerInput::autoEndTurn", Console::eDEBUG);
        CO* pCO0 = m_pPlayer->getCO(0);
        CO* pCO1 = m_pPlayer->getCO(1);
        if (Settings::getAutoEndTurn() &&
            GameAnimationFactory::getAnimationCount() == 0 &&
            (pCO0 == nullptr || (!pCO0->canUsePower() && !pCO0->canUseSuperpower())) &&
            (pCO1 == nullptr || (!pCO1->canUsePower() && !pCO1->canUseSuperpower())))
        {
            qint32 heigth = pMap->getMapHeight();
            qint32 width = pMap->getMapWidth();
            for (qint32 y = 0; y < heigth; y++)
            {
                for (qint32 x = 0; x < width; x++)
                {
                    Terrain* pTerrain = pMap->getTerrain(x, y);
                    Unit* pUnit = pTerrain->getUnit();
                    Building* pBuilding = pTerrain->getBuilding();
                    if (pUnit != nullptr && pUnit->getOwner() == m_pPlayer && !pUnit->getHasMoved())
                    {
                        return;
                    }
                    if (pBuilding != nullptr && pBuilding->getOwner() == m_pPlayer)
                    {
                        GameAction action;
                        action.setTarget(QPoint(x, y));
                        QStringList actions = pBuilding->getActionList();
                        for (qint32 i = 0; i < actions.size(); i++)
                        {
                            action.setActionID(actions[i]);
                            if (action.canBePerformed())
                            {
                                return;
                            }
                        }
                    }
                }
            }
            Console::print("Auto triggering next player cause current player can't input any actions.", Console::eDEBUG);
            spGameAction pAction = spGameAction::create(CoreAI::ACTION_NEXT_PLAYER);
            emit performAction(pAction);
        }
    }
}

void HumanPlayerInput::serializeObject(QDataStream& stream) const
{
    stream << getVersion();
}

void HumanPlayerInput::deserializeObject(QDataStream& stream)
{
    qint32 version;
    stream >> version;
}

void HumanPlayerInput::centerCameraOnAction(GameAction* pAction)
{
    spGameMap pMap = GameMap::getInstance();
    spGameMenue pGameMenue = GameMenue::getInstance();
    if (pMap.get() != nullptr && pGameMenue.get() != nullptr &&
        (pMap->getCurrentPlayer() == m_pPlayer ||
         m_pPlayer == nullptr))
    {
        if (Settings::getAutoCamera() && pAction == nullptr &&
            m_lastMapView.x() != std::numeric_limits<qint32>::min())
        {
            switch (Settings::getAutoFocusing())
            {
                case GameEnums::AutoFocusing_Owned:
                {
                    GameMap::getInstance()->centerOnPlayer(m_pPlayer);
                    break;
                }
                case GameEnums::AutoFocusing_LastPos:
                {
                    pGameMenue->getMapSlidingActor()->setPosition(m_lastMapView.x(), m_lastMapView.y());
                    break;
                }
            }
        }
    }
}
