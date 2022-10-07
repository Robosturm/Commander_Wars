#include "ai/productionSystem/simpleproductionsystem.h"
#include "ai/coreai.h"
#include "game/gamemap.h"
#include "coreengine/console.h"
#include "coreengine/interpreter.h"
#include "coreengine/globalutils.h"

SimpleProductionSystem::SimpleProductionSystem(CoreAI & owner)
    : m_owner(owner)
{
#ifdef GRAPHICSUPPORT
    setObjectName("SimpleProductionSystem");
#endif
    Interpreter::setCppOwnerShip(this);
}

void SimpleProductionSystem::initialize()
{
    if (!m_init)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValueList args({pInterpreter->newQObject(this),
                           pInterpreter->newQObject(&m_owner),
                           pInterpreter->newQObject(m_owner.getMap())});
        QString function1 = "initializeSimpleProductionSystem";
        if (pInterpreter->exists(GameScript::m_scriptName, function1))
        {
            pInterpreter->doFunction(GameScript::m_scriptName, function1, args);
            m_init = true;
        }
        else if (pInterpreter->exists(m_owner.getAiName(), function1))
        {
            pInterpreter->doFunction(m_owner.getAiName(), function1, args);
            m_init = true;
        }
    }
}

bool SimpleProductionSystem::buildUnit(QmlVectorBuilding* pBuildings, QmlVectorUnit* pUnits, QmlVectorUnit * pEnemyUnits, QmlVectorBuilding * pEnemyBuildings, bool & executed)
{
    executed = false;
    if (m_enabled)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "buildUnitSimpleProductionSystem";
        QJSValueList args({pInterpreter->newQObject(this),
                           pInterpreter->newQObject(&m_owner),
                           pInterpreter->newQObject(pBuildings),
                           pInterpreter->newQObject(pUnits),
                           pInterpreter->newQObject(pEnemyUnits),
                           pInterpreter->newQObject(pEnemyBuildings),
                           pInterpreter->newQObject(m_owner.getMap())});
        QJSValue erg = pInterpreter->doFunction(GameScript::m_scriptName, function1, args);
        if (erg.isBool())
        {
            executed = erg.toBool();
        }
    }
    return m_init && m_enabled;
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

void SimpleProductionSystem::addForcedProduction(const QString & unitId, qint32 x, qint32 y)
{
    ForcedProduction item;
    item.unitId = unitId;
    item.x = x;
    item.y = y;
    m_forcedProduction.push_back(item);
}

void SimpleProductionSystem::addItemToBuildDistribution(const QString & group, const QStringList & unitIds, const QVector<qint32> & chance, float distribution, qint32 buildMode, const QString & guardCondition)
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
                }
            }
            item.distribution = distribution;
            item.buildMode = buildMode;
            item.guardCondition = guardCondition;
        }
        else
        {
            BuildDistribution item;
            item.unitIds = unitIds;
            item.distribution = distribution;
            item.buildMode = buildMode;
            item.guardCondition = guardCondition;
            item.chance = chance;
            m_buildDistribution[group] = item;
        }
        auto & item = m_buildDistribution[group];
        item.totalChance = 0;
        for (auto & itemChance : item.chance)
        {
            item.totalChance += itemChance;
        }
    }
    else
    {
        CONSOLE_PRINT("Invalid call to SimpleProductionSystem::addItemToBuildDistribution unitIds and chances have different lengths", Console::eERROR);
    }
}

bool SimpleProductionSystem::buildNextUnit(QmlVectorBuilding* pBuildings, QmlVectorUnit* pUnits, qint32 minBuildMode, qint32 maxBuildMode)
{
    bool success = false;
    GameMap* pMap = m_owner.getMap();
    for (qint32 i = 0; i < m_initialProduction.size(); ++i)
    {
        success = buildUnit(pBuildings, m_initialProduction[i].unitId);
        if (success)
        {
            --m_initialProduction[i].count;
            if (m_initialProduction[i].count <= 0)
            {
                m_initialProduction.erase(m_initialProduction.cbegin() + i);
            }
            break;
        }
    }
    if (!success)
    {
        for (auto & forcedProduction : m_forcedProduction)
        {
            if (pMap->onMap(forcedProduction.x, forcedProduction.y) &&
                pMap->getTerrain(forcedProduction.x, forcedProduction.y)->getBuilding() != nullptr &&
                pMap->getTerrain(forcedProduction.x, forcedProduction.y)->getBuilding()->getOwner() == m_owner.getPlayer())
            {
                success = buildUnit(forcedProduction.x, forcedProduction.y, forcedProduction.unitId);
                if (success)
                {
                    break;
                }
            }
            else
            {
                success = buildUnit(pBuildings, forcedProduction.unitId);
            }
        }
    }
    if (!success)
    {
        QMap<QString, float> unitCounts;
        for (auto & unit : pUnits->getVector())
        {
            auto unitId = unit->getUnitID();
            for (const auto& [key, value] : m_buildDistribution)
            {
                if (value.unitIds.contains(unitId))
                {
                    ++unitCounts[key];
                }
            }
        }
        Interpreter* pInterpreter = Interpreter::getInstance();
        std::vector<CurrentBuildDistribution> buildDistribution;
        float totalUnitCount = pUnits->size();
        float totalDistributionCount = 0;
        for (const auto& [key, value] : m_buildDistribution)
        {
            if (minBuildMode <= value.buildMode && value.buildMode <= maxBuildMode)
            {
                if (value.guardCondition.isEmpty() || pInterpreter->doFunction(value.guardCondition).toBool())
                {
                    totalDistributionCount += value.distribution;
                    CurrentBuildDistribution item;
                    item.distribution = value;
                    if (unitCounts.contains(key))
                    {
                        item.currentValue = unitCounts[key] / totalUnitCount;
                    }
                    buildDistribution.push_back(item);
                }
            }
        }
        std::sort(buildDistribution.begin(), buildDistribution.end(), [totalDistributionCount](const CurrentBuildDistribution& lhs, const CurrentBuildDistribution& rhs)
        {
            return lhs.distribution.distribution / totalDistributionCount - lhs.currentValue > rhs.distribution.distribution / totalDistributionCount - rhs.currentValue;
        });
        // try building the unit group which has the highest gap
        for (auto & item : buildDistribution)
        {
            if (item.distribution.unitIds.length() == 1)
            {
                success = buildUnit(pBuildings, item.distribution.unitIds[0]);
            }
            else
            {
                for (qint32 i = 0; i < item.distribution.unitIds.length() * 3; ++i)
                {
                    qint32 roll = GlobalUtils::randInt(0, item.distribution.totalChance);
                    qint32 chance = 0;
                    for (qint32 i2 = 0; i2 < item.distribution.unitIds.length(); ++i2)
                    {
                        if (chance < roll)
                        {
                            success = buildUnit(pBuildings, item.distribution.unitIds[i2]);
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
    return success;
}

bool SimpleProductionSystem::buildUnit(QmlVectorBuilding* pBuildings, QString unitId)
{
    bool success = false;
    for (auto & pBuilding : pBuildings->getVector())
    {
        success = buildUnit(pBuilding->getX(), pBuilding->getY(), unitId);
        if (success)
        {
            break;
        }
    }
    return success;
}

bool SimpleProductionSystem::buildUnit(qint32 x, qint32 y, QString unitId)
{
    spGameAction pAction = spGameAction::create(CoreAI::ACTION_BUILD_UNITS, m_owner.getMap());
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
                m_owner.addMenuItemData(pAction, unitId, pData->getCostList()[indexOf]);
                // produce the unit
                if (pAction->isFinalStep())
                {
                    if (pAction->canBePerformed())
                    {
                        emit m_owner.performAction(pAction);
                        return true;
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
        pStream << value.guardCondition;
        pStream << value.buildMode;
    }
    pStream << static_cast<qint32>(m_initialProduction.size());
    for (const auto& item : m_initialProduction)
    {
        pStream << item.unitId;
        pStream << item.count;
    }
    m_Variables.serializeObject(pStream);
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
        pStream >> item.guardCondition;
        pStream >> item.buildMode;
    }
    pStream >> size;
    for (qint32 i = 0; i < size; ++i)
    {
        InitialProduction item;
        pStream >> item.unitId;
        pStream >> item.count;
        m_initialProduction.push_back(item);
    }
    m_Variables.deserializeObject(pStream);
}

