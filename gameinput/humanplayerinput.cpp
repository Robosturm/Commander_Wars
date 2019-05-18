#include "humanplayerinput.h"

#include "game/gamemap.h"

#include "game/gameaction.h"

#include "game/terrain.h"

#include "game/unitpathfindingsystem.h"

#include "game/building.h"

#include "game/player.h"
#include "game/co.h"

#include "game/gameanimationfactory.h"
#include "resource_management/gamemanager.h"
#include "resource_management/fontmanager.h"

#include "coreengine/mainapp.h"

#include "coreengine/interpreter.h"

#include "gameinput/markedfielddata.h"

#include "ai/coreai.h"

HumanPlayerInput::HumanPlayerInput()
    : BaseGameInputIF(BaseGameInputIF::AiTypes::Human)
{    
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

void HumanPlayerInput::init()
{
    Mainapp* pApp = Mainapp::getInstance();
    GameMenue* pMenue = GameMenue::getInstance();
    connect(pMenue, &GameMenue::sigRightClick, this, &HumanPlayerInput::rightClick, Qt::QueuedConnection);
    connect(pMenue, &GameMenue::sigLeftClick, this, &HumanPlayerInput::leftClick, Qt::QueuedConnection);
    connect(pMenue, &GameMenue::sigActionPerformed, this, &HumanPlayerInput::autoEndTurn, Qt::QueuedConnection);
    connect(pApp, &Mainapp::sigKeyDown, this, &HumanPlayerInput::keyInput, Qt::QueuedConnection);
    connect(pMenue->getCursor(), &Cursor::sigCursorMoved, this, &HumanPlayerInput::cursorMoved, Qt::QueuedConnection);
    connect(this, &HumanPlayerInput::performAction, pMenue, &GameMenue::performAction, Qt::QueuedConnection);
}

HumanPlayerInput::~HumanPlayerInput()
{
    delete m_pGameAction;
    m_pGameAction = nullptr;
    delete m_pUnitPathFindingSystem;
    m_pUnitPathFindingSystem = nullptr;
}


void HumanPlayerInput::rightClick(qint32 x, qint32 y)
{
    if (!GameMap::getInstance()->onMap(x, y))
    {
        return;
    }
    if (GameMap::getInstance()->getCurrentPlayer() == m_pPlayer)
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->suspendThread();
        if (GameAnimationFactory::getAnimationCount() > 0)
        {
            GameAnimationFactory::finishAllAnimations();
        }
        else if (m_pGameAction != nullptr)
        {
            Mainapp::getInstance()->getAudioThread()->playSound("cancel.wav");
            if (m_pGameAction->getInputStep() > 0)
            {
                // todo implement go back steps
                cleanUpInput();
            }
            else if (m_CurrentMenu.get() == nullptr)
            {
                cleanUpInput();
            }
            else
            {
                Unit* pUnit = m_pGameAction->getTargetUnit();
                if ((pUnit != nullptr) &&
                    (!pUnit->isStealthed(m_pPlayer)))
                {
                    if (m_pGameAction->getInputStep() == 0)
                    {
                        // go one step back :)
                        clearMenu();
                        createMarkedMoveFields();
                    }
                }
                else
                {
                    cleanUpInput();
                }
            }
        }
        else
        {
            Mainapp::getInstance()->getAudioThread()->playSound("selectunit.wav");
            showAttackableFields(x, y);
        }
        pApp->continueThread();
    }
}

void HumanPlayerInput::showAttackableFields(qint32 x, qint32 y)
{
    clearMarkedFields();
    // try to show fire ranges :)
    GameMap* pMap = GameMap::getInstance();
    Unit* pUnit = pMap->getTerrain(x, y)->getUnit();
    if ((pUnit != nullptr) &&
        (!pUnit->isStealthed(m_pPlayer)))
    {
        if (pUnit->hasAmmo1() || pUnit->hasAmmo2())
        {

            qint32 minRange = pUnit->getMinRange();
            qint32 maxRange = pUnit->getMaxRange();

            UnitPathFindingSystem pfs(pMap->getTerrain(x, y)->getUnit());
            pfs.explore();
            QVector<QPoint> points = pfs.getAllNodePoints();
            for (qint32 i = 0; i < points.size(); i++)
            {
                if (pUnit->canMoveAndFire(QPoint(x, y)) ||
                   (points[i].x() == x && points[i].y() == y))
                {
                    QmlVectorPoint* pPoints = Mainapp::getCircle(minRange, maxRange);
                    for (qint32 i2 = 0; i2 < pPoints->size(); i2++)
                    {
                        QPoint target = pPoints->at(i2) + points[i];
                        if (!m_FieldPoints.contains(QVector3D(target.x(), target.y(), 1)))
                        {
                            createMarkedField(target, QColor(255, 0, 0), Terrain::DrawPriority::MarkedFieldMap);
                        }
                    }
                    delete pPoints;
                }
            }
        }
    }
    else
    {
        Building* pBuilding = pMap->getTerrain(x, y)->getBuilding();
        if (pBuilding != nullptr)
        {
            QmlVectorPoint* pPoints = pBuilding->getActionTargetFields();
            QPoint targetOffset = pBuilding->getActionTargetOffset();
            QPoint buildingPos(pBuilding->getX(), pBuilding->getY());
            if (pPoints != nullptr)
            {
                for (qint32 i = 0; i < pPoints->size(); i++)
                {
                    createMarkedField(buildingPos + targetOffset + pPoints->at(i), QColor(255, 0, 0), Terrain::DrawPriority::MarkedFieldMap);
                }
                delete pPoints;
            }
        }
    }
}

void HumanPlayerInput::cleanUpInput()
{
    clearMenu();
    // delete action
    delete m_pGameAction;
    m_pGameAction = nullptr;
    delete m_pUnitPathFindingSystem;
    m_pUnitPathFindingSystem = nullptr;
    clearMarkedFields();
    deleteArrow();
    GameMenue* pMenue = GameMenue::getInstance();
    pMenue->getCursor()->changeCursor("cursor+default");
}

void HumanPlayerInput::clearMenu()
{
    if (m_CurrentMenu.get() != nullptr)
    {
        GameMenue* pMenue = GameMenue::getInstance();
        pMenue->setFocused(true);
        m_CurrentMenu->getParent()->removeChild(m_CurrentMenu);
        m_CurrentMenu = nullptr;
    }
}

void HumanPlayerInput::clearMarkedFields()
{
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < m_Fields.size(); i++)
    {
        if (static_cast<qint32>(m_FieldPoints[i].z()) == 0)
        {
            pMap->getTerrain(static_cast<qint32>(m_FieldPoints[i].x()), static_cast<qint32>(m_FieldPoints[i].y()))->removeChild(m_Fields[i]);
        }
        else
        {
            pMap->removeChild(m_Fields[i]);
        }
    }
    m_FieldPoints.clear();
    m_Fields.clear();
    if (m_pMarkedFieldData != nullptr)
    {
        delete m_pMarkedFieldData;
        m_pMarkedFieldData = nullptr;
    }
    if (m_ZInformationLabel.get() != nullptr)
    {
        GameMap::getInstance()->removeChild(m_ZInformationLabel);
        m_ZInformationLabel = nullptr;
    }
}

void HumanPlayerInput::leftClick(qint32 x, qint32 y)
{
    if (!GameMap::getInstance()->onMap(x, y))
    {
        return;
    }
    if (GameMap::getInstance()->getCurrentPlayer() == m_pPlayer)
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->suspendThread();
        if (GameAnimationFactory::getAnimationCount() > 0)
        {
            // do nothing
        }
        else if (m_CurrentMenu.get() != nullptr)
        {
            // do nothing
        }
        else if (m_pMarkedFieldData != nullptr)
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
        else if (m_pGameAction == nullptr)
        {
            // prepare action
            m_pGameAction = new GameAction();
            m_pGameAction->setTarget(QPoint(x, y));
            GameMap* pMap = GameMap::getInstance();
            Unit* pUnit = pMap->getTerrain(x, y)->getUnit();
            if ((pUnit != nullptr) &&
                (!pUnit->isStealthed(m_pPlayer)))
            {
                selectUnit(x, y);
            }
            else
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
                }
            }

        }
        // we want to select an action
        else if (m_pGameAction->getActionID() == "")
        {
            if ((m_pUnitPathFindingSystem != nullptr) &&
                (m_pUnitPathFindingSystem->isReachable(x, y)) &&
                (m_CurrentMenu.get() == nullptr))
            {
                Unit* pUnit = m_pGameAction->getTargetUnit();
                m_pGameAction->setMovepath(m_ArrowPoints);
                if (pUnit != nullptr)
                {
                    // we want to do something with this unit :)
                    QStringList actions = pUnit->getActionList();
                    QStringList possibleActions;
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
                cleanUpInput();
            }
        }
        else
        {
            //cleanUpInput();
        }
        pApp->continueThread();
    }    
}

void HumanPlayerInput::markedFieldSelected(QPoint point)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
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
    pApp->continueThread();
}

void HumanPlayerInput::menuItemSelected(QString itemID, qint32 cost)
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->suspendThread();
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
    pApp->continueThread();
}

void HumanPlayerInput::getNextStepData()
{
    clearMenu();
    clearMarkedFields();
    GameMenue* pMenue = GameMenue::getInstance();
    pMenue->getCursor()->changeCursor("cursor+default");

    QString stepType = m_pGameAction->getStepInputType();
    if (stepType.toUpper() == "MENU")
    {
        GameMap* pMap = GameMap::getInstance();
        MenuData* pData = m_pGameAction->getMenuStepData();
        m_CurrentMenu = new HumanPlayerInputMenu(pData->getTexts(), pData->getActionIDs(), pData->getIconList(), pData->getCostList(), pData->getEnabledList());
        m_CurrentMenu->setMenuPosition(m_pGameAction->getActionTarget().x() * GameMap::Imagesize, m_pGameAction->getActionTarget().y() * GameMap::Imagesize);
        pMap->addChild(m_CurrentMenu);
        GameMenue* pMenue = GameMenue::getInstance();
        pMenue->setFocused(false);
        connect(m_CurrentMenu.get(), &HumanPlayerInputMenu::sigItemSelected, this, &HumanPlayerInput::menuItemSelected, Qt::QueuedConnection);
        connect(m_CurrentMenu.get(), &HumanPlayerInputMenu::sigCanceled, this, &HumanPlayerInput::rightClick, Qt::QueuedConnection);
        delete pData;
    }
    else if (stepType.toUpper() == "FIELD")
    {
        MarkedFieldData* pData = m_pGameAction->getMarkedFieldStepData();
        QVector<QPoint>* pFields = pData->getPoints();
        for (qint32 i = 0; i < pFields->size(); i++)
        {
            createMarkedField(pFields->at(i), pData->getColor(), Terrain::DrawPriority::MarkedFieldMap);
        }
        m_pMarkedFieldData = pData;
        CursorData* pCursordata = m_pGameAction->getStepCursor();
        pMenue->getCursor()->changeCursor(pCursordata->getCursor(), pCursordata->getXOffset(), pCursordata->getYOffset(), pCursordata->getScale());
        delete pCursordata;
    }
}

void HumanPlayerInput::finishAction()
{
    emit performAction(m_pGameAction);
    m_pGameAction = nullptr;
    cleanUpInput();
}

void HumanPlayerInput::createActionMenu(QStringList actionIDs, qint32 x, qint32 y)
{
    clearMarkedFields();
    MenuData data;
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < actionIDs.size(); i++)
    {
        data.addData(GameAction::getActionText(actionIDs[i]), actionIDs[i], GameAction::getActionIcon(actionIDs[i]));
    }
    m_CurrentMenu = new HumanPlayerInputMenu(data.getTexts(), actionIDs, data.getIconList());
    m_CurrentMenu->setMenuPosition(x * GameMap::Imagesize, y * GameMap::Imagesize);
    pMap->addChild(m_CurrentMenu);
    GameMenue* pMenue = GameMenue::getInstance();
    pMenue->setFocused(false);
    connect(m_CurrentMenu.get(), &HumanPlayerInputMenu::sigItemSelected, this, &HumanPlayerInput::menuItemSelected, Qt::QueuedConnection);
    connect(m_CurrentMenu.get(), &HumanPlayerInputMenu::sigCanceled, this, &HumanPlayerInput::rightClick, Qt::QueuedConnection);
}

void HumanPlayerInput::selectUnit(qint32 x, qint32 y)
{
    Mainapp::getInstance()->getAudioThread()->playSound("selectunit.wav");
    Mainapp::getInstance()->getAudioThread()->playSound("selectunit.wav");
    GameMap* pMap = GameMap::getInstance();
    m_pUnitPathFindingSystem = new UnitPathFindingSystem(pMap->getTerrain(x, y)->getUnit());
    m_pUnitPathFindingSystem->explore();
    createMarkedMoveFields();
}

void HumanPlayerInput::createMarkedField(QPoint point, QColor color, Terrain::DrawPriority drawPriority)
{
    GameMap* pMap = GameMap::getInstance();
    if (pMap->onMap(point.x(), point.y()))
    {
        GameManager* pGameManager = GameManager::getInstance();
        oxygine::spSprite pSprite = new oxygine::Sprite();
        oxygine::ResAnim* pAnim = pGameManager->getResAnim("marked+field");
        if (pAnim->getTotalFrames() > 1)
        {
            oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
            pSprite->addTween(tween);
        }
        else
        {
            pSprite->setResAnim(pAnim);
        }
        oxygine::Sprite::TweenColor tweenColor(oxygine::Color(color.red(), color.green(), color.blue(), color.alpha()));
        oxygine::spTween tween2 = oxygine::createTween(tweenColor, 1);
        pSprite->addTween(tween2);

        pSprite->setScale(GameMap::Imagesize / pAnim->getWidth());

        if (drawPriority == Terrain::DrawPriority::MarkedFieldMap)
        {
            pSprite->setPosition(point.x() * GameMap::Imagesize, point.y() * GameMap::Imagesize);
            pSprite->setPriority(static_cast<qint16>(Mainapp::ZOrder::MarkedFields));
            pMap->addChild(pSprite);
            m_FieldPoints.append(QVector3D(point.x(), point.y(), 1));
        }
        else
        {
            pSprite->setPriority(static_cast<qint16>(drawPriority));
            pSprite->setPosition(-(pSprite->getScaledWidth() - GameMap::Imagesize) / 2, -(pSprite->getScaledHeight() - GameMap::Imagesize));
            pMap->getSpTerrain(point.x(), point.y())->addChild(pSprite);
            m_FieldPoints.append(QVector3D(point.x(), point.y(), 0));
        }
        m_Fields.append(pSprite);
    }
}

void HumanPlayerInput::createMarkedMoveFields()
{
    clearMarkedFields();
    if (m_pUnitPathFindingSystem != nullptr)
    {
        QVector<QPoint> points = m_pUnitPathFindingSystem->getAllNodePoints();
        for (qint32 i = 0; i < points.size(); i++)
        {
            createMarkedField(points[i], QColor(50, 230, 200, 255), Terrain::DrawPriority::MarkedFieldLow);
        }
    }
}

void HumanPlayerInput::cursorMoved(qint32 x, qint32 y)
{
    if (GameMap::getInstance()->getCurrentPlayer() == m_pPlayer)
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->suspendThread();
        if (m_pMarkedFieldData != nullptr)
        {
            if (m_pMarkedFieldData->getShowZData())
            {
                // marked field?
                if (m_pMarkedFieldData->getPoints()->contains(QPoint(x, y)))
                {
                    if (m_ZInformationLabel.get() != nullptr)
                    {
                        GameMap::getInstance()->removeChild(m_ZInformationLabel);
                        m_ZInformationLabel = nullptr;
                    }
                    m_ZInformationLabel = new oxygine::Actor();
                    GameManager* pGameManager = GameManager::getInstance();
                    oxygine::spSprite pSprite = new oxygine::Sprite();
                    oxygine::ResAnim* pAnim = pGameManager->getResAnim("z_information_label");
                    if (pAnim->getTotalFrames() > 1)
                    {
                        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim), pAnim->getTotalFrames() * GameMap::frameTime, -1);
                        pSprite->addTween(tween);
                    }
                    else
                    {
                        pSprite->setResAnim(pAnim);
                    }
                    oxygine::spSprite pSprite2 = new oxygine::Sprite();
                    oxygine::ResAnim* pAnim2 = pGameManager->getResAnim("z_information_label+mask");
                    if (pAnim2->getTotalFrames() > 1)
                    {
                        oxygine::spTween tween = oxygine::createTween(oxygine::TweenAnim(pAnim2), pAnim2->getTotalFrames() * GameMap::frameTime, -1);
                        pSprite2->addTween(tween);
                    }
                    else
                    {
                        pSprite2->setResAnim(pAnim2);
                    }
                    QColor color = m_pMarkedFieldData->getZLabelColor();
                    pSprite2->setColor(color.red(), color.green(), color.blue(), color.alpha());
                    m_ZInformationLabel->addChild(pSprite);
                    m_ZInformationLabel->addChild(pSprite2);
                    // add text to the label
                    oxygine::spTextField textField = new oxygine::TextField();
                    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getTimesFont10()).
                                               withColor(oxygine::Color(0, 0, 0)).
                                               alignLeft().
                                               alignTop();
                    textField->setStyle(style);
                    textField->setScale(0.6f);
                    textField->setY(-2);
                    textField->setText(m_pMarkedFieldData->getZLabelText().toStdString().c_str());
                    textField->attachTo(m_ZInformationLabel);

                    oxygine::spTextField textField2 = new oxygine::TextField();
                    textField2->setStyle(style);
                    textField2->setScale(0.7f);
                    textField2->setY(8);
                    textField2->setX(3);
                    QString labelText = "";
                    QPoint field(x, y);
                    for (qint32 i = 0; i < m_pMarkedFieldData->getPoints()->size(); i++)
                    {
                        if (m_pMarkedFieldData->getPoints()->at(i) == field)
                        {
                            labelText = QString::number(m_pMarkedFieldData->getZInformation()->at(i)) + " %";
                            break;
                        }
                    }
                    textField2->setText(labelText.toStdString().c_str());
                    textField2->attachTo(m_ZInformationLabel);

                    m_ZInformationLabel->setScale(1.5f);
                    m_ZInformationLabel->setPosition(x * GameMap::Imagesize - GameMap::Imagesize / 2.0f,
                                                     y * GameMap::Imagesize - GameMap::Imagesize * 1.75f);
                    m_ZInformationLabel->setPriority(static_cast<qint16>(Mainapp::ZOrder::Animation));
                    GameMap::getInstance()->addChild(m_ZInformationLabel);
                }
                else
                {
                    if (m_ZInformationLabel.get() != nullptr)
                    {
                        GameMap::getInstance()->removeChild(m_ZInformationLabel);
                        m_ZInformationLabel = nullptr;
                    }
                }
            }
        }
        else if (m_pUnitPathFindingSystem != nullptr)
        {
            if ((m_CurrentMenu.get() == nullptr) && m_pGameAction->getActionID() == "")
            {
                createCursorPath(x, y);
            }
        }
        pApp->continueThread();
    }

}

void HumanPlayerInput::createCursorPath(qint32 x, qint32 y)
{
    QVector<QPoint> points = m_ArrowPoints;
    deleteArrow();
    if (m_pGameAction->getTarget() != QPoint(x, y))
    {
        if (m_pUnitPathFindingSystem->getCosts(x, y) > 0)
        {
            // is it a neighbour field to the last target?
            qint32 fieldCosts = m_pUnitPathFindingSystem->getCosts(m_pGameAction->getTarget().x(), m_pGameAction->getTarget().y());
            if (((points.size() > 0) && ((points[0].x() - x + points[0].y() - y) != 0)))
            {
                if ((points.size() > 0) && ((qAbs(points[0].x() - x) + qAbs(points[0].y() - y)) == 1))
                {
                    if (points.contains(QPoint(x, y)))
                    {
                        points = m_pUnitPathFindingSystem->getPath(x, y);
                    }
                    else
                    {
                        points.push_front(QPoint(x, y));
                        if (m_pUnitPathFindingSystem->getCosts(points) - fieldCosts > m_pGameAction->getTargetUnit()->getMovementpoints(QPoint(x, y)))
                        {
                            // not reachable this way get the ideal path
                            points = m_pUnitPathFindingSystem->getPath(x, y);
                        }
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
            m_pGameAction->setCosts(m_pUnitPathFindingSystem->getCosts(points) - fieldCosts);
            m_ArrowPoints = points;
            GameMap* pMap = GameMap::getInstance();
            GameManager* pGameManager = GameManager::getInstance();
            for (qint32 i = 0; i < points.size() - 1; i++)
            {
                oxygine::spSprite pSprite = new oxygine::Sprite();
                oxygine::ResAnim* pAnim = pGameManager->getResAnim("arrow+unit");
                pSprite->setResAnim(pAnim);
                pSprite->setPriority(static_cast<qint16>(Terrain::DrawPriority::Arrow));
                pSprite->setScale(GameMap::Imagesize / pAnim->getWidth());
                pSprite->setPosition(-(pSprite->getScaledWidth() - GameMap::Imagesize) / 2, -(pSprite->getScaledHeight() - GameMap::Imagesize));
                pMap->getSpTerrain(points[i].x(), points[i].y())->addChild(pSprite);
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
    }
}

QStringList HumanPlayerInput::getEmptyActionList()
{
    Interpreter* pInterpreter = Mainapp::getInstance()->getInterpreter();
    QJSValue value = pInterpreter->doFunction("ACTION", "getEmptyFieldActions");
    if (value.isString())
    {
        return value.toString().split(",");
    }
    else
    {
        return QStringList();
    }
}

void HumanPlayerInput::deleteArrow()
{
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < m_Arrows.size(); i++)
    {
        pMap->getTerrain(m_ArrowPoints[i].x(), m_ArrowPoints[i].y())->removeChild(m_Arrows[i]);
    }
    m_ArrowPoints.clear();
    m_Arrows.clear();
}

void HumanPlayerInput::keyInput(SDL_Event event)
{
    if (GameMenue::getInstance() != nullptr &&
        GameMap::getInstance()->getCurrentPlayer() == m_pPlayer &&
        GameMenue::getInstance()->getFocused())
    {
        if (GameAnimationFactory::getAnimationCount() == 0)
        {
            // for debugging
            SDL_Keycode cur = event.key.keysym.sym;
            if (cur == Settings::getKey_next())
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
            else if (cur == Settings::getKey_previous())
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
        }
    }
}

void HumanPlayerInput::nextMarkedField()
{
    GameMap* pMap = GameMap::getInstance();
    GameMenue* pGameMenue = GameMenue::getInstance();
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

void HumanPlayerInput::previousMarkedField()
{
    GameMap* pMap = GameMap::getInstance();
    GameMenue* pGameMenue = GameMenue::getInstance();
    qint32 width = pMap->getMapWidth();
    qint32 heigth = pMap->getMapHeight();
    qint32 startX = pGameMenue->getCursor()->getMapPointX();
    qint32 startY = pGameMenue->getCursor()->getMapPointY();
    qint32 x = startX - 1;
    qint32 y = startY;
    bool found = false;
    while (y  >= 0 && !found)
    {
        while (x >= 0 && !found)
        {
            for (qint32 i = 0; i < m_FieldPoints.size(); i++)
            {
                if (x == static_cast<qint32>(m_FieldPoints[i].x()) &&
                   (y == static_cast<qint32>(m_FieldPoints[i].y())))
                {
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

void HumanPlayerInput::nextSelectOption()
{
    GameMap* pMap = GameMap::getInstance();
    GameMenue* pGameMenue = GameMenue::getInstance();
    qint32 width = pMap->getMapWidth();
    qint32 heigth = pMap->getMapHeight();
    qint32 startX = pGameMenue->getCursor()->getMapPointX();
    qint32 startY = pGameMenue->getCursor()->getMapPointY();
    qint32 x = startX + 1;
    qint32 y = startY;
    bool found = false;
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

void HumanPlayerInput::previousSelectOption()
{
    GameMap* pMap = GameMap::getInstance();
    GameMenue* pGameMenue = GameMenue::getInstance();
    qint32 width = pMap->getMapWidth();
    qint32 heigth = pMap->getMapHeight();
    qint32 startX = pGameMenue->getCursor()->getMapPointX();
    qint32 startY = pGameMenue->getCursor()->getMapPointY();
    qint32 x = startX - 1;
    qint32 y = startY;
    bool found = false;
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

void HumanPlayerInput::autoEndTurn()
{
    CO* pCO0 = m_pPlayer->getCO(0);
    CO* pCO1 = m_pPlayer->getCO(1);
    if (Settings::getAutoEndTurn() &&
        GameMap::getInstance()->getCurrentPlayer() == m_pPlayer &&
        GameAnimationFactory::getAnimationCount() == 0 &&
        (pCO0 == nullptr || (!pCO0->canUsePower() && !pCO0->canUseSuperpower())) &&
        (pCO1 == nullptr || (!pCO1->canUsePower() && !pCO1->canUseSuperpower())))
    {
        GameMap* pMap = GameMap::getInstance();
        qint32 heigth = pMap->getMapHeight();
        qint32 width = pMap->getMapWidth();
        for (qint32 y = 0; y < heigth; y++)
        {
            for (qint32 x = 0; x < width; x++)
            {
                Unit* pUnit = pMap->getTerrain(x, y)->getUnit();
                Building* pBuilding = pMap->getTerrain(x, y)->getBuilding();
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
                        if (action.canBePerformed(actions[i]))
                        {
                            return;
                        }
                    }
                }
            }
        }
        GameAction* pAction = new GameAction(CoreAI::ACTION_NEXT_PLAYER);
        emit performAction(pAction);
    }
}

void HumanPlayerInput::serializeObject(QDataStream&)
{
}
void HumanPlayerInput::deserializeObject(QDataStream&)
{
}
