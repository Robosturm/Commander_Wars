#include <QStringList>
#include <QJSValueList>

#include "coreengine/interpreter.h"
#include "coreengine/gameconsole.h"

#include "ai/capturebuildingselector.h"

#include "game/gameaction.h"
#include "game/gamemap.h"

CaptureBuildingSelector::CaptureBuildingSelector(CoreAI & owner)
    : m_owner(owner)
{

}

spGameAction CaptureBuildingSelector::getNextCaptureBuilding(std::vector<CoreAI::MoveUnitData> & rOwnUnits, CoreAI::MoveUnitData** targetUnitData)
{
    AI_CONSOLE_PRINT("CaptureBuildingSelector::getNextCaptureBuilding()", GameConsole::eDEBUG);

    spGameAction pAction;
    QStringList highPrioBuildings;
    Interpreter *pInterpreter = Interpreter::getInstance();
    QJSValueList args({JsThis::getJsThis(&m_owner)});
    const QString func = "getHighPrioBuildings";
    auto erg = pInterpreter->doFunction(m_owner.getAiName(), func, args);
    highPrioBuildings = erg.toVariant().toStringList();
    TargetBuildings captureBuildings = getTargetBuildings(rOwnUnits, highPrioBuildings, pAction, targetUnitData);

    if (captureBuildings.size() > 0)
    {
        Interpreter *pInterpreter = Interpreter::getInstance();
        for (qint32 i = 0; i < rOwnUnits.size(); ++i)
        {
            auto &unitData = rOwnUnits[i];
            if (unitData.nextAiStep <= m_owner.getAiFunctionStep())
            {
                pInterpreter->threadProcessEvents();
                Unit *pUnit = unitData.pUnit.get();
                if (!pUnit->getHasMoved() &&
                    unitData.actions.contains(CoreAI::ACTION_CAPTURE) &&
                    pUnit->getAiMode() == GameEnums::GameAi_Normal)
                {
                    TargetBuildings captures = filterCaptures(captureBuildings, i);
                    qint32 targetIndex = 0;
                    bool perform = getNextTarget(captureBuildings, captures, targetIndex);
                    if (perform)
                    {
                        pAction = getPerformingAction(captures, pUnit, unitData, targetIndex);
                        *targetUnitData = &unitData;
                        break;
                    }
                }
            }
        }
    }
    return pAction;
}

CaptureBuildingSelector::TargetBuildings CaptureBuildingSelector::filterCaptures(TargetBuildings & captureBuildings, qint32 unitIdx)
{
    TargetBuildings captures;
    for (auto &building : captureBuildings)
    {
        if (building.m_unitIdx == unitIdx)
        {
            captures.push_back(building);
        }
    }
    return captures;
}

bool CaptureBuildingSelector::getNextTarget(TargetBuildings & captureBuildings, TargetBuildings & captures, qint32 & targetIndex)
{
    bool perform = false;
    if (captures.size() > 0)
    {
        if (captures.size() == 1)
        {
            // we have only one target go for it
            targetIndex = 0;
            perform = true;
        }
        else
        {
            perform = findSingleCaptureBuilding(captureBuildings, captures, targetIndex);
            // if not we can select a target from the list
            if (!perform &&
                captures.size() > 0)
            {
                qint32 prio = -1;
                targetIndex = 0;
                // priorities production buildings over over captures
                for (qint32 i2 = 0; i2 < captures.size(); i2++)
                {
                    qint32 testPrio = getPrio(captures[i2]);
                    if (testPrio > prio)
                    {
                        targetIndex = i2;
                        prio = testPrio;
                    }
                }
                perform = true;
            }
        }
    }
    return perform;
}

bool CaptureBuildingSelector::findSingleCaptureBuilding(TargetBuildings & captureBuildings, TargetBuildings & captures, qint32 & targetIndex)
{
    bool perform = false;
    qint32 prio = -1;
    // check if we have a building only we can capture and capture it
    qint32 i2 = 0;
    while (i2 < captures.size())
    {
        qint32 captureCount = 0;
        for (auto &buildingPos2 : captureBuildings)
        {
            if (buildingPos2.m_x == captures[i2].m_x &&
                buildingPos2.m_y == captures[i2].m_y)
            {
                if (buildingPos2.m_farAway == captures[i2].m_farAway ||
                    (buildingPos2.m_farAway == true && captures[i2].m_farAway == false))
                {
                    ++captureCount;
                }
                else
                {
                    --captureCount;
                }
            }
        }
        if (captureCount <= 0)
        {
            captures.erase(captures.cbegin() + i2);
        }
        else
        {
            bool isProductionBuilding = m_owner.getMap()->getTerrain(captures[i2].m_x, captures[i2].m_y)->getBuilding()->isProductionBuilding();
            if ((captureCount == 1 && perform == false) ||
                (captureCount == 1 && perform == true && isProductionBuilding))
            {
                qint32 testPrio = getPrio(captures[i2]);
                if (!perform)
                {
                    prio = testPrio;
                    targetIndex = i2;
                }
                else
                {
                    if (testPrio > prio)
                    {
                        targetIndex = i2;
                        prio = testPrio;
                    }
                }
                perform = true;
            }
            ++i2;
        }
    }
    return perform;
}

qint32 CaptureBuildingSelector::getPrio(CaptureInfo & info)
{
    constexpr qint32 nearbyPrioBonus = 2048;
    Building *pBuilding = m_owner.getMap()->getTerrain(info.m_x, info.m_y)->getBuilding();
    qint32 testPrio = std::numeric_limits<qint32>::min();
    if (pBuilding->isHq())
    {
        testPrio = std::numeric_limits<qint32>::max();
    }
    else if (pBuilding->isProductionBuilding())
    {
        testPrio = pBuilding->getConstructionList().size();
    }
    if (!info.m_farAway && testPrio > 0)
    {
        testPrio += nearbyPrioBonus;
    }
    return testPrio;
}

spGameAction CaptureBuildingSelector::getPerformingAction(TargetBuildings & captures, Unit *pUnit, CoreAI::MoveUnitData &unitData, qint32 targetIndex)
{
    spGameAction pAction;
    // perform capturing
    if (captures[targetIndex].m_farAway)
    {
        pAction = MemoryManagement::create<GameAction>(CoreAI::ACTION_WAIT, m_owner.getMap());
        pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
        QVector<QPoint> movePath({QPoint(captures[targetIndex].m_x, captures[targetIndex].m_y)});
        pAction->setMovepath(movePath, 0);
    }
    else
    {
        pAction = MemoryManagement::create<GameAction>(CoreAI::ACTION_CAPTURE, m_owner.getMap());
        pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
        auto path = unitData.pUnitPfs->getPathFast(captures[targetIndex].m_x, captures[targetIndex].m_y);
        pAction->setMovepath(path, unitData.pUnitPfs->getCosts(path));
        if (pAction->canBePerformed())
        {
        }
        else
        {
            qint32 cost = 0;
            QPoint rocketTarget = m_owner.getPlayer()->getSiloRockettarget(2, 3, cost);
            m_owner.addSelectedFieldData(pAction, rocketTarget);
            pAction->setActionID(CoreAI::ACTION_MISSILE);
            if (pAction->canBePerformed())
            {
            }
            else
            {
                pAction = nullptr;
            }
        }
    }
    return pAction;
}

CaptureBuildingSelector::TargetBuildings CaptureBuildingSelector::getTargetBuildings(std::vector<CoreAI::MoveUnitData> & rOwnUnits, QStringList & highPrioBuildings, spGameAction & pAction, CoreAI::MoveUnitData** targetUnitData)
{
    Interpreter *pInterpreter = Interpreter::getInstance();
    bool fireSilos = m_owner.hasMissileTarget();
    TargetBuildings captureBuildings;
    for (qint32 i = 0; i < rOwnUnits.size(); ++i)
    {
        pInterpreter->threadProcessEvents();
        auto &unitData = rOwnUnits[i];
        if (unitData.nextAiStep <= m_owner.getAiFunctionStep())
        {
            Unit *pUnit = unitData.pUnit.get();
            if (!pUnit->getHasMoved() &&
                unitData.actions.contains(CoreAI::ACTION_CAPTURE) &&
                pUnit->getAiMode() == GameEnums::GameAi_Normal)
            {
                if (pUnit->getCapturePoints() > 0)
                {
                    pAction = MemoryManagement::create<GameAction>(CoreAI::ACTION_CAPTURE, m_owner.getMap());
                    pAction->setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                    if (pAction->canBePerformed())
                    {
                        *targetUnitData = &unitData;
                        break;
                    }
                    else
                    {
                        pAction = nullptr;
                    }
                }
                GameAction action(CoreAI::ACTION_CAPTURE, m_owner.getMap());
                action.setTarget(QPoint(pUnit->Unit::getX(), pUnit->Unit::getY()));
                auto targets = unitData.pUnitPfs->getAllNodePointsFast();
                for (auto &target : targets)
                {
                    action.setActionID(CoreAI::ACTION_CAPTURE);
                    action.setMovepath(QVector<QPoint>(1, target), 0);
                    if (unitData.pUnitPfs->getTargetCosts(target.x(), target.y()) < unitData.movementPoints + 1)
                    {
                        if (action.canBePerformed())
                        {
                            captureBuildings.push_back(CaptureInfo(target.x(), target.y(), i, false));
                        }
                        else
                        {
                            action.setActionID(CoreAI::ACTION_MISSILE);
                            if (action.canBePerformed() && fireSilos)
                            {
                                captureBuildings.push_back(CaptureInfo(target.x(), target.y(), i, false));
                            }
                        }
                    }
                    else
                    {
                        Terrain *pTerrain = m_owner.getMap()->getTerrain(target.x(), target.y());
                        auto *pBuilding = pTerrain->getBuilding();
                        if (pBuilding != nullptr &&
                            pBuilding->getOwner() == nullptr &&
                            !m_usedFarAwayBuildings.contains(target) &&
                            highPrioBuildings.contains(pBuilding->getBuildingID()) &&
                            pTerrain->getUnit() == nullptr)
                        {
                            if (action.canBePerformed())
                            {
                                captureBuildings.push_back(CaptureInfo(target.x(), target.y(), i, true));
                            }
                        }
                    }
                }
            }
        }
    }
    return captureBuildings;
}

void CaptureBuildingSelector::addUsedFarAwayBuildings(QPoint farAwayTarget)
{
    m_usedFarAwayBuildings.push_back(farAwayTarget);
}

void CaptureBuildingSelector::resetUsedFarAwayBuildings()
{
    m_usedFarAwayBuildings.clear();
}
