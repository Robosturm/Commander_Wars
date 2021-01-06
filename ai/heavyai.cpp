#include "heavyai.h"

#include "coreengine/qmlvector.h"
#include "coreengine/console.h"
#include "coreengine/globalutils.h"
#include "ai/targetedunitpathfindingsystem.h"
#include "game/player.h"

#include "resource_management/unitspritemanager.h"

const qint32 HeavyAi::minSiloDamage = 7000;

HeavyAi::HeavyAi()
    : CoreAI(GameEnums::AiTypes_Heavy),
      m_InfluenceFrontMap(m_IslandMaps)
{
    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, &HeavyAi::process, Qt::QueuedConnection);
}

void HeavyAi::readIni(QString name)
{

}

void HeavyAi::toggleAiPause()
{
    m_pause = !m_pause;
}

void HeavyAi::showFrontMap()
{
    m_InfluenceFrontMap.show();
}

void HeavyAi::showFrontLines()
{
    m_InfluenceFrontMap.showFrontlines();
}

void HeavyAi::hideFrontMap()
{
    m_InfluenceFrontMap.hide();
}

void HeavyAi::process()
{
    spQmlVectorBuilding pBuildings = m_pPlayer->getBuildings();
    pBuildings->randomize();
    setupTurn(pBuildings);
    if (m_pause)
    {
        m_timer.start(1000);
        return;
    }
    if (useBuilding(pBuildings.get())){}
    else
    {
        turnMode = GameEnums::AiTurnMode_EndOfDay;
        if (useCOPower(m_pUnits.get(), m_pEnemyUnits.get()))
        {
            turnMode = GameEnums::AiTurnMode_DuringDay;
            usedTransportSystem = false;
        }
        else
        {
            // end the turn for the player
            turnMode = GameEnums::AiTurnMode_StartOfDay;
            m_pUnits = nullptr;
            m_pEnemyUnits = nullptr;
            finishTurn();
        }
    }
}

void HeavyAi::setupTurn(const spQmlVectorBuilding & buildings)
{
    bool startOfTurn = (m_pUnits.get() == nullptr);
    if (m_pUnits.get() == nullptr)
    {
        m_pUnits = m_pPlayer->getUnits();
        m_pUnits->sortShortestMovementRange(true);
        initUnits(m_pUnits.get(), m_ownUnits, false);
    }
    if (m_pEnemyUnits.get() == nullptr)
    {
        m_pEnemyUnits = m_pPlayer->getEnemyUnits();
        m_pEnemyUnits->randomize();
        initUnits(m_pEnemyUnits.get(), m_enemyUnits, true);
    }
    if (startOfTurn)
    {
        Console::print("HeavyAi initial start of turn calculation", Console::eDEBUG);
        createIslandMaps();

        // create influence map at the start of the turn
        m_InfluenceFrontMap.reset();
        m_InfluenceFrontMap.addBuildingInfluence();
        for (auto & unit : m_ownUnits)
        {
            m_InfluenceFrontMap.addUnitInfluence(unit.m_pUnit, unit.m_pPfs.get(), unit.m_movepoints);
        }
        for (auto & unit : m_enemyUnits)
        {
            m_InfluenceFrontMap.addUnitInfluence(unit.m_pUnit, unit.m_pPfs.get(), unit.m_movepoints);
        }
        m_InfluenceFrontMap.updateOwners();
        m_InfluenceFrontMap.findFrontLines();
        Console::print("HeavyAi front lines created", Console::eDEBUG);
        findHqThreads(buildings);
    }

    qint32 cost = 0;
    m_pPlayer->getSiloRockettarget(2, 3, cost);
    m_missileTarget = (cost >= minSiloDamage);
}

void HeavyAi::createIslandMaps()
{
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    qint32 unitCount = pUnitSpriteManager->getCount();
    for (qint32 i = 0; i < unitCount; i++)
    {
        QString unitId = pUnitSpriteManager->getID(i);
        createIslandMap(pUnitSpriteManager->getMovementType(unitId), unitId);
    }
}

void HeavyAi::initUnits(QmlVectorUnit* pUnits, QVector<UnitData> & units, bool enemyUnits)
{
    units.clear();
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        UnitData data;
        data.m_pUnit = pUnits->at(i);
        data.m_pPfs = new UnitPathFindingSystem(pUnits->at(i));
        data.m_movepoints = data.m_pUnit->getMovementpoints(data.m_pUnit->getPosition());
        data.m_pPfs->setMovepoints(data.m_movepoints * 2);
        data.m_pPfs->setIgnoreEnemies(enemyUnits);
        data.m_pPfs->explore();
        units.append(data);
    }
}

void HeavyAi::updateUnits()
{
    updateUnits(m_ownUnits, false);
    updateUnits(m_enemyUnits, true);
    m_updatePoints.clear();
}

void HeavyAi::updateUnits(QVector<UnitData> & units, bool enemyUnits)
{
    QVector<qint32> updated;
    for (qint32 i = 0; i < m_updatePoints.size(); i++)
    {
        qint32 i2 = 0;
        while (i2 < units.size())
        {
            if (!updated.contains(i2))
            {
                if (units[i2].m_pUnit->getHp() > 0)
                {
                    if (qAbs(m_updatePoints[i].x() - units[i2].m_pUnit->getX()) +
                        qAbs(m_updatePoints[i].y() - units[i2].m_pUnit->getY()) <=
                        units[i2].m_pUnit->getMovementpoints(QPoint(units[i2].m_pUnit->getX(), units[i2].m_pUnit->getY())) + 2)
                    {
                        units[i2].m_pPfs = new UnitPathFindingSystem(units[i2].m_pUnit);
                        units[i2].m_pPfs->setIgnoreEnemies(enemyUnits);
                        units[i2].m_pPfs->explore();
                    }
                    updated.push_back(i2);
                    i2++;
                }
                else
                {
                    units.removeAt(i2);
                }
            }
        }
    }
}

void HeavyAi::findHqThreads(const spQmlVectorBuilding & buildings)
{
    Console::print("Searching for HQ Threads", Console::eDEBUG);
    QVector<QVector3D> hqPositions;
    for (qint32 i = 0; i < buildings->size(); ++i)
    {
        const auto * pBuilding = buildings->at(i);
        if (pBuilding->getBuildingID() == "HQ")
        {
            hqPositions.append(QVector3D(pBuilding->getX(), pBuilding->getY(), 1));
        }
    }
    for (auto & enemy : m_enemyUnits)
    {
        if (isCaptureTransporterOrCanCapture(enemy.m_pUnit))
        {
            constexpr qint32 dayDistance = 3;
            QPoint pos = enemy.m_pUnit->getMapPosition();
            qint32 movePoints = enemy.m_pUnit->getMovementpoints(enemy.m_pUnit->getMapPosition());
            for (const auto & hqPos : hqPositions)
            {
                if (GlobalUtils::getDistance(pos, QPoint(hqPos.x(), hqPos.y())) <= dayDistance * movePoints)
                {
                    TargetedUnitPathFindingSystem pfs = TargetedUnitPathFindingSystem(enemy.m_pUnit, hqPositions, &m_MoveCostMap);
                    pfs.setIgnoreEnemies(true);
                    pfs.setMovepoints(movePoints * dayDistance);
                    pfs.setAbortOnCostExceed(true);
                    pfs.setFast(true);
                    pfs.setUseBasecosts(true);
                    pfs.explore();
                    QPoint targetFields = pfs.getReachableTargetField(movePoints * 3);
                    if (targetFields.x() >= 0)
                    {
                        enemy.m_threadLevel = ThreadLevel::Hq;
                        enemy.m_hqThread = targetFields;
                    }
                }
            }
        }
    }
}

bool HeavyAi::isCaptureTransporterOrCanCapture(Unit* pUnit)
{
    bool canCapture = false;
    if (pUnit->getActionList().contains(CoreAI::ACTION_CAPTURE))
    {
        canCapture = true;
    }
    else if (pUnit->getLoadedUnitCount() > 0)
    {
        for (qint32 i = 0; i < pUnit->getLoadedUnitCount(); ++i)
        {
            if (isCaptureTransporterOrCanCapture(pUnit->getLoadedUnit(i)))
            {
                canCapture = true;
                break;
            }
        }
    }
    return canCapture;
}
