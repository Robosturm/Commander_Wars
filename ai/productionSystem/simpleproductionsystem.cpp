#include "ai/productionSystem/simpleproductionsystem.h"
#include "ai/coreai.h"
#include "game/gamemap.h"
#include "game/player.h"
#include "coreengine/gameconsole.h"
#include "coreengine/interpreter.h"
#include "coreengine/globalutils.h"
#include "resource_management/unitspritemanager.h"

#include <QCryptographicHash>

#include <limits>

namespace
{
constexpr quint32 PRODUCTION_QUERY_SEED = 0;
// Pinned so a Qt upgrade cannot silently change every derived Counterpoint seed.
constexpr QDataStream::Version COUNTERPOINT_SEED_STREAM_VERSION = QDataStream::Version::Qt_6_5;
constexpr qint32 COUNTERPOINT_SEED_HALF_BITS = std::numeric_limits<quint16>::digits;
constexpr qint32 COUNTERPOINT_SEED_HALF_MAX =
    static_cast<qint32>(std::numeric_limits<quint16>::max());
const QString BASE_PRODUCTION_MENU_FUNCTION = QStringLiteral("getStepData");
const QString CUSTOM_PRODUCTION_MENU_FUNCTION = QStringLiteral("getProductionMenuData");
const QString COUNTERPOINT_SEED_NAMESPACE = QStringLiteral("counterpoint-production");
const QString PREPARE_PRODUCTION_FUNCTION = QStringLiteral("prepareProduction");
const QString BASE_PRODUCTION_ACTION_FUNCTION = QStringLiteral("getIsBaseProductionAction");
// Comfortably above any real roster, including large mods, and a hard ceiling on retained units.
constexpr std::size_t COUNTERPOINT_UNIT_CACHE_LIMIT = 512;
}

SimpleProductionSystem::SimpleProductionSystem(CoreAI * owner)
    : m_owner(owner)
{
#ifdef GRAPHICSUPPORT
    setObjectName("SimpleProductionSystem");
#endif
    Interpreter::setCppOwnerShip(this);
    setupJsThis(this);
}

void SimpleProductionSystem::initialize()
{
    if (!m_init)
    {
        QJSValueList args({m_jsThis,
                           JsThis::getJsThis(m_owner),
                           GameMap::getMapJsThis(m_owner->getMap())});
        QJSValue erg = dispatchScriptFunction(QStringLiteral("initializeSimpleProductionSystem"), args);
        if (erg.isBool())
        {
            m_init = erg.toBool();
        }
    }
    else
    {
        for (auto & item : m_buildDistribution)
        {
            if (item.second.units.length() == 0)
            {
                for (auto & unitId : item.second.unitIds)
                {
                    item.second.units.append(MemoryManagement::create<Unit>(unitId, m_owner->getPlayer(), false, m_owner->getMap()));
                }
            }
        }
    }
}

bool SimpleProductionSystem::buildUnit(QmlVectorBuilding* pBuildings, QmlVectorUnit* pUnits, QmlVectorUnit * pEnemyUnits, QmlVectorBuilding * pEnemyBuildings, bool & executed)
{
    executed = false;
    if (m_enabled && m_init)
    {
        QJSValueList args({m_jsThis,
                           JsThis::getJsThis(m_owner),
                           JsThis::getJsThis(pBuildings),
                           JsThis::getJsThis(pUnits),
                           JsThis::getJsThis(pEnemyUnits),
                           JsThis::getJsThis(pEnemyBuildings),
                           GameMap::getMapJsThis(m_owner->getMap())});
        QJSValue erg = dispatchScriptFunction(QStringLiteral("buildUnitSimpleProductionSystem"), args);
        if (erg.isBool())
        {
            executed = erg.toBool();
        }
    }
    return m_init && m_enabled;
}

QJSValue SimpleProductionSystem::dispatchScriptFunction(const QString & function, const QJSValueList & args) const
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue erg(false);
    if (pInterpreter->exists(GameScript::m_scriptName, function))
    {
        erg = pInterpreter->doFunction(GameScript::m_scriptName, function, args);
    }
    if (erg.isBool() && !erg.toBool() && pInterpreter->exists(m_owner->getAiName(), function))
    {
        erg = pInterpreter->doFunction(m_owner->getAiName(), function, args);
    }
    return erg;
}

Building* SimpleProductionSystem::ownedBuildingAt(qint32 x, qint32 y) const
{
    if (m_owner == nullptr || m_owner->getPlayer() == nullptr)
    {
        return nullptr;
    }
    GameMap* pMap = m_owner->getMap();
    if (pMap == nullptr || !pMap->onMap(x, y))
    {
        return nullptr;
    }
    Building* pBuilding = pMap->getTerrain(x, y)->getBuilding();
    if (pBuilding == nullptr || pBuilding->getOwner() != m_owner->getPlayer())
    {
        return nullptr;
    }
    return pBuilding;
}

void SimpleProductionSystem::resetProductionPreparation()
{
    m_productionPrepared = false;
}

void SimpleProductionSystem::prepareProduction(QmlVectorBuilding* pBuildings, QmlVectorUnit* pUnits)
{
    if (!m_enabled || !m_init || m_productionPrepared)
    {
        return;
    }
    Interpreter* pInterpreter = Interpreter::getInstance();
    const bool gameScriptHandles = pInterpreter->exists(GameScript::m_scriptName, PREPARE_PRODUCTION_FUNCTION);
    const bool aiHandles = pInterpreter->exists(m_owner->getAiName(), PREPARE_PRODUCTION_FUNCTION);
    if (!gameScriptHandles && !aiHandles)
    {
        return;
    }
    m_productionPrepared = true;
    // Deliberately unpruned, unlike the vector the ordinary build queue receives: production
    // planning scores the whole enemy army.
    spQmlVectorUnit pEnemyUnits = m_owner->getPlayer()->getSpEnemyUnits();
    spQmlVectorBuilding pEnemyBuildings = m_owner->getPlayer()->getSpEnemyBuildings();
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(m_owner),
                       JsThis::getJsThis(pBuildings),
                       JsThis::getJsThis(pUnits),
                       JsThis::getJsThis(pEnemyUnits.get()),
                       JsThis::getJsThis(pEnemyBuildings.get()),
                       GameMap::getMapJsThis(m_owner->getMap())});
    dispatchScriptFunction(PREPARE_PRODUCTION_FUNCTION, args);
}

void SimpleProductionSystem::onNewBuildQueue(QmlVectorBuilding* pBuildings, QmlVectorUnit* pUnits, spQmlVectorUnit &pEnemyUnits, QmlVectorBuilding * pEnemyBuildings)
{
    m_pEnemyUnits = pEnemyUnits;
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(m_owner),
                       JsThis::getJsThis(pBuildings),
                       JsThis::getJsThis(pUnits),
                       JsThis::getJsThis(pEnemyUnits.get()),
                       JsThis::getJsThis(pEnemyBuildings),
                       GameMap::getMapJsThis(m_owner->getMap())});
    dispatchScriptFunction(QStringLiteral("onNewBuildQueue"), args);
    updateActiveProductionSystem(pBuildings);
    updateIslandSizeForBuildings(pBuildings);
}

void SimpleProductionSystem::updateActiveProductionSystem(QmlVectorBuilding* pBuildings)
{
    m_activeBuildDistribution = m_buildDistribution;
    for (auto & distribution : m_activeBuildDistribution)
    {
        qint32 i = 0;
        while (i < distribution.second.unitIds.size())
        {
            bool found = false;
            for (auto & pBuilding : pBuildings->getVector())
            {
                if (pBuilding->isProductionBuilding() &&
                    pBuilding->getConstructionList().contains(distribution.second.unitIds[i]))
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                distribution.second.unitIds.removeAt(i);
                distribution.second.chance.removeAt(i);
            }
            else
            {
                ++i;
            }
        }
        distribution.second.totalChance = 0;
        for (auto & itemChance : distribution.second.chance)
        {
            distribution.second.totalChance += itemChance;
        }
    }
}

qint32 SimpleProductionSystem::getMaxSingleDamage() const
{
    return m_maxSingleDamage;
}

void SimpleProductionSystem::setMaxSingleDamage(qint32 newMaxSingleDamage)
{
    m_maxSingleDamage = newMaxSingleDamage;
}

ProductionActionData* SimpleProductionSystem::getProductionActionData(Building* pBuilding, const QString & actionId) const
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    Q_ASSERT(pInterpreter->getInJsCall());
    if (!pInterpreter->getInJsCall())
    {
        return nullptr;
    }
    spProductionActionData pData = queryProductionAction(pBuilding, actionId);
    if (pData.get() != nullptr)
    {
        pInterpreter->trackJsObject(pData);
    }
    return pData.get();
}

quint32 SimpleProductionSystem::deriveCounterpointSeed(qint32 algorithmVersion, qint32 generation) const
{
    if (m_owner == nullptr || m_owner->getMap() == nullptr || m_owner->getPlayer() == nullptr)
    {
        // Zero is also a legal hash result, so the caller cannot tell these apart. Say so here
        // rather than let every failing system share one silent stream.
        CONSOLE_PRINT("Counterpoint seed unavailable, falling back to a shared stream", GameConsole::eERROR);
        return 0;
    }
    GameMap* pMap = m_owner->getMap();
    QByteArray seedData;
    QDataStream seedStream(&seedData, QIODevice::WriteOnly);
    seedStream.setVersion(COUNTERPOINT_SEED_STREAM_VERSION);
    seedStream << COUNTERPOINT_SEED_NAMESPACE;
    seedStream << algorithmVersion;
    seedStream << m_owner->getPlayer()->getPlayerID();
    seedStream << pMap->getCurrentDay();
    seedStream << generation;
    seedStream << pMap->getMapHash();
    const QByteArray hash = QCryptographicHash::hash(seedData, QCryptographicHash::Sha256);
    QDataStream hashStream(hash);
    hashStream.setVersion(COUNTERPOINT_SEED_STREAM_VERSION);
    quint32 seed = 0;
    hashStream >> seed;
    return seed;
}

quint32 SimpleProductionSystem::randomCounterpointSeed() const
{
    const auto high = static_cast<quint32>(
        GlobalUtils::randIntBase(0, COUNTERPOINT_SEED_HALF_MAX));
    const auto low = static_cast<quint32>(
        GlobalUtils::randIntBase(0, COUNTERPOINT_SEED_HALF_MAX));
    return (high << COUNTERPOINT_SEED_HALF_BITS) | low;
}

spUnit SimpleProductionSystem::getCounterpointUnit(const QString & unitId)
{
    auto entry = m_counterpointUnits.find(unitId);
    if (entry != m_counterpointUnits.end())
    {
        return entry->second;
    }
    spUnit pUnit = MemoryManagement::create<Unit>(unitId, m_owner->getPlayer(), false, m_owner->getMap());
    // Past the cap callers still get a unit, it is just not retained, so an oversized roster
    // degrades to the uncached cost instead of thrashing the cache.
    if (m_counterpointUnits.size() < COUNTERPOINT_UNIT_CACHE_LIMIT)
    {
        m_counterpointUnits.emplace(unitId, pUnit);
    }
    else
    {
        CONSOLE_PRINT("Counterpoint unit cache limit reached, " + unitId + " stays uncached", GameConsole::eERROR);
    }
    return pUnit;
}

qreal SimpleProductionSystem::getCounterpointBaseDamage(const QString & attackerId, const QString & defenderId)
{
    if (m_owner == nullptr || m_owner->getMap() == nullptr || m_owner->getPlayer() == nullptr ||
        !UnitSpriteManager::getInstance()->exists(attackerId) ||
        !UnitSpriteManager::getInstance()->exists(defenderId))
    {
        return 0;
    }
    // Base damage is a weapon table lookup keyed by unit id, so the pair is stable for the
    // whole match and the units can be reused.
    spUnit pAttacker = getCounterpointUnit(attackerId);
    spUnit pDefender = getCounterpointUnit(defenderId);
    return pAttacker->getBaseDamage(pDefender.get());
}

bool SimpleProductionSystem::executeCounterpointBuild(qint32 x, qint32 y, const QString & unitId, qint32 ordinal, qint32 expectedCost)
{
    if (unitId.isEmpty() || ordinal < 0)
    {
        return false;
    }
    Building* pBuilding = ownedBuildingAt(x, y);
    if (pBuilding == nullptr)
    {
        return false;
    }
    return executeBuildAction(pBuilding, unitId, ordinal, expectedCost, true);
}

bool SimpleProductionSystem::isBaseProductionAction(const QString & actionId) const
{
    auto entry = m_baseProductionActions.find(actionId);
    if (entry != m_baseProductionActions.end())
    {
        return entry->second;
    }
    Interpreter* pInterpreter = Interpreter::getInstance();
    bool result = false;
    if (pInterpreter->exists(actionId, BASE_PRODUCTION_ACTION_FUNCTION))
    {
        QJSValueList args({GameMap::getMapJsThis(m_owner->getMap())});
        result = pInterpreter->doFunction(actionId, BASE_PRODUCTION_ACTION_FUNCTION, args).toBool();
    }
    else
    {
        // Keeps mods that replace a base action script without the new hook working.
        result = actionId == CoreAI::ACTION_BUILD_UNITS ||
                 actionId == CoreAI::ACTION_BLACKHOLEFACTORY_DOOR1 ||
                 actionId == CoreAI::ACTION_BLACKHOLEFACTORY_DOOR2 ||
                 actionId == CoreAI::ACTION_BLACKHOLEFACTORY_DOOR3 ||
                 actionId == CoreAI::ACTION_NEST_FACTORY_DOOR ||
                 actionId == CoreAI::ACTION_PRODUCE_OOZIUM_FREE;
    }
    m_baseProductionActions.emplace(actionId, result);
    return result;
}

spProductionActionData SimpleProductionSystem::queryProductionAction(Building* pBuilding, const QString & actionId) const
{
    GameMap* pMap = m_owner != nullptr ? m_owner->getMap() : nullptr;
    if (pBuilding == nullptr ||
        pMap == nullptr ||
        pBuilding->getMap() != pMap ||
        pBuilding->getOwner() == nullptr ||
        pBuilding->getOwner() != m_owner->getPlayer() ||
        !pMap->onMap(pBuilding->getX(), pBuilding->getY()) ||
        pMap->getTerrain(pBuilding->getX(), pBuilding->getY())->getBuilding() != pBuilding ||
        !pBuilding->getActionList().contains(actionId))
    {
        return nullptr;
    }

    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function;
    if (pInterpreter->exists(actionId, CUSTOM_PRODUCTION_MENU_FUNCTION))
    {
        function = CUSTOM_PRODUCTION_MENU_FUNCTION;
    }
    else if (isBaseProductionAction(actionId))
    {
        function = BASE_PRODUCTION_MENU_FUNCTION;
    }
    else
    {
        return nullptr;
    }

    spGameAction pAction = MemoryManagement::create<GameAction>(actionId, pMap, PRODUCTION_QUERY_SEED);
    pAction->setTarget(pBuilding->getPosition());
    spProductionActionData pData = MemoryManagement::create<ProductionActionData>(pMap, pBuilding->getX(), pBuilding->getY(), actionId);
    QJSValueList args({JsThis::getJsThis(pAction.get()),
                       pInterpreter->newQObject(pData.get()),
                       GameMap::getMapJsThis(pMap)});
    QJSValue result = pInterpreter->doFunction(actionId, function, args);
    if (result.isError() || !pData->validData())
    {
        return nullptr;
    }
    pData->setActionAvailable(pAction->canBePerformed(actionId, false, pBuilding->getOwner()));
    return pData;
}

qint32 SimpleProductionSystem::getMaxDamageCheckRange() const
{
    return m_maxDamageCheckRange;
}

void SimpleProductionSystem::setMaxDamageCheckRange(qint32 newMaxDamageCheckRange)
{
    m_maxDamageCheckRange = newMaxDamageCheckRange;
}

qint32 SimpleProductionSystem::getCurrentTurnProducedUnitsCounter() const
{
    return m_currentTurnProducedUnitsCounter;
}

void SimpleProductionSystem::setCurrentTurnProducedUnitsCounter(qint32 newCurrentTurnProducedUnitsCounter)
{
    m_currentTurnProducedUnitsCounter = newCurrentTurnProducedUnitsCounter;
}

void SimpleProductionSystem::updateIslandSizeForBuildings(QmlVectorBuilding* pBuildings)
{
    m_averageMoverange.clear();
    for (auto & pBuilding : pBuildings->getVector())
    {
        if (pBuilding->isProductionBuilding())
        {
            qint32 x = pBuilding->getX();
            qint32 y = pBuilding->getY();
            qreal averageCount = 0;
            QStringList prodList = pBuilding->getConstructionList();
            auto & item = m_averageMoverange[pBuilding.get()];
            for (const auto & unitId : prodList)
            {
                spUnit pUnit = MemoryManagement::create<Unit>(unitId, m_owner->getPlayer(), false, m_owner->getMap());
                qreal islandSize = m_owner->getIslandSize(pUnit.get(), x, y);
                item.averageValue += islandSize;
                item.islandSizes[unitId] = islandSize;
                ++averageCount;
            }
            if (averageCount > 0)
            {
                item.averageValue = item.averageValue / averageCount;
            }
        }
    }
}

void SimpleProductionSystem::resetInitialProduction()
{
    m_initialProduction.clear();
}

void SimpleProductionSystem::resetBuildDistribution()
{
    m_buildDistribution.clear();
}

void SimpleProductionSystem::resetForcedProduction()
{
    m_forcedProduction.clear();
}

void SimpleProductionSystem::resetPriorityProduction()
{
    m_priorityProduction.clear();
}

void SimpleProductionSystem::addPriorityProduction(const QStringList & unitIds, qint32 x, qint32 y, bool blocking)
{
    if (unitIds.isEmpty())
    {
        CONSOLE_PRINT("SimpleProductionSystem::addPriorityProduction ignoring entry with empty unitIds", GameConsole::eWARNING);
        return;
    }
    for (auto & existing : m_priorityProduction)
    {
        // makes re-queueing every turn idempotent; a changed blocking flag
        // updates the entry instead of duplicating it
        if (existing.x == x &&
            existing.y == y &&
            existing.unitIds == unitIds)
        {
            existing.blocking = blocking;
            return;
        }
    }
    PriorityProduction item;
    item.unitIds = unitIds;
    item.x = x;
    item.y = y;
    item.blocking = blocking;
    m_priorityProduction.push_back(item);
}

qint32 SimpleProductionSystem::getPriorityProductionCount() const
{
    return static_cast<qint32>(m_priorityProduction.size());
}

QString SimpleProductionSystem::getLastPriorityBuildResult() const
{
    return m_lastPriorityBuildResult;
}

void SimpleProductionSystem::addForcedProduction(const QStringList & unitIds, qint32 x, qint32 y)
{
    ForcedProduction item;
    item.unitIds = unitIds;
    item.x = x;
    item.y = y;
    m_forcedProduction.push_back(item);
}

void SimpleProductionSystem::addForcedProductionCloseToTargets(const QStringList & unitIds, QmlVectorUnit* targets)
{
    ForcedProduction item;
    item.unitIds = unitIds;
    item.targets = MemoryManagement::create<QmlVectorUnit>();
    item.targets->clone(targets);
    m_forcedProduction.push_back(item);
}

void SimpleProductionSystem::addInitialProduction(const QStringList & unitIds, qint32 count)
{
    InitialProduction item;
    item.unitIds = unitIds;
    item.count = count;
    m_initialProduction.push_back(item);
}

void SimpleProductionSystem::addItemToBuildDistribution(const QString & group, const QStringList & unitIds, const QVector<qint32> & chance, qreal distribution, qint32 buildMode, const QString & guardCondition, qreal maxUnitDistribution)
{
    if (unitIds.length() == chance.length())
    {
        if (m_buildDistribution.contains(group))
        {
            auto & item = m_buildDistribution[group];
            for (qint32 i = 0; i < unitIds.length(); ++i)
            {
                auto index = item.unitIds.indexOf(unitIds[i]);
                if (index >= 0)
                {
                    item.chance[index] += chance[index];
                }
                else
                {
                    item.unitIds.append(unitIds[i]);
                    item.chance.append(chance[i]);
                    item.units.append(MemoryManagement::create<Unit>(unitIds[i], m_owner->getPlayer(), false, m_owner->getMap()));
                }
            }
            item.distribution = distribution;
            item.maxUnitDistribution = maxUnitDistribution;
            item.buildMode = buildMode;
            item.guardCondition = guardCondition;
        }
        else
        {
            BuildDistribution item;
            item.unitIds = unitIds;
            for (auto & unitId : unitIds)
            {
                item.units.append(MemoryManagement::create<Unit>(unitId, m_owner->getPlayer(), false, m_owner->getMap()));
            }
            item.maxUnitDistribution = maxUnitDistribution;
            item.distribution = distribution;
            item.buildMode = buildMode;
            item.guardCondition = guardCondition;
            item.chance = chance;
            m_buildDistribution[group] = item;
        }
    }
    else
    {
        CONSOLE_PRINT("Invalid call to SimpleProductionSystem::addItemToBuildDistribution unitIds and chances have different lengths", GameConsole::eERROR);
    }
}

bool SimpleProductionSystem::buildPriorityProduction(QmlVectorBuilding* pBuildings, bool & blocked)
{
    bool success = false;
    // cleared up front so the token always describes this build pass, never a stale turn
    m_lastPriorityBuildResult.clear();
    GameMap* pMap = m_owner->getMap();
    for (qint32 i = 0; i < m_priorityProduction.size(); ++i)
    {
        auto & item = m_priorityProduction[i];
        BuildFailure reason = BuildFailure::NoFactory;
        QString lastUnitId = item.unitIds[0];
        if (pMap->onMap(item.x, item.y))
        {
            Building* pBuilding = pMap->getTerrain(item.x, item.y)->getBuilding();
            if (pBuilding != nullptr &&
                pBuilding->getOwner() == m_owner->getPlayer())
            {
                for (auto & unitId : item.unitIds)
                {
                    lastUnitId = unitId;
                    success = buildUnit(item.x, item.y, unitId, true, &reason);
                    if (success)
                    {
                        break;
                    }
                }
            }
            else
            {
                reason = BuildFailure::InvalidPosition;
            }
        }
        else
        {
            for (auto & unitId : item.unitIds)
            {
                lastUnitId = unitId;
                // island filter bypassed on purpose, the build menu decides instead
                success = buildUnit(pBuildings, unitId, 0.0, true, &reason);
                if (success)
                {
                    break;
                }
            }
        }
        m_lastPriorityBuildResult = (success ? QString("BUILT") : QString(buildFailureName(reason))) + "|" + lastUnitId + "|" +
                                    QString::number(item.x) + "," + QString::number(item.y);
        CONSOLE_PRINT("SimpleProductionSystem priority production " + m_lastPriorityBuildResult, GameConsole::eDEBUG);
        if (success)
        {
            m_priorityProduction.erase(m_priorityProduction.cbegin() + i);
            break;
        }
        else if (reason != BuildFailure::NoFunds &&
                 reason != BuildFailure::FactoryBlocked)
        {
            // permanent failure: drop the entry so a blocking one cannot starve the ai forever
            CONSOLE_PRINT("SimpleProductionSystem dropping priority production " + m_lastPriorityBuildResult, GameConsole::eWARNING);
            m_priorityProduction.erase(m_priorityProduction.cbegin() + i);
            --i;
        }
        else if (item.blocking)
        {
            blocked = true;
            break;
        }
    }
    return success;
}

bool SimpleProductionSystem::buildNextUnit(QmlVectorBuilding* pBuildings, QmlVectorUnit* pUnits, qint32 minBuildMode, qint32 maxBuildMode,
                                           qreal minAverageIslandSize, qint32 minBaseCost, qint32 maxBaseCost, bool alwaysBuild)
{
    if (maxBaseCost < 0)
    {
        maxBaseCost = m_owner->getPlayer()->getFunds();
    }
    GameMap* pMap = m_owner->getMap();
    bool blocked = false;
    bool success = buildPriorityProduction(pBuildings, blocked);
    if (blocked)
    {
        return false;
    }
    for (qint32 i = 0; i < m_initialProduction.size() && !success; ++i)
    {
        auto & item = m_initialProduction[i];
        for (auto & unitId : item.unitIds)
        {
            success = buildUnit(pBuildings, unitId, minAverageIslandSize, alwaysBuild);
            if (success)
            {
                --item.count;
                if (item.count <= 0)
                {
                    m_initialProduction.erase(m_initialProduction.cbegin() + i);
                }
                break;
            }
        }
        if (success)
        {
            break;
        }
    }
    if (!success)
    {
        for (qint32 i = 0; i < m_forcedProduction.size(); ++i)
        {
            auto & forcedProduction = m_forcedProduction[i];
            QStringList unitIds(forcedProduction.unitIds.size(), "");
            qint32 size = unitIds.size() - 1;
            for (const QString & item : forcedProduction.unitIds)
            {
                qint32 count = 0;
                while (count < 100)
                {
                    qint32 index = GlobalUtils::randIntBase(0, size);
                    if (unitIds[index].isEmpty())
                    {
                        unitIds[index] = item;
                        break;
                    }
                    ++count;
                }
            }
            if (pMap->onMap(forcedProduction.x, forcedProduction.y) &&
                pMap->getTerrain(forcedProduction.x, forcedProduction.y)->getBuilding() != nullptr &&
                pMap->getTerrain(forcedProduction.x, forcedProduction.y)->getBuilding()->getOwner() == m_owner->getPlayer())
            {
                for (auto & unitId : forcedProduction.unitIds)
                {
                    success = buildUnit(forcedProduction.x, forcedProduction.y, unitId, alwaysBuild);
                    if (success)
                    {
                        break;
                    }
                }
            }
            else
            {
                for (auto & unitId : forcedProduction.unitIds)
                {
                    if (forcedProduction.targets.get() != nullptr)
                    {
                        success = buildUnitCloseTo(pBuildings, unitId, minAverageIslandSize, forcedProduction.targets, alwaysBuild);
                    }
                    else
                    {
                        success = buildUnit(pBuildings, unitId, minAverageIslandSize, alwaysBuild);
                    }
                    if (success)
                    {
                        break;
                    }
                }
            }
            if (success)
            {
                m_forcedProduction.erase(m_forcedProduction.cbegin() + i);
                break;
            }
        }
    }
    if (!success)
    {
        std::vector<CurrentBuildDistribution> buildDistribution;
        getBuildDistribution(buildDistribution, pUnits, minBuildMode, maxBuildMode, minBaseCost, maxBaseCost);
        // try building the unit group which has the highest gap
        for (auto & item : buildDistribution)
        {            
            auto count = item.distribution.unitIds.length();
            if (count > 0)
            {
                if (count == 1)
                {
                    success = buildUnit(pBuildings, item.distribution.unitIds[0], minAverageIslandSize, alwaysBuild);
                }
                else
                {
                    for (qint32 i = 0; i < item.distribution.unitIds.length() * 3; ++i)
                    {
                        qint32 roll = GlobalUtils::randInt(0, item.distribution.totalChance);
                        qint32 chance = 0;
                        for (qint32 i2 = 0; i2 < item.distribution.unitIds.length(); ++i2)
                        {
                            if (roll < chance + item.distribution.chance[i2])
                            {
                                success = buildUnit(pBuildings, item.distribution.unitIds[i2], minAverageIslandSize, alwaysBuild);
                                break;
                            }
                            else
                            {
                                chance += item.distribution.chance[i2];
                            }
                            if (success)
                            {
                                break;
                            }
                        }
                        if (success)
                        {
                            break;
                        }
                    }
                }
                if (success)
                {
                    break;
                }
            }
        }
    }
    return success;
}

qint32 SimpleProductionSystem::getProductionFromList(const QStringList & unitIds, QmlVectorUnit* pUnits, QmlVectorBuilding* pBuildings, qint32 minBuildMode, qint32 maxBuildMode, const QVector<bool> & enableList)
{
    if (m_activeBuildDistribution.size() == 0)
    {
        updateActiveProductionSystem(pBuildings);
    }
    constexpr qint32 minBaseCost = 0;
    constexpr qint32 maxBaseCost = -1;
    std::vector<CurrentBuildDistribution> buildDistribution;
    getBuildDistribution(buildDistribution, pUnits, minBuildMode, maxBuildMode, minBaseCost, maxBaseCost);
    qint32 index = -1;
    for (auto & item : buildDistribution)
    {
        auto count = item.distribution.unitIds.length();
        if (count > 0)
        {
            if (count == 1)
            {
                index = unitIds.indexOf(item.distribution.unitIds[0]);
                if (index >= 0 && (enableList.size() == 0 || enableList[index]))
                {
                    break;
                }
            }
            else
            {
                for (qint32 i = 0; i < item.distribution.unitIds.length() * 3; ++i)
                {
                    qint32 roll = GlobalUtils::randInt(0, item.distribution.totalChance);
                    qint32 chance = 0;
                    for (qint32 i2 = 0; i2 < item.distribution.unitIds.length(); ++i2)
                    {
                        if (roll < chance + item.distribution.chance[i2])
                        {
                            index = unitIds.indexOf(item.distribution.unitIds[i2]);
                        }
                        else
                        {
                            chance += item.distribution.chance[i2];
                        }
                        if (index >= 0 && (enableList.size() == 0 || enableList[index]))
                        {
                            break;
                        }
                    }
                    if (index >= 0 && (enableList.size() == 0 || enableList[index]))
                    {
                        break;
                    }
                }
            }
            if (index >= 0 && (enableList.size() == 0 || enableList[index]))
            {
                break;
            }
        }
    }
    return index;
}

void SimpleProductionSystem::getBuildDistribution(std::vector<CurrentBuildDistribution> & buildDistribution, QmlVectorUnit* pUnits,
                                                  qint32 minBuildMode, qint32 maxBuildMode, qint32 minBaseCost, qint32 maxBaseCost)
{
    QMap<QString, qreal> unitCounts;
    for (auto & unit : pUnits->getVector())
    {
        auto unitId = unit->getUnitID();
        for (const auto& [key, value] : m_activeBuildDistribution)
        {
            if (value.unitIds.contains(unitId))
            {
                ++unitCounts[key];
            }
        }
    }
    Interpreter* pInterpreter = Interpreter::getInstance();
    qreal totalUnitCount = pUnits->size();
    qreal totalDistributionCount = 0.0;
    for (const auto& [key, value] : m_activeBuildDistribution)
    {
        qreal distribution = 0.0;
        if (unitCounts.contains(key))
        {
            distribution = unitCounts[key] / totalUnitCount;
        }
        if (minBuildMode <= value.buildMode &&
            value.buildMode <= maxBuildMode &&
            value.unitIds.size() > 0 &&
            distribution <= value.maxUnitDistribution)
        {
            if (value.guardCondition.isEmpty() || pInterpreter->doFunction(value.guardCondition).toBool())
            {
                totalDistributionCount += value.distribution;
                CurrentBuildDistribution item;
                item.distribution = value;
                if (unitCounts.contains(key))
                {
                    item.currentValue = distribution;
                }
                else
                {
                    item.currentValue = 0.0f;
                }
                buildDistribution.push_back(item);
            }
        }
    }
    std::sort(buildDistribution.begin(), buildDistribution.end(), [totalDistributionCount](const CurrentBuildDistribution& lhs, const CurrentBuildDistribution& rhs)
    {
        if (lhs.currentValue <= 0.0f &&
            lhs.distribution.buildMode > 1 &&
            rhs.currentValue > 0.0f)
        {
            return true;
        }
        else if (lhs.currentValue > 0.0f && rhs.currentValue <= 0.0f)
        {
            return false;
        }
        else
        {
            return lhs.distribution.distribution / totalDistributionCount - lhs.currentValue > rhs.distribution.distribution / totalDistributionCount - rhs.currentValue;
        }
    });
    for (auto & item : buildDistribution)
    {
        qint32 i = 0;
        while(i < item.distribution.unitIds.size())
        {
            qint32 cost = item.distribution.units[i]->getUnitCosts();
            if (cost >= minBaseCost &&
                cost <= maxBaseCost)
            {
                ++i;
            }
            else
            {
                item.distribution.totalChance -= item.distribution.chance[i];
                item.distribution.unitIds.removeAt(i);
                item.distribution.chance.removeAt(i);
            }
        }
    }

    CONSOLE_PRINT("Created builddistribution minMode=" + QString::number(minBuildMode) + " maxMode=" + QString::number(maxBuildMode) + " found items=" + QString::number(buildDistribution.size()), GameConsole::eDEBUG);
}

bool SimpleProductionSystem::getInit() const
{
    return m_init;
}

bool SimpleProductionSystem::buildUnitCloseTo(QmlVectorBuilding* pBuildings, QString unitId, qreal minAverageIslandSize, const spQmlVectorUnit & pUnits, bool alwaysBuild)
{
    bool success = false;
    spQmlVectorBuilding buildings = MemoryManagement::create<QmlVectorBuilding>();
    buildings->clone(pBuildings);
    buildings->sortClosestToEnemy(pUnits);
    for (auto & pBuilding : buildings->getVector())
    {
        auto & item = m_averageMoverange[pBuilding.get()];
        if (item.averageValue * minAverageIslandSize <= item.islandSizes[unitId])
        {
            success = buildUnit(pBuilding->getX(), pBuilding->getY(), unitId, alwaysBuild);
            if (success)
            {
                break;
            }
        }
    }
    if (!success)
    {
        CONSOLE_PRINT("Unable to build " + unitId, GameConsole::eDEBUG);
    }
    return success;
}

bool SimpleProductionSystem::buildUnit(QmlVectorBuilding* pBuildings, QString unitId, qreal minAverageIslandSize, bool alwaysBuild, BuildFailure * failureReason)
{
    bool success = false;
    for (auto & pBuilding : pBuildings->getVector())
    {
        auto & item = m_averageMoverange[pBuilding.get()];
        if (item.averageValue * minAverageIslandSize <= item.islandSizes[unitId])
        {
            BuildFailure reason = BuildFailure::NoFactory;
            success = buildUnit(pBuilding->getX(), pBuilding->getY(), unitId, alwaysBuild, failureReason != nullptr ? &reason : nullptr);
            if (success)
            {
                break;
            }
            // keep the most retryable failure so one town cannot mask a blocked factory
            if (failureReason != nullptr &&
                reason > *failureReason)
            {
                *failureReason = reason;
            }
        }
    }
    if (!success)
    {
        CONSOLE_PRINT("Unable to build " + unitId, GameConsole::eDEBUG);
    }
    return success;
}

void SimpleProductionSystem::setBuildFailure(BuildFailure * failureReason, BuildFailure reason)
{
    if (failureReason != nullptr)
    {
        *failureReason = reason;
    }
}

// the only place the reason names exist, converted at the script and console boundary
const char* SimpleProductionSystem::buildFailureName(BuildFailure reason)
{
    switch (reason)
    {
        case BuildFailure::NoFactory: return "NO_FACTORY";
        case BuildFailure::Danger: return "DANGER";
        case BuildFailure::InvalidPosition: return "INVALID_POSITION";
        case BuildFailure::NotAFactory: return "NOT_A_FACTORY";
        case BuildFailure::NotAllowed: return "NOT_ALLOWED";
        case BuildFailure::NotInBuildList: return "NOT_IN_BUILD_LIST";
        case BuildFailure::Disabled: return "DISABLED";
        case BuildFailure::FactoryBlocked: return "FACTORY_BLOCKED";
        case BuildFailure::NoFunds: return "NO_FUNDS";
    }
    return "UNKNOWN";
}

bool SimpleProductionSystem::buildUnit(qint32 x, qint32 y, QString unitId, bool alwaysBuild, BuildFailure * failureReason)
{
    if (unitId.isEmpty())
    {
        setBuildFailure(failureReason, BuildFailure::NotInBuildList);
        return false;
    }
    Building* pBuilding = ownedBuildingAt(x, y);
    if (pBuilding == nullptr)
    {
        setBuildFailure(failureReason, BuildFailure::InvalidPosition);
        return false;
    }
    return executeBuildAction(pBuilding, unitId, DEFAULT_ACTION_ORDINAL, NO_EXPECTED_COST, alwaysBuild, failureReason);
}

bool SimpleProductionSystem::executeBuildAction(Building* pBuilding, const QString & unitId, qint32 ordinal, qint32 expectedCost, bool alwaysBuild, BuildFailure * failureReason)
{
    if (pBuilding == nullptr || pBuilding->getOwner() != m_owner->getPlayer() ||
        !pBuilding->getActionList().contains(CoreAI::ACTION_BUILD_UNITS))
    {
        setBuildFailure(failureReason, BuildFailure::NotAFactory);
        return false;
    }
    if (pBuilding->getTerrain()->getUnit() != nullptr)
    {
        setBuildFailure(failureReason, BuildFailure::FactoryBlocked);
        return false;
    }
    if (!alwaysBuild &&
        !reasonableBuildField(pBuilding->getX(), pBuilding->getY(), unitId, m_maxDamageCheckRange, m_maxSingleDamage))
    {
        setBuildFailure(failureReason, BuildFailure::Danger);
        return false;
    }
    // Ask about this unit before the action does, because ACTION_BUILD_UNITS
    // refuses outright when the player cannot afford anything the factory
    // offers. That reads as NOT_ALLOWED, which callers treat as permanent, when
    // it is only poverty and will fix itself once funds accumulate.
    if (m_owner->getPlayer()->getCosts(unitId, pBuilding->getPosition()) > m_owner->getPlayer()->getFunds())
    {
        setBuildFailure(failureReason, BuildFailure::NoFunds);
        return false;
    }
    spGameAction pAction = MemoryManagement::create<GameAction>(CoreAI::ACTION_BUILD_UNITS, m_owner->getMap());
    pAction->setTarget(pBuilding->getPosition());
    if (!pAction->canBePerformed())
    {
        setBuildFailure(failureReason, BuildFailure::NotAllowed);
        return false;
    }
    spMenuData pData = pAction->getMenuStepData();
    if (!pData->validData())
    {
        setBuildFailure(failureReason, BuildFailure::NotAllowed);
        return false;
    }
    const QStringList actionIds = pData->getActionIDs();
    qint32 currentOrdinal = 0;
    qint32 selectedIndex = -1;
    for (qint32 index = 0; index < actionIds.size(); ++index)
    {
        if (actionIds[index] == unitId)
        {
            if (currentOrdinal == ordinal)
            {
                selectedIndex = index;
                break;
            }
            ++currentOrdinal;
        }
    }
    if (selectedIndex < 0)
    {
        setBuildFailure(failureReason, BuildFailure::NotInBuildList);
        return false;
    }
    if (!pData->getEnabledList()[selectedIndex])
    {
        if (pData->getCostList()[selectedIndex] > m_owner->getPlayer()->getFunds())
        {
            setBuildFailure(failureReason, BuildFailure::NoFunds);
        }
        else
        {
            setBuildFailure(failureReason, BuildFailure::Disabled);
        }
        return false;
    }
    const qint32 liveCost = pData->getCostList()[selectedIndex];
    const bool hasExpectedCost = expectedCost > NO_EXPECTED_COST;
    if (hasExpectedCost && liveCost != expectedCost)
    {
        setBuildFailure(failureReason, BuildFailure::Disabled);
        return false;
    }
    m_owner->addMenuItemData(pAction, unitId, liveCost);
    if (!pAction->isFinalStep() || !pAction->canBePerformed())
    {
        setBuildFailure(failureReason, BuildFailure::NotAllowed);
        return false;
    }
    CONSOLE_PRINT("Building unit " + unitId + " at x=" + QString::number(pBuilding->getX()) + " y=" + QString::number(pBuilding->getY()), GameConsole::eDEBUG);
    ++m_currentTurnProducedUnitsCounter;
    emit m_owner->sigPerformAction(pAction);
    return true;
}

bool SimpleProductionSystem::reasonableBuildField(qint32 x, qint32 y, QString unitId, qint32 maxDamageCheckRange, qint32 maxSingleDamage)
{
    bool allowed = true;
    auto* pMap = m_owner->getMap();
    for (auto & pEnemey : m_pEnemyUnits->getVector())
    {
        if (GlobalUtils::getDistance(x, y, pEnemey->getX(), pEnemey->getY()) <= maxDamageCheckRange &&
            (pEnemey->hasAmmo1() || pEnemey->hasAmmo2()))
        {
            auto baseDamage = UnitSpriteManager::getInstance()->getBaseDamage(pEnemey->getUnitIdx(), UnitSpriteManager::getInstance()->getIndex(unitId));
            if (baseDamage >= maxSingleDamage)
            {
                QPoint position = pEnemey->getPosition();
                bool canMoveAndFire = pEnemey->canMoveAndFire(position);
                qint32 maxRange = pEnemey->getMaxRange(position);
                qint32 minRange = pEnemey->getMinRange(position);
                std::vector<QPoint> points;
                if (canMoveAndFire)
                {
                    UnitPathFindingSystem pfs(pMap, pEnemey.get(), pEnemey->getOwner());
                    pfs.explore();
                    points = pfs.getAllNodePointsFast();
                }
                else
                {
                    points.push_back(position);
                }
                for (auto & point : points)
                {
                    auto distance = GlobalUtils::getDistance(x, y, point.x(), point.y());
                    if (distance >= minRange && distance <= maxRange)
                    {
                        allowed = false;
                        break;
                    }
                }
                if (!allowed)
                {
                    break;
                }
            }
        }
    }
    return allowed;
}

bool SimpleProductionSystem::getEnabled() const
{
    return m_enabled;
}

void SimpleProductionSystem::setEnabled(bool newEnabled)
{
    m_enabled = newEnabled;
}

void SimpleProductionSystem::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    pStream << m_init;
    pStream << static_cast<qint32>(m_buildDistribution.size());
    for (const auto& [key, value] : m_buildDistribution)
    {
        pStream << key;
        pStream << static_cast<qint32>(value.unitIds.size());
        for (qint32 i2 = 0; i2 < value.unitIds.size(); ++i2)
        {
            pStream << value.unitIds[i2];
            pStream << value.chance[i2];
        }
        pStream << value.totalChance;
        pStream << value.distribution;
        pStream << value.maxUnitDistribution;
        pStream << value.guardCondition;
        pStream << value.buildMode;
    }
    pStream << static_cast<qint32>(m_initialProduction.size());
    for (const auto& item : m_initialProduction)
    {
        pStream << static_cast<qint32>(item.unitIds.size());
        for (qint32 i2 = 0; i2 < item.unitIds.size(); ++i2)
        {
            pStream << item.unitIds[i2];
        }
        pStream << item.count;
    }
    m_Variables.serializeObject(pStream);
    pStream << m_currentTurnProducedUnitsCounter;
    // version 2: forced production loses close-to-target references, those entries degrade to any factory
    pStream << static_cast<qint32>(m_forcedProduction.size());
    for (const auto& item : m_forcedProduction)
    {
        pStream << item.x;
        pStream << item.y;
        pStream << static_cast<qint32>(item.unitIds.size());
        for (const auto& unitId : item.unitIds)
        {
            pStream << unitId;
        }
    }
    pStream << static_cast<qint32>(m_priorityProduction.size());
    for (const auto& item : m_priorityProduction)
    {
        pStream << item.x;
        pStream << item.y;
        pStream << item.blocking;
        pStream << static_cast<qint32>(item.unitIds.size());
        for (const auto& unitId : item.unitIds)
        {
            pStream << unitId;
        }
    }
}

void SimpleProductionSystem::deserializeObject(QDataStream& pStream)
{
    qint32 version;
    pStream >> version;
    pStream >> m_init;
    qint32 size = 0;
    pStream >> size;
    for (qint32 i = 0; i < size; ++i)
    {
        QString key;
        pStream >> key;
        m_buildDistribution[key] = BuildDistribution();
        auto & item = m_buildDistribution[key];
        qint32 size2 = 0;
        pStream >> size2;
        for (qint32 i2 = 0; i2 < size2; ++i2)
        {
            QString id;
            pStream >> id;
            item.unitIds.push_back(id);
            qint32 chance;
            pStream >> chance;
            item.chance.push_back(chance);
        }
        pStream >> item.totalChance;
        pStream >> item.distribution;
        pStream >> item.maxUnitDistribution;
        pStream >> item.guardCondition;
        pStream >> item.buildMode;
    }
    pStream >> size;
    for (qint32 i = 0; i < size; ++i)
    {
        InitialProduction item;
        qint32 size2 = 0;
        pStream >> size2;
        // size2 is this entry's id count; size is the entry count, and reading by it misaligns.
        for (qint32 i2 = 0; i2 < size2; ++i2)
        {
            QString id;
            pStream >> id;
            item.unitIds.append(id);
        }
        pStream >> item.count;
        m_initialProduction.push_back(item);
    }
    m_Variables.deserializeObject(pStream);
    if (version > 0)
    {
        pStream >> m_currentTurnProducedUnitsCounter;
    }
    if (version > 1)
    {
        pStream >> size;
        for (qint32 i = 0; i < size; ++i)
        {
            ForcedProduction item;
            pStream >> item.x;
            pStream >> item.y;
            qint32 size2 = 0;
            pStream >> size2;
            for (qint32 i2 = 0; i2 < size2; ++i2)
            {
                QString id;
                pStream >> id;
                item.unitIds.append(id);
            }
            m_forcedProduction.push_back(item);
        }
        pStream >> size;
        for (qint32 i = 0; i < size; ++i)
        {
            PriorityProduction item;
            pStream >> item.x;
            pStream >> item.y;
            pStream >> item.blocking;
            qint32 size2 = 0;
            pStream >> size2;
            for (qint32 i2 = 0; i2 < size2; ++i2)
            {
                QString id;
                pStream >> id;
                item.unitIds.append(id);
            }
            if (!item.unitIds.isEmpty())
            {
                m_priorityProduction.push_back(item);
            }
        }
    }
}

Unit* SimpleProductionSystem::getDummyUnit(const QString & unitId)
{
    m_dummy = MemoryManagement::create<Unit>(unitId, m_owner->getPlayer(), false, nullptr);
    return m_dummy.get();
}
