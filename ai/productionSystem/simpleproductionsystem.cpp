#include "ai/productionSystem/simpleproductionsystem.h"
#include "ai/coreai.h"
#include "game/gamemap.h"
#include "coreengine/gameconsole.h"
#include "coreengine/interpreter.h"
#include "coreengine/globalutils.h"

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
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValueList args({m_jsThis,
                           JsThis::getJsThis(m_owner),
                           JsThis::getJsThis(m_owner->getMap())});
        QString function1 = "initializeSimpleProductionSystem";
        QJSValue erg(false);
        if (pInterpreter->exists(GameScript::m_scriptName, function1))
        {
            erg = pInterpreter->doFunction(GameScript::m_scriptName, function1, args);
        }
        if (erg.isBool() && !erg.toBool())
        {
            if (pInterpreter->exists(m_owner->getAiName(), function1))
            {
                erg = pInterpreter->doFunction(m_owner->getAiName(), function1, args);
            }
        }
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
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "buildUnitSimpleProductionSystem";
        QJSValueList args({m_jsThis,
                           JsThis::getJsThis(m_owner),
                           JsThis::getJsThis(pBuildings),
                           JsThis::getJsThis(pUnits),
                           JsThis::getJsThis(pEnemyUnits),
                           JsThis::getJsThis(pEnemyBuildings),
                           JsThis::getJsThis(m_owner->getMap())});
        QJSValue erg(false);
        if (pInterpreter->exists(GameScript::m_scriptName, function1))
        {
            erg = pInterpreter->doFunction(GameScript::m_scriptName, function1, args);
        }
        if (erg.isBool() && !erg.toBool())
        {
            if (pInterpreter->exists(m_owner->getAiName(), function1))
            {
                erg = pInterpreter->doFunction(m_owner->getAiName(), function1, args);
            }
        }
        if (erg.isBool())
        {
            executed = erg.toBool();
        }
    }
    return m_init && m_enabled;
}

void SimpleProductionSystem::onNewBuildQueue(QmlVectorBuilding* pBuildings, QmlVectorUnit* pUnits, QmlVectorUnit * pEnemyUnits, QmlVectorBuilding * pEnemyBuildings)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "onNewBuildQueue";
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(m_owner),
                       JsThis::getJsThis(pBuildings),
                       JsThis::getJsThis(pUnits),
                       JsThis::getJsThis(pEnemyUnits),
                       JsThis::getJsThis(pEnemyBuildings),
                       JsThis::getJsThis(m_owner->getMap())});
    QJSValue erg(false);
    if (pInterpreter->exists(GameScript::m_scriptName, function1))
    {
        erg = pInterpreter->doFunction(GameScript::m_scriptName, function1, args);
    }
    if (erg.isBool() && !erg.toBool())
    {
        if (pInterpreter->exists(m_owner->getAiName(), function1))
        {
            erg = pInterpreter->doFunction(m_owner->getAiName(), function1, args);
        }
    }
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

bool SimpleProductionSystem::buildNextUnit(QmlVectorBuilding* pBuildings, QmlVectorUnit* pUnits, qint32 minBuildMode, qint32 maxBuildMode,
                                           qreal minAverageIslandSize, qint32 minBaseCost, qint32 maxBaseCost)
{
    if (maxBaseCost < 0)
    {
        maxBaseCost = m_owner->getPlayer()->getFunds();
    }
    bool success = false;
    GameMap* pMap = m_owner->getMap();
    for (qint32 i = 0; i < m_initialProduction.size(); ++i)
    {
        auto & item = m_initialProduction[i];
        for (auto & unitId : item.unitIds)
        {
            success = buildUnit(pBuildings, unitId, minAverageIslandSize);
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
                    success = buildUnit(forcedProduction.x, forcedProduction.y, unitId);
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
                        success = buildUnitCloseTo(pBuildings, unitId, minAverageIslandSize, forcedProduction.targets);
                    }
                    else
                    {
                        success = buildUnit(pBuildings, unitId, minAverageIslandSize);
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
                    success = buildUnit(pBuildings, item.distribution.unitIds[0], minAverageIslandSize);
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
                                success = buildUnit(pBuildings, item.distribution.unitIds[i2], minAverageIslandSize);
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
            if (cost >= minBaseCost ||
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

bool SimpleProductionSystem::buildUnitCloseTo(QmlVectorBuilding* pBuildings, QString unitId, qreal minAverageIslandSize, const spQmlVectorUnit & pUnits)
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
            success = buildUnit(pBuilding->getX(), pBuilding->getY(), unitId);
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

bool SimpleProductionSystem::buildUnit(QmlVectorBuilding* pBuildings, QString unitId, qreal minAverageIslandSize)
{
    bool success = false;
    for (auto & pBuilding : pBuildings->getVector())
    {
        auto & item = m_averageMoverange[pBuilding.get()];
        if (item.averageValue * minAverageIslandSize <= item.islandSizes[unitId])
        {
            success = buildUnit(pBuilding->getX(), pBuilding->getY(), unitId);
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

bool SimpleProductionSystem::buildUnit(qint32 x, qint32 y, QString unitId)
{
    Building* pBuilding = m_owner->getMap()->getTerrain(x, y)->getBuilding();
    if (pBuilding->getActionList().contains(CoreAI::ACTION_BUILD_UNITS) &&
        pBuilding->getTerrain()->getUnit() == nullptr)
    {
        spGameAction pAction = MemoryManagement::create<GameAction>(CoreAI::ACTION_BUILD_UNITS, m_owner->getMap());
        pAction->setTarget(QPoint(x, y));
        if (pAction->canBePerformed())
        {
            // we're allowed to build units here
            spMenuData pData = pAction->getMenuStepData();
            if (pData->validData())
            {
                auto indexOf = pData->getActionIDs().indexOf(unitId);
                if (indexOf >= 0 && pData->getEnabledList()[indexOf])
                {
                    m_owner->addMenuItemData(pAction, unitId, pData->getCostList()[indexOf]);
                    // produce the unit
                    if (pAction->isFinalStep())
                    {
                        if (pAction->canBePerformed())
                        {
                            CONSOLE_PRINT("Building unit " + unitId + " at x=" + QString::number(x) + " y=" + QString::number(y), GameConsole::eDEBUG);
                            ++m_currentTurnProducedUnitsCounter;
                            emit m_owner->sigPerformAction(pAction);
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
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
        for (qint32 i2 = 0; i2 < size; ++i2)
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
}

Unit* SimpleProductionSystem::getDummyUnit(const QString & unitId)
{
    m_dummy = MemoryManagement::create<Unit>(unitId, m_owner->getPlayer(), false, nullptr);
    return m_dummy.get();
}
