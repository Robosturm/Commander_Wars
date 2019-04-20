#include "veryeasyai.h"

#include "game/gameaction.h"

#include "game/player.h"

#include "game/co.h"

#include "game/unit.h"

#include "game/gamemap.h"

#include "game/unitpathfindingsystem.h"
#include "resource_management/cospritemanager.h"
#include "resource_management/unitspritemanager.h"

VeryEasyAI::VeryEasyAI()
    : m_COPowerTree("resources/aidata/very_easy/copower.tree", "resources/aidata/very_easy/copower.txt"),
      m_COUnitTree("resources/aidata/very_easy/counit.tree", "resources/aidata/very_easy/counit.txt"),
      m_GeneralBuildingTree("resources/aidata/very_easy/generalbuilding.tree", "resources/aidata/very_easy/generalbuilding.txt"),
      m_AirportBuildingTree("resources/aidata/very_easy/airportbuilding.tree", "resources/aidata/very_easy/airportbuilding.txt"),
      m_HarbourBuildingTree("resources/aidata/very_easy/harbourbuilding.tree", "resources/aidata/very_easy/harbourbuilding.txt")
{
    Interpreter::setCppOwnerShip(this);
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());


    m_GeneralBuildingTree.printTree();
}

void VeryEasyAI::process()
{
    QmlVectorBuilding* pBuildings = m_pPlayer->getBuildings();
    pBuildings->randomize();
    QmlVectorUnit* pUnits = m_pPlayer->getUnits();
    QmlVectorUnit* pEnemyUnits = m_pPlayer->getEnemyUnits();
    pUnits->randomize();
    // make the ai to stuff
    if (useCOPower(pUnits, pEnemyUnits)){}
    else if (useBuilding(pBuildings)){}
    else if (captureBuildings(pUnits)){}
    else if (fireWithIndirectUnits(pUnits)){}
    else if (fireWithDirectUnits(pUnits)){}
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
    delete pEnemyUnits;
    delete pUnits;
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
                    // todo select
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
                        pAction->setMovepath(pfs.getPath(targets[i2].x(), targets[i2].y()));
                        if (pAction->canBePerformed())
                        {
                            emit performAction(pAction);
                            return true;
                        }
                    }
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
        if (ret.size() > 0)
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
            if (pAction->isFinalStep())
            {
                emit performAction(pAction);
                return true;
            }
            else
            {
                delete pAction;
            }
        }
        else
        {
            delete pAction;
        }
    }
    return false;
}

bool VeryEasyAI::buildUnits(QmlVectorBuilding* pBuildings, QmlVectorUnit* pUnits)
{
    QVector<float> data;
    qint32 productionBuildings = 0;
    for (qint32 i = 0; i < pBuildings->size(); i++)
    {
        Building* pBuilding = pBuildings->at(i);
        QStringList actions = pBuilding->getActionList();
        if (actions.contains(ACTION_BUILD_UNITS))
        {
            productionBuildings++;
        }
    }
    qint32 infantryUnits = 0;
    qint32 indirectUnits = 0;
    qint32 directUnits = 0;
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
    }
    data.append(m_pPlayer->getFonds());
    data.append(m_pPlayer->getFonds() / static_cast<float>(productionBuildings));
    data.append(pUnits->size());
    data.append(directUnits / static_cast<float>(indirectUnits));
    data.append(infantryUnits);

    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    for (qint32 i = 0; i < pBuildings->size(); i++)
    {
        Building* pBuilding = pBuildings->at(i);
        QStringList actions = pBuilding->getActionList();
        if (actions.contains(ACTION_BUILD_UNITS))
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
            else
            {
                delete pAction;
            }
        }
    }
    return false;
}

void VeryEasyAI::serializeObject(QDataStream&)
{
}
void VeryEasyAI::deserializeObject(QDataStream&)
{
}

