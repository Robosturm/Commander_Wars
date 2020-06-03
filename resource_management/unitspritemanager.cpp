#include "unitspritemanager.h"

UnitSpriteManager::UnitSpriteManager()
    : RessourceManagement<UnitSpriteManager>("/images/units/res.xml",
                                             "/scripts/units")
{
}

GameEnums::UnitType UnitSpriteManager::getUnitType(qint32 i)
{
    if ((i >= 0) && (i < m_loadedRessources.size()))
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "getUnitType";
        QJSValue ret = pInterpreter->doFunction(m_loadedRessources[i], function1);
        if (ret.isNumber())
        {
            return static_cast<GameEnums::UnitType>(ret.toInt());
        }
    }
    return GameEnums::UnitType_Ground;
}


GameEnums::UnitType UnitSpriteManager::getUnitType(QString id)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getUnitType";
    QJSValue ret = pInterpreter->doFunction(id, function1);
    if (ret.isNumber())
    {
        return static_cast<GameEnums::UnitType>(ret.toInt());
    }
    return GameEnums::UnitType_Ground;
}

QStringList UnitSpriteManager::getUnitsSorted()
{
    QStringList sortedUnits;
    QVector<GameEnums::UnitType> unitTypes;
    for (qint32 i = 0; i < m_loadedRessources.size(); i++)
    {
        GameEnums::UnitType unitType = getUnitType(i);
        if (!unitTypes.contains(unitType))
        {
            unitTypes.append(unitType);
        }
    }
    for (qint32 i2 = 0; i2 < unitTypes.size(); i2++)
    {
        for (qint32 i = 0; i < m_loadedRessources.size(); i++)
        {
            if (getUnitType(i) == unitTypes[i2])
            {
                sortedUnits.append(m_loadedRessources[i]);
            }
        }
    }
    return sortedUnits;
}
