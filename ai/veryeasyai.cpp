#include "veryeasyai.h"

#include "game/gameaction.h"

#include "game/player.h"

#include "game/co.h"

#include "game/unit.h"

#include "game/gamemap.h"

#include "game/unitpathfindingsystem.h"

#include "ai/targetedunitpathfindingsystem.h"

#include "resource_management/cospritemanager.h"
#include "resource_management/unitspritemanager.h"

#include "ai/islandmap.h"

VeryEasyAI::VeryEasyAI()
    : CoreAI(BaseGameInputIF::AiTypes::VeryEasy),
      m_COPowerTree("resources/aidata/very_easy/copower.tree", "resources/aidata/very_easy/copower.txt"),
      m_COUnitTree("resources/aidata/very_easy/counit.tree", "resources/aidata/very_easy/counit.txt"),
      m_GeneralBuildingTree("resources/aidata/very_easy/generalbuilding.tree", "resources/aidata/very_easy/generalbuilding.txt"),
      m_AirportBuildingTree("resources/aidata/very_easy/airportbuilding.tree", "resources/aidata/very_easy/airportbuilding.txt"),
      m_HarbourBuildingTree("resources/aidata/very_easy/harbourbuilding.tree", "resources/aidata/very_easy/harbourbuilding.txt")
{
    Interpreter::setCppOwnerShip(this);
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
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
    // and create a new one
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        if (getIslandIndex(pUnit) < 0)
        {
            createIslandMap(pUnit->getMovementType(), pUnit->getUnitID());
        }
        for (qint32 i2 = 0; i2 < pUnit->getLoadedUnitCount(); i2++)
        {
            if (getIslandIndex(pUnit->getLoadedUnit(i2)) < 0)
            {
                createIslandMap(pUnit->getLoadedUnit(i2)->getMovementType(), pUnit->getLoadedUnit(i2)->getUnitID());
            }
        }
    }

    // make the ai do stuff
    if (useCOPower(pUnits, pEnemyUnits)){}
    else if (useBuilding(pBuildings)){}
    else if (buildCOUnit(pUnits)){}
    else if (captureBuildings(pUnits)){}
    else if (CoreAI::moveOoziums(pUnits, pEnemyUnits)){}
    else if (fireWithIndirectUnits(pUnits)){}
    else if (fireWithDirectUnits(pUnits)){}
    else if (moveUnits(pUnits, pBuildings, pEnemyUnits, pEnemyBuildings)){}
    else if (loadUnits(pUnits)){}
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
    delete pBuildings;
    delete pUnits;

    delete pEnemyBuildings;
    delete pEnemyUnits;
}

bool VeryEasyAI::useCOPower(QmlVectorUnit* pUnits, QmlVectorUnit* pEnemyUnits)
{
    QVector<float> data;
    data.append(-1);
    data.append(0);
    data.append(-1);
    data.append(pUnits->size());
    qint32 repairUnits = 0;
    qint32 indirectUnits = 0;
    qint32 directUnits = 0;
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        if (pUnit->getHpRounded() < 10)
        {
            repairUnits++;
        }
        if (pUnit->getMaxRange() > 1)
        {
            indirectUnits++;
        }
        else
        {
            directUnits++;
        }
    }
    data.append(repairUnits);
    data.append(indirectUnits);
    data.append(directUnits);
    data.append(pEnemyUnits->size());
    data.append(m_pPlayer->getFonds());
    data.append(static_cast<float>(turnMode));
    CO* pCO = m_pPlayer->getCO(0);
    if (pCO != nullptr)
    {
        data[0] = COSpriteManager::getInstance()->getCOIndex(pCO->getCoID());
        if (pCO->canUseSuperpower())
        {
            data[1] = 2;
        }
        else if (pCO->canUsePower())
        {
            data[1] = 1;
        }
        else
        {
            data[1] = 0;
        }
        data[2] = pCO->getPowerFilled() - pCO->getPowerStars();
        float result = m_COPowerTree.getDecision(data);
        if (result == 1.0f)
        {
            GameAction* pAction = new GameAction(ACTION_ACTIVATE_POWER_CO_0);
            if (pAction->canBePerformed())
            {
                emit performAction(pAction);
                return true;
            }
        }
        else if (result == 2.0f)
        {
            GameAction* pAction = new GameAction(ACTION_ACTIVATE_SUPERPOWER_CO_0);
            if (pAction->canBePerformed())
            {
                emit performAction(pAction);
                return true;
            }
        }
    }
    pCO = m_pPlayer->getCO(1);
    if (pCO != nullptr)
    {
        data[0] = COSpriteManager::getInstance()->getCOIndex(pCO->getCoID());
        if (pCO->canUseSuperpower())
        {
            data[1] = 2;
        }
        else if (pCO->canUsePower())
        {
            data[1] = 1;
        }
        else
        {
            data[1] = 0;
        }
        data[2] = pCO->getPowerFilled() - pCO->getPowerStars();
        float result = m_COPowerTree.getDecision(data);
        if (result == 1.0f)
        {
            GameAction* pAction = new GameAction(ACTION_ACTIVATE_POWER_CO_1);
            if (pAction->canBePerformed())
            {
                emit performAction(pAction);
                return true;
            }
        }
        else if (result == 2.0f)
        {
            GameAction* pAction = new GameAction(ACTION_ACTIVATE_SUPERPOWER_CO_1);
            if (pAction->canBePerformed())
            {
                emit performAction(pAction);
                return true;
            }
        }
    }
    return false;
}

void VeryEasyAI::finishTurn()
{
    turnMode = TurnTime::startOfTurn;
    rebuildIslandMaps = true;
    GameAction* pAction = new GameAction(ACTION_NEXT_PLAYER);
    emit performAction(pAction);
}

bool VeryEasyAI::useBuilding(QmlVectorBuilding* pBuildings)
{
    turnMode = TurnTime::onGoingTurn;
    for (qint32 i = 0; i < pBuildings->size(); i++)
    {
        Building* pBuilding = pBuildings->at(i);
        QStringList actions = pBuilding->getActionList();
        if (actions.size() == 1 &&
            actions[0] != ACTION_BUILD_UNITS &&
            !actions[0].isEmpty())
        {
            GameAction* pAction = new GameAction(actions[0]);
            pAction->setTarget(QPoint(pBuilding->getX(), pBuilding->getY()));
            if (pAction->canBePerformed())
            {
                if (pAction->isFinalStep())
                {

                    emit performAction(pAction);
                    return true;
                }
                else
                {
                    if (pAction->getStepInputType() == "FIELD")
                    {
                        MarkedFieldData* pData = pAction->getMarkedFieldStepData();
                        QVector<QPoint>* points = pData->getPoints();
                        QPoint target = points->at(Mainapp::randInt(0, points->size() -1));
                        delete pData;
                        addSelectedFieldData(pAction, target);
                        if (pAction->isFinalStep())
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
                if (pCO != nullptr)
                {
                    data[0] = pCOSpriteManager->getCOIndex(pCO->getCoID());
                }
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
        if (!pUnit->getHasMoved() && pUnit->getMaxRange() > 1 &&
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
        QVector<QPoint> moveTargetFields;
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
            if (moveTargetFields[selection] != pAction->getTarget())
            {
                pAction->setMovepath(pfs.getPath(moveTargetFields[selection].x(), moveTargetFields[selection].y()));
            }
            else
            {
                pAction->setMovepath(QVector<QPoint>());
            }
            CoreAI::addSelectedFieldData(pAction, QPoint(target.x(), target.y()));
            // attacing none unit targets may modify the islands for a unit -> rebuild all for the love of god
            if (GameMap::getInstance()->getTerrain(target.x(), target.y())->getUnit() == nullptr)
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
            QVector<QPoint> targets;
            QVector<QPoint> transporterTargets;
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
            if (targets.size() == 0)
            {
                appendAttackTargetsIgnoreOwnUnits(pUnit, pEnemyUnits, targets);
            }
            if (targets.size() == 0)
            {
                appendRepairTargets(pUnit, pBuildings, targets);
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
                QVector<QPoint> targets;
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
                if (targets.size() == 0)
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
                QVector<QPoint> targets;
                QVector<QPoint> transporterTargets;
                appendLoadingTargets(pUnit, pUnits, pEnemyUnits, pEnemyBuildings, false, targets);
                if (targets.size() == 0)
                {
                    appendLoadingTargets(pUnit, pUnits, pEnemyUnits, pEnemyBuildings, true, targets);
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

bool VeryEasyAI::moveAwayFromProduction(QmlVectorUnit* pUnits)
{
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pUnit = pUnits->at(i);
        // can we use the unit and does it block a production center cause it has nothing to do this turn?
        if (!pUnit->getHasMoved() &&
            pUnit->getTerrain()->getBuilding() != nullptr &&
            !m_pPlayer->isEnemy(pUnit->getTerrain()->getBuilding()->getOwner()) &&
            pUnit->getTerrain()->getBuilding()->isProductionBuilding())
        {
            UnitPathFindingSystem turnPfs(pUnit);
            turnPfs.explore();
            QVector<QPoint> points = turnPfs.getAllNodePoints();
            QPoint target(-1 , -1);
            for (qint32 i = 0; i < points.size(); i++)
            {
                Terrain* pTerrain = pMap->getTerrain(points[i].x(), points[i].y());
                if (pTerrain->getUnit() == nullptr)
                {
                    if (pTerrain->getBuilding() == nullptr)
                    {
                        target = points[i];
                        break;
                    }
                    else if (!pTerrain->getBuilding()->isProductionBuilding())
                    {
                        target = points[i];
                        break;
                    }
                }
            }
            if (target.x() >= 0 && target.y() >= 0)
            {
                GameAction* pAction = new GameAction(ACTION_LOAD);
                pAction->setTarget(QPoint(pUnit->getX(), pUnit->getY()));
                pAction->setMovepath(turnPfs.getPath(target.x(), target.y()));
                pAction->setActionID(ACTION_WAIT);
                emit performAction(pAction);
                return true;
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
            QVector<QPoint> targets;
            QVector<QPoint> transporterTargets;
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
                          QVector<QPoint>& targets, QVector<QPoint>& transporterTargets, bool unload)
{
    TargetedUnitPathFindingSystem pfs(pUnit, targets);
    pfs.explore();
    qint32 movepoints = pUnit->getMovementpoints(QPoint(pUnit->getX(), pUnit->getY()));
    QPoint targetFields = pfs.getReachableTargetField(movepoints);
    if (targetFields.x() >= 0)
    {
        UnitPathFindingSystem turnPfs(pUnit);
        turnPfs.explore();

        if (transporterTargets.contains(targetFields))
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
    QVector<float> data;
    qint32 productionBuildings = 0;
    for (qint32 i = 0; i < pBuildings->size(); i++)
    {
        Building* pBuilding = pBuildings->at(i);
        if (pBuilding->isProductionBuilding())
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
        if (pUnit->getMaxRange() > 1)
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
    data.append(m_pPlayer->getFonds());
    data.append(m_pPlayer->getFonds() / static_cast<float>(productionBuildings));
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
        if (i2 == 0 || m_pPlayer->getFonds() >= 8000)
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

void VeryEasyAI::appendLoadingTargets(Unit* pUnit, QmlVectorUnit* pUnits, QmlVectorUnit* pEnemyUnits, QmlVectorBuilding* pEnemyBuildings, bool ignoreCaptureTargets, QVector<QPoint>& targets)
{
    qint32 unitIslandIdx = getIslandIndex(pUnit);
    qint32 unitIsland = getIsland(pUnit);
    GameMap* pMap = GameMap::getInstance();
    qint32 width = pMap->getMapWidth();
    qint32 heigth = pMap->getMapHeight();
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pLoadingUnit = pUnits->at(i);
        if (pLoadingUnit != pUnit)
        {
            // can we transport it?
            if (pUnit->canTransportUnit(pLoadingUnit))
            {
                bool found = false;
                bool canCapture = pLoadingUnit->getActionList().contains(ACTION_CAPTURE);
                if (ignoreCaptureTargets && canCapture)
                {
                    // no targets found -> try to speed up those infis
                }
                else
                {
                    // check if we have anything to do here :)
                    for (qint32 i2 = 0; i2 < pEnemyUnits->size(); i2++)
                    {
                        Unit* pEnemy = pEnemyUnits->at(i2);
                        if (onSameIsland(pLoadingUnit, pEnemy) &&
                            pLoadingUnit->isAttackable(pEnemy, true))
                        {
                            // this unit can do stuff skip it
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                    {
                        // check for capturing or missiles next
                        if (canCapture)
                        {
                            for (qint32 i2 = 0; i2 < pEnemyBuildings->size(); i2++)
                            {
                                Building* pBuilding = pEnemyBuildings->at(i2);
                                if (onSameIsland(pLoadingUnit, pBuilding) &&
                                    pBuilding->isCaptureOrMissileBuilding())
                                {
                                    // this unit can do stuff skip it
                                    found = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                if (!found)
                {
                    // no targets for this unit :(
                    found = false;
                    qint32 loadingIslandIdx = getIslandIndex(pLoadingUnit);
                    qint32 loadingIsland = getIsland(pLoadingUnit);
                    qint32 loadingUnitX = pLoadingUnit->getX();
                    qint32 loadingUnitY = pLoadingUnit->getY();
                    qint32 radius = loadingUnitX;
                    // find nearest loading field of this unit
                    // by searching in circles around it first detect the maximum radius required for this map
                    if (width - loadingUnitX > radius)
                    {
                        radius = width - loadingUnitX;
                    }
                    if (loadingUnitY > radius)
                    {
                        radius = loadingUnitY;
                    }
                    if (heigth - loadingUnitY > radius)
                    {
                        radius = heigth - loadingUnitY;
                    }
                    // find closest field to the loading unit we can reach and add it
                    for (qint32 r = 0; r < radius; r++)
                    {
                        // check fields in a radius around it
                        QmlVectorPoint* fields = Mainapp::getCircle(r, r);
                        for (qint32 i3 = 0; i3 < fields->size(); i3++)
                        {
                            qint32 x = loadingUnitX + fields->at(i3).x();
                            qint32 y = loadingUnitY + fields->at(i3).y();
                            // is it a map point?
                            if (pMap->onMap(x, y))
                            {
                                // can be reached by both units and is empty
                                // and not added yet
                                if ((m_IslandMaps[loadingIslandIdx]->getIsland(x, y) == loadingIsland) &&
                                    (m_IslandMaps[unitIslandIdx]->getIsland(x, y) == unitIsland) &&
                                    (pMap->getTerrain(x, y)->getUnit() == nullptr) &&
                                    (!targets.contains(QPoint(x, y))))
                                {
                                    // append it and and skip further searching
                                    found = true;
                                    targets.append(QPoint(x, y));
                                }
                            }
                        }
                        delete fields;
                        // a loading field for this unit was
                        if (found)
                        {
                            break;
                        }
                    }
                }
            }
        }
    }
}

void VeryEasyAI::appendCaptureTargets(QStringList actions, Unit* pUnit, QmlVectorBuilding* pEnemyBuildings, QVector<QPoint>& targets)
{
    if (actions.contains(ACTION_CAPTURE) ||
        actions.contains(ACTION_MISSILE))
    {
        for (qint32 i2 = 0; i2 < pEnemyBuildings->size(); i2++)
        {
            Building* pBuilding = pEnemyBuildings->at(i2);
            QPoint point(pBuilding->getX(), pBuilding->getY());
            if (pUnit->canMoveOver(pBuilding->getX(), pBuilding->getY()))
            {
                if (pBuilding->isCaptureOrMissileBuilding() &&
                    pBuilding->getTerrain()->getUnit() == nullptr)
                {
                    targets.append(QPoint(pBuilding->getX(), pBuilding->getY()));
                }
            }
        }
    }
}



void VeryEasyAI::appendAttackTargetsIgnoreOwnUnits(Unit* pUnit, QmlVectorUnit* pEnemyUnits, QVector<QPoint>& targets)
{
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i2 = 0; i2 < pEnemyUnits->size(); i2++)
    {
        Unit* pEnemy = pEnemyUnits->at(i2);
        if (pUnit->isAttackable(pEnemy, true))
        {
            qint32 firerange = pUnit->getMaxRange();
            QmlVectorPoint* pTargetFields = Mainapp::getCircle(firerange, firerange);
            for (qint32 i3 = 0; i3 < pTargetFields->size(); i3++)
            {
                qint32 x = pTargetFields->at(i3).x() + pEnemy->getX();
                qint32 y = pTargetFields->at(i3).y() + pEnemy->getY();
                if (pMap->onMap(x, y))
                {
                    Unit* pTargetUnit = pMap->getTerrain(x, y)->getUnit();
                    if (pUnit->canMoveOver(x, y) &&
                        pTargetUnit != nullptr &&
                        pTargetUnit->getOwner()->checkAlliance(m_pPlayer) == GameEnums::Alliance_Friend)
                    {
                        QPoint possibleTarget(x, y);
                        if (!targets.contains(possibleTarget))
                        {
                            targets.append(possibleTarget);
                        }
                    }
                }
            }
            delete pTargetFields;
        }
    }
}

void VeryEasyAI::appendRepairTargets(Unit* pUnit, QmlVectorBuilding* pBuildings, QVector<QPoint>& targets)
{
    GameMap* pMap = GameMap::getInstance();
    if ((pUnit->getMaxAmmo1() > 0 && !pUnit->hasAmmo1()) ||
        (pUnit->getMaxAmmo2() > 0 && !pUnit->hasAmmo2()) ||
        (pUnit->getMaxFuel() > 0 && static_cast<float>(pUnit->getFuel()) / static_cast<float>(pUnit->getMaxFuel()) < 1.0f / 3.0f))
    {
        for (qint32 i2 = 0; i2 < pBuildings->size(); i2++)
        {
            Building* pBuilding = pBuildings->at(i2);
            QPoint point(pBuilding->getX(), pBuilding->getY());
            if (pMap->getTerrain(point.x(), point.y())->getUnit() == nullptr &&
                pBuilding->canRepair(pUnit))
            {
                targets.append(QPoint(pBuilding->getX(), pBuilding->getY()));
            }
        }
    }
}

void VeryEasyAI::appendSupplyTargets(Unit* pUnit, QmlVectorUnit* pUnits, QVector<QPoint>& targets)
{
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pSupplyUnit = pUnits->at(i);
        if (pSupplyUnit != pUnit)
        {
            if ((pSupplyUnit->getMaxAmmo1() > 0 && pSupplyUnit->hasAmmo1() < pSupplyUnit->getMaxAmmo1()) ||
                (pSupplyUnit->getMaxAmmo2() > 0 && pSupplyUnit->hasAmmo2() < pSupplyUnit->getMaxAmmo2()) ||
                (pSupplyUnit->getMaxFuel() > 0 && static_cast<float>(pSupplyUnit->getFuel()) / static_cast<float>(pSupplyUnit->getMaxFuel()) < 0.5f))
            {
                targets.append(QPoint(pSupplyUnit->getX(), pSupplyUnit->getY()));
            }
        }
    }
}

void VeryEasyAI::appendTransporterTargets(Unit* pUnit, QmlVectorUnit* pUnits, QVector<QPoint>& targets)
{
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pTransporterUnit = pUnits->at(i);
        if (pTransporterUnit != pUnit)
        {
            if (pTransporterUnit->canTransportUnit(pUnit))
            {
                targets.append(QPoint(pTransporterUnit->getX(), pTransporterUnit->getY()));
            }
        }
    }
}

void VeryEasyAI::appendCaptureTransporterTargets(Unit* pUnit, QmlVectorUnit* pUnits, QmlVectorBuilding* pEnemyBuildings, QVector<QPoint>& targets)
{
    qint32 unitIslandIdx = getIslandIndex(pUnit);
    qint32 unitIsland = getIsland(pUnit);
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        Unit* pTransporterUnit = pUnits->at(i);
        if (pTransporterUnit != pUnit)
        {
            // assuming unit transporter only have space for one unit
            if (pTransporterUnit->canTransportUnit(pUnit) &&
                pTransporterUnit->getLoadingPlace() == 1)
            {
                bool goodTransporter = false;
                // check captures on this island
                qint32 transporterIslandIdx = getIslandIndex(pTransporterUnit);
                qint32 transporterIsland = getIsland(pTransporterUnit);
                for (qint32 i2 = 0; i2 < pEnemyBuildings->size(); i2++)
                {
                    qint32 x = pEnemyBuildings->at(i2)->getX();
                    qint32 y = pEnemyBuildings->at(i2)->getY();
                    // check if both can move there on this island
                    // so we avoid loading and unloading a unit recursivly cause we think we need to transport it to another island
                    // eventhough it has something to do here
                    if ((m_IslandMaps[unitIslandIdx]->getIsland(x, y) == unitIsland) &&
                        (m_IslandMaps[transporterIslandIdx]->getIsland(x, y) == transporterIsland) &&
                        (pMap->getTerrain(x, y)->getUnit() == nullptr) &&
                        (pEnemyBuildings->at(i2)->isCaptureOrMissileBuilding()))
                    {
                        goodTransporter = true;
                        break;
                    }
                }
                if (goodTransporter)
                {

                    targets.append(QPoint(pTransporterUnit->getX(), pTransporterUnit->getY()));
                }
            }
        }
    }
}

void VeryEasyAI::appendNearestUnloadTargets(Unit* pUnit, QmlVectorUnit* pEnemyUnits, QmlVectorBuilding* pEnemyBuildings, QVector<QPoint>& targets)
{
    QVector<QVector<qint32>> checkedIslands;
    QVector<qint32> loadedUnitIslandIdx;

    qint32 unitIslandIdx = getIslandIndex(pUnit);
    qint32 unitIsland = getIsland(pUnit);
    QVector<Unit*> captureUnits;
    for (qint32 i2 = 0; i2 < pUnit->getLoadedUnitCount(); i2++)
    {
        Unit* pLoadedUnit = pUnit->getLoadedUnit(i2);
        if (pLoadedUnit->getActionList().contains(ACTION_CAPTURE))
        {
            captureUnits.append(pLoadedUnit);
        }
        loadedUnitIslandIdx.append(getIslandIndex(pLoadedUnit));
        checkedIslands.append(QVector<qint32>());
    }
    QmlVectorPoint* pUnloadArea = Mainapp::getCircle(1, 1);
    // check for enemis
    for (qint32 i = 0; i < pEnemyUnits->size(); i++)
    {
        Unit* pEnemy = pEnemyUnits->at(i);
        for (qint32 i2 = 0; i2 < pUnit->getLoadedUnitCount(); i2++)
        {
            Unit* pLoadedUnit = pUnit->getLoadedUnit(i2);
            qint32 targetIsland = m_IslandMaps[loadedUnitIslandIdx[i2]]->getIsland(pEnemy->getX(), pEnemy->getY());
            // check if we could reach the enemy if we would be on his island
            // and we didn't checked this island yet -> improves the speed
            if (targetIsland >= 0 && !checkedIslands[i2].contains(targetIsland))
            {
                // could we beat his ass? -> i mean can we attack him
                // if so this is a great island
                if (pLoadedUnit->isAttackable(pEnemy, true))
                {
                    checkIslandForUnloading(pLoadedUnit, checkedIslands[i2], unitIslandIdx, unitIsland,
                                            loadedUnitIslandIdx[i2], targetIsland, pUnloadArea, targets);
                }
            }
        }
    }
    // check for capturable buildings
    if (captureUnits.size() > 0)
    {
        for (qint32 i = 0; i < pEnemyBuildings->size(); i++)
        {
            Building* pEnemyBuilding = pEnemyBuildings->at(i);
            for (qint32 i2 = 0; i2 < captureUnits.size(); i2++)
            {
                Unit* pLoadedUnit = captureUnits[i2];
                qint32 targetIsland = m_IslandMaps[loadedUnitIslandIdx[i2]]->getIsland(pEnemyBuilding->getX(), pEnemyBuilding->getY());
                // check if we could reach the enemy if we would be on his island
                // and we didn't checked this island yet -> improves the speed
                if (targetIsland >= 0 && !checkedIslands[i2].contains(targetIsland))
                {
                    if (pEnemyBuilding->isCaptureOrMissileBuilding())
                    {
                        checkIslandForUnloading(pLoadedUnit, checkedIslands[i2], unitIslandIdx, unitIsland,
                                                loadedUnitIslandIdx[i2], targetIsland, pUnloadArea, targets);
                    }
                }
            }
        }
    }
    delete pUnloadArea;
}

void VeryEasyAI::checkIslandForUnloading(Unit* pLoadedUnit, QVector<qint32>& checkedIslands,
                                         qint32 unitIslandIdx, qint32 unitIsland,
                                         qint32 loadedUnitIslandIdx, qint32 targetIsland,
                                         QmlVectorPoint* pUnloadArea, QVector<QPoint>& targets)
{
    GameMap* pMap = GameMap::getInstance();
    qint32 width = pMap->getMapWidth();
    qint32 heigth = pMap->getMapHeight();
    checkedIslands.append(loadedUnitIslandIdx);
    for (qint32 x = 0; x < width; x++)
    {
        for (qint32 y = 0; y < heigth; y++)
        {
            // check if this is the same island as we search for
            // check if it's the same island our transporter is on if so we can reach the field
            // the unloading area is also free
            if (m_IslandMaps[loadedUnitIslandIdx]->getIsland(x, y) == targetIsland &&
                m_IslandMaps[unitIslandIdx]->getIsland(x, y) == unitIsland &&
                pMap->getTerrain(x, y)->getUnit() == nullptr)
            {
                // and on top of that we have same free fields to unload the unit
                for (qint32 i3 = 0; i3 < pUnloadArea->size(); i3++)
                {
                    qint32 unloadX = x + pUnloadArea->at(i3).x();
                    qint32 unloadY = y + pUnloadArea->at(i3).y();
                    if (pMap->onMap(unloadX, unloadY) &&
                        pMap->getTerrain(unloadX, unloadY)->getUnit() == nullptr &&
                        pLoadedUnit->getBaseMovementCosts(unloadX, unloadY) > 0 &&
                        !targets.contains(QPoint(x, y)))
                    {
                        targets.append(QPoint(x, y));
                        break;
                    }
                }
            }
        }
    }
}

void VeryEasyAI::appendUnloadTargetsForCapturing(Unit* pUnit, QmlVectorBuilding* pEnemyBuildings, QVector<QPoint>& targets)
{
    GameMap* pMap = GameMap::getInstance();

    qint32 unitIslandIdx = getIslandIndex(pUnit);
    qint32 unitIsland = getIsland(pUnit);

    QVector<Unit*> capturUnits;
    for (qint32 i2 = 0; i2 < pUnit->getLoadedUnitCount(); i2++)
    {
        Unit* pLoadedUnit = pUnit->getLoadedUnit(i2);
        if (pLoadedUnit->getActionList().contains(ACTION_CAPTURE))
        {
            capturUnits.append(pLoadedUnit);
        }
    }
    if (capturUnits.size() > 0)
    {
        QmlVectorPoint* pUnloadArea = Mainapp::getCircle(1, 1);
        GameAction testAction;
        testAction.setTargetUnit(capturUnits[0]);
        // store has moved
        bool hasMoved = capturUnits[0]->getHasMoved();
        // simulate a not moved unit for checking if we can capture the building or fire a missile from it.
        capturUnits[0]->setHasMoved(false);
        for (qint32 i = 0; i < pEnemyBuildings->size(); i++)
        {
            Building* pBuilding = pEnemyBuildings->at(i);
            QPoint point(pBuilding->getX(), pBuilding->getY());
            if (capturUnits[0]->canMoveOver(pBuilding->getX(), pBuilding->getY()))
            {
                // we can capture it :)
                if (pBuilding->isCaptureOrMissileBuilding() &&
                    pBuilding->getTerrain()->getUnit() == nullptr)
                {
                    // check unload fields
                    for (qint32 i2 = 0; i2 < pUnloadArea->size(); i2++)
                    {
                        qint32 x = point.x() + pUnloadArea->at(i2).x();
                        qint32 y = point.y() + pUnloadArea->at(i2).y();
                        if (!targets.contains(QPoint(x, y)) &&
                            pMap->onMap(x, y) &&
                            pMap->getTerrain(x, y)->getUnit() == nullptr)
                        {
                            // we can reach this unload field?
                            if (m_IslandMaps[unitIslandIdx]->getIsland(x, y) == unitIsland)
                            {
                                for (qint32 i3 = 0; i3 < capturUnits.size(); i3++)
                                {
                                    if (capturUnits[i3]->canMoveOver(x, y))
                                    {
                                        targets.append(QPoint(x, y));
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        capturUnits[0]->setHasMoved(hasMoved);
        delete pUnloadArea;
    }
}

void VeryEasyAI::createIslandMap(QString movementType, QString unitID)
{
    bool found = false;

    for (qint32 i = 0; i < m_IslandMaps.size(); i++)
    {
        if (m_IslandMaps[i]->getMovementType() == movementType)
        {
            found = true;
            break;
        }
    }
    if (!found)
    {
        m_IslandMaps.append(new IslandMap(unitID, m_pPlayer));
    }
}

bool VeryEasyAI::onSameIsland(Unit* pUnit1, Unit* pUnit2)
{
    for (auto i = 0; i < m_IslandMaps.size(); i++)
    {
        if (m_IslandMaps[i]->getMovementType() == pUnit1->getMovementType())
        {
            if (m_IslandMaps[i]->getIsland(pUnit1->getX(), pUnit1->getY()) ==
                m_IslandMaps[i]->getIsland(pUnit2->getX(), pUnit2->getY()))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    return false;
}

bool VeryEasyAI::onSameIsland(Unit* pUnit1, Building* pBuilding)
{
    for (auto i = 0; i < m_IslandMaps.size(); i++)
    {
        if (m_IslandMaps[i]->getMovementType() == pUnit1->getMovementType())
        {
            if (m_IslandMaps[i]->getIsland(pUnit1->getX(), pUnit1->getY()) ==
                m_IslandMaps[i]->getIsland(pBuilding->getX(), pBuilding->getY()))
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    return false;
}

qint32 VeryEasyAI::getIsland(Unit* pUnit)
{
    for (qint32 i = 0; i < m_IslandMaps.size(); i++)
    {
        if (m_IslandMaps[i]->getMovementType() == pUnit->getMovementType())
        {
            return m_IslandMaps[i]->getIsland(pUnit->getX(), pUnit->getY());
        }
    }
    return -1;
}

qint32 VeryEasyAI::getIslandIndex(Unit* pUnit)
{
    for (qint32 i = 0; i < m_IslandMaps.size(); i++)
    {
        if (m_IslandMaps[i]->getMovementType() == pUnit->getMovementType())
        {
            return i;
        }
    }
    return -1;
}


void VeryEasyAI::serializeObject(QDataStream&)
{
}
void VeryEasyAI::deserializeObject(QDataStream&)
{
}

