#include "veryeasyai.h"

#include "game/gameaction.h"

#include "game/player.h"

#include "game/co.h"
#include "game/building.h"
#include "game/unit.h"
#include "game/gamemap.h"

#include "game/unitpathfindingsystem.h"

#include "ai/targetedunitpathfindingsystem.h"

#include "resource_management/cospritemanager.h"
#include "resource_management/unitspritemanager.h"

#include "ai/islandmap.h"

VeryEasyAI::VeryEasyAI()
    : CoreAI(BaseGameInputIF::AiTypes::VeryEasy),
      m_COUnitTree("resources/aidata/very_easy/counit.tree", "resources/aidata/very_easy/counit.txt"),
      m_GeneralBuildingTree("resources/aidata/very_easy/generalbuilding.tree", "resources/aidata/very_easy/generalbuilding.txt"),
      m_AirportBuildingTree("resources/aidata/very_easy/airportbuilding.tree", "resources/aidata/very_easy/airportbuilding.txt"),
      m_HarbourBuildingTree("resources/aidata/very_easy/harbourbuilding.tree", "resources/aidata/very_easy/harbourbuilding.txt")
{
    Interpreter::setCppOwnerShip(this);
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());

    buildingValue = 1.0f;
    ownUnitValue = 1.0f;
}

void VeryEasyAI::process()
{
    QmlVectorBuilding* pBuildings = m_pPlayer->getBuildings();
    pBuildings->randomize();
    QmlVectorUnit* pUnits = m_pPlayer->getUnits();
    pUnits->randomize();

    QmlVectorUnit* pEnemyUnits = m_pPlayer->getEnemyUnits();
    pEnemyUnits->randomize();
    QmlVectorBuilding* pEnemyBuildings = m_pPlayer->getEnemyBuildings();
    pEnemyBuildings->randomize();

    // create island maps at the start of turn
    if (rebuildIslandMaps)
    {
        rebuildIslandMaps = false;
        // delete island maps of the last turn
        m_IslandMaps.clear();
    }
    rebuildIsland(pUnits);

    // make the ai do stuff
    if (useCOPower(pUnits, pEnemyUnits)){}
    else
    {
        turnMode = TurnTime::onGoingTurn;
        if (useBuilding(pBuildings)){}
        else if (buildCOUnit(pUnits)){}
        else if (captureBuildings(pUnits)){}
        else if (CoreAI::moveOoziums(pUnits, pEnemyUnits)){}
        else if (CoreAI::moveBlackBombs(pUnits, pEnemyUnits)){}
        else if (fireWithIndirectUnits(pUnits)){}
        else if (fireWithDirectUnits(pUnits)){}
        else if (moveUnits(pUnits, pBuildings, pEnemyUnits, pEnemyBuildings)){}
        else if (loadUnits(pUnits)){}
        else if (moveRepair(pUnits)){}
        else if (moveTransporters(pUnits, pEnemyUnits, pEnemyBuildings)){}
        else if (moveAwayFromProduction(pUnits)){}
        else if (buildUnits(pBuildings, pUnits)){}
        else
        {
            turnMode = TurnTime::endOfTurn;
            if (useCOPower(pUnits, pEnemyUnits))
            {
                turnMode = TurnTime::onGoingTurn;
            }
            else
            {
                finishTurn();
            }
        }
    }
    delete pBuildings;
    delete pUnits;

    delete pEnemyBuildings;
    delete pEnemyUnits;
}


void VeryEasyAI::finishTurn()
{
    turnMode = TurnTime::startOfTurn;
    rebuildIslandMaps = true;
    CoreAI::finishTurn();
}



bool VeryEasyAI::buildCOUnit(QmlVectorUnit* pUnits)
{
    QVector<float> data;
    data.append(-1);
    data.append(-1);
    COSpriteManager* pCOSpriteManager = COSpriteManager::getInstance();
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        if (!pUnit->getHasMoved())
        {
            GameAction* pAction = new GameAction();
            for (quint8 i2 = 0; i2 < 2; i2++)
            {
                if (i2 == 0)
                {
                    pAction->setActionID(ACTION_CO_UNIT_0);
                }
                else
                {
                    pAction->setActionID(ACTION_CO_UNIT_1);
                }
                CO* pCO = m_pPlayer->getCO(i2);

                if (pCO != nullptr &&
                    pCO->getCOUnit() == nullptr)
                {
                    data[0] = pCOSpriteManager->getCOIndex(pCO->getCoID());

                    data[1] = pUnitSpriteManager->getUnitIndex(pUnit->getUnitID());
                    float ret = m_COUnitTree.getDecision(data);
                    if (ret == 1.0f)
                    {
                        pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
                        if (pAction->canBePerformed())
                        {
                            emit performAction(pAction);
                            return true;
                        }
                    }
                }
            }
            delete pAction;
        }
    }
    return false;
}

bool VeryEasyAI::captureBuildings(QmlVectorUnit* pUnits)
{
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        if (!pUnit->getHasMoved())
        {
            if (pUnit->getActionList().contains(ACTION_CAPTURE))
            {
                if (pUnit->getCapturePoints() > 0)
                {
                    GameAction* pAction = new GameAction(ACTION_CAPTURE);
                    pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
                    emit performAction(pAction);
                    return true;
                }
                else
                {
                    GameAction* pAction = new GameAction(ACTION_CAPTURE);
                    pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
                    UnitPathFindingSystem pfs(pUnit);
                    pfs.explore();
                    QVector<QPoint> targets = pfs.getAllNodePoints();
                    for (qint32 i2 = 0; i2 < targets.size(); i2++)
                    {
                        if (pUnit->getX() == targets[i2].x() &&
                            pUnit->getY() == targets[i2].y())
                        {
                            pAction->setMovepath(QVector<QPoint>());
                        }
                        else
                        {
                            pAction->setMovepath(pfs.getPath(targets[i2].x(), targets[i2].y()));
                        }
                        if (pAction->canBePerformed())
                        {
                            emit performAction(pAction);
                            return true;
                        }
                    }
                    // try to fire a missile instead
                    pAction->setActionID(ACTION_MISSILE);
                    for (qint32 i2 = 0; i2 < targets.size(); i2++)
                    {
                        if (pUnit->getX() == targets[i2].x() &&
                            pUnit->getY() == targets[i2].y())
                        {
                            pAction->setMovepath(QVector<QPoint>());
                        }
                        else
                        {
                            pAction->setMovepath(pfs.getPath(targets[i2].x(), targets[i2].y()));
                        }
                        if (pAction->canBePerformed())
                        {
                            // select a target for the rocket and launch it
                            QPoint rocketTarget = m_pPlayer->getRockettarget(2, 3);
                            CoreAI::addSelectedFieldData(pAction, rocketTarget);
                            emit performAction(pAction);
                            return true;
                        }
                    }
                    delete pAction;
                }
            }
        }
    }
    return false;
}

bool VeryEasyAI::fireWithIndirectUnits(QmlVectorUnit* pUnits)
{
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        // can we use the unit?
        if (!pUnit->getHasMoved() && pUnit->getBaseMaxRange() > 1 &&
            (pUnit->getAmmo1() > 0 || pUnit->getAmmo2() > 0))
        {
            if (attack(pUnit))
            {
                return true;
            }
        }
    }
    return false;
}

bool VeryEasyAI::fireWithDirectUnits(QmlVectorUnit* pUnits)
{
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        // can we use the unit?
        if (!pUnit->getHasMoved() && pUnit->getMaxRange() == 1 &&
            (pUnit->getAmmo1() > 0 || pUnit->getAmmo2() > 0))
        {
            if (attack(pUnit))
            {
                return true;
            }
        }
    }
    return false;
}

bool VeryEasyAI::attack(Unit* pUnit)
{
    if (pUnit->getActionList().contains(ACTION_FIRE))
    {
        // try to perform an attack
        GameAction* pAction = new GameAction(ACTION_FIRE);
        pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
        UnitPathFindingSystem pfs(pUnit);
        pfs.explore();
        QVector<QVector3D> ret;
        QVector<QVector3D> moveTargetFields;
        CoreAI::getBestTarget(pUnit, pAction, &pfs, ret, moveTargetFields);
        float minDamage = -pUnit->getUnitValue() / 4.0f;
        if (minDamage > - 500.0f)
        {
            minDamage = -500.0f;
        }
        if (ret.size() > 0 && ret[0].z() >= minDamage)
        {
            qint32 selection = Mainapp::randInt(0, ret.size() - 1);
            QVector3D target = ret[selection];
            QPoint point = pAction->getTarget();
            if (static_cast<qint32>(moveTargetFields[selection].x()) != point.x() ||
                static_cast<qint32>(moveTargetFields[selection].y()) != point.y())
            {
                pAction->setMovepath(pfs.getPath(static_cast<qint32>(moveTargetFields[selection].x()),
                                                 static_cast<qint32>(moveTargetFields[selection].y())));
            }
            else
            {
                pAction->setMovepath(QVector<QPoint>());
            }
            CoreAI::addSelectedFieldData(pAction, QPoint(static_cast<qint32>(target.x()), static_cast<qint32>(target.y())));
            // attacing none unit targets may modify the islands for a unit -> rebuild all for the love of god
            if (GameMap::getInstance()->getTerrain(static_cast<qint32>(target.x()), static_cast<qint32>(target.y()))->getUnit() == nullptr)
            {
                rebuildIslandMaps = true;
            }

            if (pAction->isFinalStep())
            {
                emit performAction(pAction);
                return true;
            }
        }
        delete pAction;
    }
    return false;
}

bool VeryEasyAI::moveUnits(QmlVectorUnit* pUnits, QmlVectorBuilding* pBuildings,
                           QmlVectorUnit* pEnemyUnits, QmlVectorBuilding* pEnemyBuildings)
{
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        // can we use the unit?
        if (!pUnit->getHasMoved())
        {
            QVector<QVector3D> targets;
            QVector<QVector3D> transporterTargets;
            GameAction* pAction = new GameAction(ACTION_CAPTURE);
            QStringList actions = pUnit->getActionList();
            // find possible targets for this unit
            pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));

            // find some cool targets
            appendCaptureTargets(actions, pUnit, pEnemyBuildings, targets);
            if (targets.size() > 0)
            {
                appendCaptureTransporterTargets(pUnit, pUnits, pEnemyBuildings, transporterTargets);
                targets.append(transporterTargets);
            }
            appendAttackTargets(pUnit, pEnemyUnits, targets);
            appendAttackTargetsIgnoreOwnUnits(pUnit, pEnemyUnits, targets);
            if (targets.size() == 0)
            {
                if ((pUnit->getMaxAmmo1() > 0 && !pUnit->hasAmmo1()) ||
                    (pUnit->getMaxAmmo2() > 0 && !pUnit->hasAmmo2()) ||
                    (pUnit->getMaxFuel() > 0 && static_cast<float>(pUnit->getFuel()) / static_cast<float>(pUnit->getMaxFuel()) < 1.0f / 3.0f))
                {
                    appendRepairTargets(pUnit, pBuildings, targets);
                }
            }
            // force resupply when low on fuel
            else if (static_cast<float>(pUnit->getFuel()) / static_cast<float>(pUnit->getMaxFuel()) < 1.0f / 3.0f)
            {
                targets.clear();
                appendRepairTargets(pUnit, pBuildings, targets);
            }
            if (moveUnit(pAction, pUnit, actions, targets, transporterTargets))
            {
                return true;
            }
        }
    }
    return false;
}


bool VeryEasyAI::moveTransporters(QmlVectorUnit* pUnits, QmlVectorUnit* pEnemyUnits, QmlVectorBuilding* pEnemyBuildings)
{
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        // can we use the unit?
        if (!pUnit->getHasMoved() && pUnit->getLoadingPlace() > 0)
        {
            // wooohooo it's a transporter
            if (pUnit->getLoadedUnitCount() > 0)
            {
                GameAction* pAction = new GameAction(ACTION_WAIT);
                QStringList actions = pUnit->getActionList();
                pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
                // find possible targets for this unit
                QVector<QVector3D> targets;
                // can one of our units can capture buildings?
                for (qint32 i = 0; i < pUnit->getLoadedUnitCount(); i++)
                {
                    Unit* pLoaded = pUnit->getLoadedUnit(i);
                    if (pLoaded->getActionList().contains(ACTION_CAPTURE))
                    {
                        appendUnloadTargetsForCapturing(pUnit, pEnemyBuildings, targets);
                        break;
                    }
                }
                // if not find closest unloading field
                if (targets.size() == 0 || pUnit->getLoadedUnitCount() > 1)
                {
                    appendNearestUnloadTargets(pUnit, pEnemyUnits, pEnemyBuildings, targets);
                }
                if (moveUnit(pAction, pUnit, actions, targets, targets, true))
                {
                    return true;
                }
            }
            else
            {
                GameAction* pAction = new GameAction(ACTION_WAIT);
                QStringList actions = pUnit->getActionList();
                // find possible targets for this unit
                pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
                // we need to move to a loading place
                QVector<QVector3D> targets;
                QVector<QVector3D> transporterTargets;
                appendLoadingTargets(pUnit, pUnits, pEnemyUnits, pEnemyBuildings, false, false, targets);
                if (targets.size() == 0)
                {
                    appendLoadingTargets(pUnit, pUnits, pEnemyUnits, pEnemyBuildings, true, false, targets);
                }
                if (moveUnit(pAction, pUnit, actions, targets, transporterTargets))
                {
                    return true;
                }
            }
        }
    }
    return false;
}


bool VeryEasyAI::loadUnits(QmlVectorUnit* pUnits)
{
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        // can we use the unit?
        if (!pUnit->getHasMoved() &&
            // we don't support multi transporting for the ai for now this will break the system trust me
            pUnit->getLoadingPlace() <= 0)
        {
            QVector<QVector3D> targets;
            QVector<QVector3D> transporterTargets;
            GameAction* pAction = new GameAction(ACTION_LOAD);
            QStringList actions = pUnit->getActionList();
            // find possible targets for this unit
            pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));

            // find some cool targets
            appendTransporterTargets(pUnit, pUnits, transporterTargets);
            targets.append(transporterTargets);
            // till now the selected targets are a little bit lame cause we only search for reachable transporters
            // but not for reachable loading places.
            if (moveUnit(pAction, pUnit, actions, targets, transporterTargets))
            {
                return true;
            }
        }
    }
    return false;
}

bool VeryEasyAI::moveUnit(GameAction* pAction, Unit* pUnit, QStringList& actions,
                          QVector<QVector3D>& targets, QVector<QVector3D>& transporterTargets, bool unload)
{
    TargetedUnitPathFindingSystem pfs(pUnit, targets);
    pfs.explore();
    qint32 movepoints = pUnit->getMovementpoints(QPoint(pUnit->getX(), pUnit->getY()));
    QPoint targetFields = pfs.getReachableTargetField(movepoints);
    if (targetFields.x() >= 0)
    {
        UnitPathFindingSystem turnPfs(pUnit);
        turnPfs.explore();

        if (CoreAI::contains(transporterTargets, targetFields))
        {
            if (unload)
            {
                pAction->setMovepath(turnPfs.getPath(targetFields.x(), targetFields.y()));
                pAction->setActionID(ACTION_UNLOAD);
                // this isn't ideal since we unload the units in a random order and not the best fitting order
                // but we're lazy here for the very easy ai
                while (true)
                {
                    MenuData* pDataMenu = pAction->getMenuStepData();
                    QStringList actions = pDataMenu->getActionIDs();
                    qint32 costs = pDataMenu->getCostList()[0];
                    delete pDataMenu;
                    if (actions.size() == 1)
                    {
                        break;
                    }
                    else
                    {
                        addMenuItemData(pAction, actions[0], costs);
                    }
                    MarkedFieldData* pFields = pAction->getMarkedFieldStepData();
                    qint32 field = Mainapp::randInt(0, pFields->getPoints()->size() - 1);
                    addSelectedFieldData(pAction, pFields->getPoints()->at(field));
                    delete pFields;
                }
                addMenuItemData(pAction, ACTION_WAIT, 0);
                emit performAction(pAction);
                return true;
            }
            else
            {
                pAction->setMovepath(turnPfs.getPath(targetFields.x(), targetFields.y()));
                pAction->setActionID(ACTION_LOAD);
                emit performAction(pAction);
                return true;
            }
        }
        else
        {
            pAction->setMovepath(turnPfs.getClosestReachableMovePath(targetFields));
            if (actions.contains(ACTION_RATION))
            {
                pAction->setActionID(ACTION_RATION);
                if (pAction->canBePerformed())
                {
                    emit performAction(pAction);
                    return true;
                }
            }
            if (actions.contains(ACTION_STEALTH))
            {
                pAction->setActionID(ACTION_STEALTH);
                if (pAction->canBePerformed())
                {
                    emit performAction(pAction);
                    return true;
                }
            }
            if (actions.contains(ACTION_UNSTEALTH))
            {
                pAction->setActionID(ACTION_UNSTEALTH);
                if (pAction->canBePerformed())
                {
                    emit performAction(pAction);
                    return true;
                }
            }
            pAction->setActionID(ACTION_WAIT);
            emit performAction(pAction);
            return true;
        }
    }
    delete pAction;
    return false;
}

bool VeryEasyAI::buildUnits(QmlVectorBuilding* pBuildings, QmlVectorUnit* pUnits)
{
    GameMap* pMap = GameMap::getInstance();
    QVector<float> data;
    qint32 productionBuildings = 0;
    for (qint32 i = 0; i < pBuildings->size(); i++)
    {
        Building* pBuilding = pBuildings->at(i);
        if (pBuilding->isProductionBuilding() &&
            pMap->getTerrain(pBuilding->getX(), pBuilding->getY())->getUnit() == nullptr)
        {
            productionBuildings++;
        }
    }
    qint32 infantryUnits = 0;
    qint32 indirectUnits = 0;
    qint32 directUnits = 0;
    qint32 transporterUnits = 0;
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        if (pUnit->getActionList().contains(ACTION_CAPTURE))
        {
            infantryUnits++;
        }
        if (pUnit->getBaseMaxRange() > 1)
        {
            indirectUnits++;
        }
        else
        {
            directUnits++;
        }
        if (pUnit->getLoadingPlace() > 0 &&
            pUnit->getWeapon1ID().isEmpty() &&
            pUnit->getWeapon2ID().isEmpty())
        {
            transporterUnits++;
        }
    }
    data.append(m_pPlayer->getFunds());
    data.append(m_pPlayer->getFunds() / static_cast<float>(productionBuildings));
    data.append(pUnits->size());
    if (indirectUnits > 0)
    {
        data.append(directUnits / static_cast<float>(indirectUnits));
    }
    else
    {
        data.append(1.0f);
    }
    data.append(infantryUnits);
    data.append(transporterUnits);

    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    for (qint32 i2 = 0; i2 < 10; i2++)
    {
        if (i2 == 0 || m_pPlayer->getFunds() >= 8000)
        {
            for (qint32 i = 0; i < pBuildings->size(); i++)
            {
                Building* pBuilding = pBuildings->at(i);
                if (pBuilding->isProductionBuilding() &&
                    pBuilding->getTerrain()->getUnit() == nullptr)
                {
                    GameAction* pAction = new GameAction(ACTION_BUILD_UNITS);
                    pAction->setTarget(QPoint(pBuilding->getX(), pBuilding->getY()));
                    if (pAction->canBePerformed())
                    {
                        // we're allowed to build units here
                        MenuData* pData = pAction->getMenuStepData();
                        qint32 selectedUnit = -1;
                        if (pBuilding->getBuildingID() == "AIRPORT")
                        {
                            selectedUnit = static_cast<qint32>(m_AirportBuildingTree.getDecision(data));
                        }
                        else if (pBuilding->getBuildingID() == "HARBOUR")
                        {
                            selectedUnit = static_cast<qint32>(m_HarbourBuildingTree.getDecision(data));
                        }
                        else
                        {
                            selectedUnit = static_cast<qint32>(m_GeneralBuildingTree.getDecision(data));
                        }
                        if (selectedUnit >= 0)
                        {
                            QString unitID = pUnitSpriteManager->getUnitID(selectedUnit);
                            qint32 menuIndex = pData->getActionIDs().indexOf(unitID);
                            if (menuIndex >= 0 && pData->getEnabledList()[menuIndex])
                            {
                                CoreAI::addMenuItemData(pAction, unitID, pData->getCostList()[menuIndex]);
                                delete pData;
                                // produce the unit
                                if (pAction->isFinalStep())
                                {
                                    emit performAction(pAction);
                                    return true;
                                }
                            }
                        }
                        delete pData;
                    }
                    delete pAction;
                }
            }
        }
    }
    return false;
}

void VeryEasyAI::serializeObject(QDataStream& stream)
{
    stream << getVersion();
    stream << enableNeutralTerrainAttack;
}
void VeryEasyAI::deserializeObject(QDataStream& stream)
{
    qint32 version;
    stream >> version;
    if (version > 1)
    {
        stream >> enableNeutralTerrainAttack;
    }
}

