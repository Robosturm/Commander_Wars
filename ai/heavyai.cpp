#include "heavyai.h"

#include "coreengine/qmlvector.h"

#include "game/player.h"

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

void HeavyAi::process()
{
    if (m_pause)
    {
        m_timer.start(1000);
        return;
    }

    if (m_pUnits == nullptr)
    {
        m_pUnits = m_pPlayer->getUnits();
        m_pUnits->sortShortestMovementRange(true);
        initUnits(m_pUnits, m_ownUnits);
    }
    if (m_pEnemyUnits == nullptr)
    {
        m_pEnemyUnits = m_pPlayer->getEnemyUnits();
        m_pEnemyUnits->randomize();
        initUnits(m_pEnemyUnits, m_enemyUnits);
    }

    turnMode = GameEnums::AiTurnMode_EndOfDay;
    if (useCOPower(m_pUnits, m_pEnemyUnits))
    {
        turnMode = GameEnums::AiTurnMode_DuringDay;
    }
    else
    {
        turnMode = GameEnums::AiTurnMode_StartOfDay;
        delete m_pUnits;
        m_pUnits = nullptr;
        delete m_pEnemyUnits;
        m_pEnemyUnits = nullptr;
        finishTurn();
    }
}

void HeavyAi::initUnits(QmlVectorUnit* pUnits, QVector<UnitData> & units)
{
    units.clear();
    for (qint32 i = 0; i < pUnits->size(); i++)
    {
        UnitData data;
        data.m_pUnit = pUnits->at(i);
        data.m_pPfs = new UnitPathFindingSystem(pUnits->at(i));
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
