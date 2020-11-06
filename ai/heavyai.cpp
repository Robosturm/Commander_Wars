#include "heavyai.h"

#include "coreengine/qmlvector.h"

#include "game/player.h"

const qint32 HeavyAi::minSiloDamage = 7000;

HeavyAi::HeavyAi()
    : CoreAI(GameEnums::AiTypes_Heavy)
{
    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, &HeavyAi::process, Qt::QueuedConnection);
}

void HeavyAi::toggleAiPause()
{
    m_pause = !m_pause;
}

void HeavyAi::showFrontMap()
{
    m_InfluenceFrontMap.show();
}

void HeavyAi::hideFrontMap()
{
    m_InfluenceFrontMap.hide();
}

void HeavyAi::process()
{
    spQmlVectorBuilding pBuildings = m_pPlayer->getBuildings();
    pBuildings->randomize();
    setupTurn();
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

void HeavyAi::setupTurn()
{
    bool startOfTurn = (m_pUnits.get() == nullptr);
    if (m_pUnits.get() == nullptr)
    {
        m_pUnits = m_pPlayer->getUnits();
        m_pUnits->sortShortestMovementRange(true);
        initUnits(m_pUnits.get(), m_ownUnits);
    }
    if (m_pEnemyUnits.get() == nullptr)
    {
        m_pEnemyUnits = m_pPlayer->getEnemyUnits();
        m_pEnemyUnits->randomize();
        initUnits(m_pEnemyUnits.get(), m_enemyUnits);
    }
    if (startOfTurn)
    {
        // create influence map at the start of the turn
        m_InfluenceFrontMap.reset();
        for (auto & unit : m_ownUnits)
        {
            m_InfluenceFrontMap.addUnitInfluence(unit.m_pUnit, unit.m_pPfs.get(), unit.m_movepoints);
        }
        for (auto & unit : m_enemyUnits)
        {
            m_InfluenceFrontMap.addUnitInfluence(unit.m_pUnit, unit.m_pPfs.get(), unit.m_movepoints);
        }
    }

    qint32 cost = 0;
    m_pPlayer->getSiloRockettarget(2, 3, cost);
    m_missileTarget = (cost >= minSiloDamage);
}

void HeavyAi::initUnits(QmlVectorUnit* pUnits, QVector<UnitData> & units)
{
    units.clear();
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        UnitData data;
        data.m_pUnit = pUnits->at(i);
        data.m_pPfs = new UnitPathFindingSystem(pUnits->at(i));
        data.m_movepoints = data.m_pUnit->getMovementpoints(data.m_pUnit->getPosition());
        data.m_pPfs->setMovepoints(data.m_movepoints * 2);
        data.m_pPfs->explore();
        units.append(data);
    }
}

void HeavyAi::updateUnits()
{
    updateUnits(m_ownUnits);
    updateUnits(m_enemyUnits);
    m_updatePoints.clear();
}

void HeavyAi::updateUnits(QVector<UnitData> & units)
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
