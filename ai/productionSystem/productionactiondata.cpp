#include "ai/productionSystem/productionactiondata.h"
#include "coreengine/interpreter.h"
#include "game/unit.h"
#include "resource_management/unitspritemanager.h"

ProductionActionData::ProductionActionData(GameMap* pMap, qint32 x, qint32 y, QString actionId)
    : m_pMap{pMap},
      m_x{x},
      m_y{y},
      m_actionId{actionId}
{
    Interpreter::setCppOwnerShip(this);
}

void ProductionActionData::addData(QString text, QString unitId, QString icon, qint32 transactionCost, bool enabled)
{
    Q_UNUSED(text);
    Q_UNUSED(icon);
    if (UnitSpriteManager::getInstance()->exists(unitId))
    {
        m_unitIds.append(unitId);
        m_transactionCosts.append(transactionCost);
        m_strategicValues.append(transactionCost == 0 ? Unit::getBaseCosts(unitId, m_pMap) : transactionCost);
        m_enabledList.append(enabled);
    }
}

qint32 ProductionActionData::getX() const
{
    return m_x;
}

qint32 ProductionActionData::getY() const
{
    return m_y;
}

QString ProductionActionData::getActionId() const
{
    return m_actionId;
}

bool ProductionActionData::getActionAvailable() const
{
    return m_actionAvailable;
}

QStringList ProductionActionData::getUnitIds() const
{
    return m_unitIds;
}

QVector<qint32> ProductionActionData::getTransactionCosts() const
{
    return m_transactionCosts;
}

QVector<qint32> ProductionActionData::getStrategicValues() const
{
    return m_strategicValues;
}

QVector<bool> ProductionActionData::getEnabledList() const
{
    return m_enabledList;
}

void ProductionActionData::setActionAvailable(bool actionAvailable)
{
    m_actionAvailable = actionAvailable;
}

bool ProductionActionData::validData() const
{
    return !m_unitIds.isEmpty() &&
           m_unitIds.size() == m_transactionCosts.size() &&
           m_transactionCosts.size() == m_strategicValues.size() &&
           m_strategicValues.size() == m_enabledList.size();
}
