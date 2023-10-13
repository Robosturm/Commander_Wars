
#include "coreengine/qmlvector.h"
#include "coreengine/gameconsole.h"
#include "coreengine/globalutils.h"

#include "ai/heavyai/heavyai.h"
#include "ai/heavyai/heavyAiSharedData.h"

#include "game/player.h"
#include "game/gameaction.h"
#include "game/gamemap.h"

#include "resource_management/unitspritemanager.h"

HeavyAi::HeavyAi(GameMap* pMap, QString type, GameEnums::AiTypes aiType)
    : CoreAI(pMap, aiType, type)
{
#ifdef GRAPHICSUPPORT
    setObjectName("HeavyAi");
#endif
    Interpreter::setCppOwnerShip(this);
    setupJsThis(this);

    if (m_pMap != nullptr &&
        !m_pMap->getSavegame())
    {
        loadIni("heavy/" + m_aiName.toLower() + ".ini");
    }
    CONSOLE_PRINT("Creating heavy ai", GameConsole::eDEBUG);
}

void HeavyAi::toggleAiPause()
{
    m_pause = !m_pause;
}

void HeavyAi::showIslandMap(QString unitId)
{
    Unit unit(unitId, m_pPlayer, false, m_pMap);
    qint32 unitIslandIdx = getIslandIndex(&unit);
    if (unitIslandIdx >= 0 && unitIslandIdx < m_IslandMaps.size())
    {
        m_IslandMaps[unitIslandIdx]->show();
    }
}

void HeavyAi::hideIslandMap(QString unitId)
{
    Unit unit(unitId, m_pPlayer, false, m_pMap);
    qint32 unitIslandIdx = getIslandIndex(&unit);
    if (unitIslandIdx >= 0 && unitIslandIdx < m_IslandMaps.size())
    {
        m_IslandMaps[unitIslandIdx]->hide();
    }
}

void HeavyAi::process()
{
    AI_CONSOLE_PRINT("NormalAi::process()", GameConsole::eDEBUG);
    if (m_pause)
    {
        m_timer.start(1000);
        return;
    }
    else
    {
        m_timer.stop();
    }

    spQmlVectorBuilding pBuildings = m_pPlayer->getSpBuildings();
    pBuildings->randomize();
    spQmlVectorUnit pUnits = m_pPlayer->getSpUnits();
    spQmlVectorUnit pEnemyUnits = m_pPlayer->getSpEnemyUnits();
    pEnemyUnits->pruneEnemies(pUnits.get(), m_enemyPruneRange);
    qint32 cost = 0;
    m_pPlayer->getSiloRockettarget(2, 3, cost);
    m_missileTarget = (cost >= m_minSiloDamage);
    if (useBuilding(pBuildings, pUnits))
    {
        clearUnitCache(pUnits);
        clearUnitCache(pEnemyUnits);
    }
    else
    {
        m_IslandMaps.clear();
        m_turnMode = GameEnums::AiTurnMode_EndOfDay;
        if (useCOPower(pUnits, pEnemyUnits))
        {
            m_usedTransportSystem = false;
            m_usedPredefinedAi = false;
            m_turnMode = GameEnums::AiTurnMode_DuringDay;
            clearUnitCache(pUnits);
            clearUnitCache(pEnemyUnits);

        }
        else
        {
            updateUnitCache(pUnits);
            updateUnitCache(pEnemyUnits);


            m_turnMode = GameEnums::AiTurnMode_StartOfDay;
            finishTurn();
        }
    }
}

void HeavyAi::updateUnitCache(spQmlVectorUnit & pUnits)
{
    for(auto & pUnit : pUnits->getVector())
    {
        auto & cache = pUnit->getAiCache();
        if (cache.size() != static_cast<qint32>(AiCache::Max))
        {
            QPoint pos = pUnit->getPosition();
            cache.resize(HeavyAiSharedData::AiCache::Max);
            cache[HeavyAiSharedData::AiCache::MovementPoints] = pUnit->getMovementpoints(pos);
            cache[HeavyAiSharedData::AiCache::MinFirerange] = pUnit->getMinRange(pos);
            cache[HeavyAiSharedData::AiCache::MaxFirerange] = pUnit->getMaxRange(pos);
            cache[HeavyAiSharedData::AiCache::CanMoveAndFire] = pUnit->canMoveAndFire(pos);
        }
    }
    rebuildIsland(pUnits);
}

void HeavyAi::clearUnitCache(spQmlVectorUnit & pUnits)
{
    for(auto & pUnit : pUnits->getVector())
    {
        pUnit->getAiCache().clear();
    }
}

void HeavyAi::finishTurn()
{
    auto pUnits = m_pPlayer->getSpUnits();
    clearUnitCache(pUnits);
    pUnits = m_pPlayer->getSpEnemyUnits();
    clearUnitCache(pUnits);
    CoreAI::finishTurn();
}
