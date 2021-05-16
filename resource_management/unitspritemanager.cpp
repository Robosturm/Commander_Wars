#include "resource_management/unitspritemanager.h"

UnitSpriteManager::UnitSpriteManager()
    : RessourceManagement<UnitSpriteManager>("/images/units/res.xml",
                                             "/scripts/units")
{
    setObjectName("UnitSpriteManager");
}

qint32 UnitSpriteManager::getUnitType(qint32 i)
{
    if ((i >= 0) && (i < m_loadedRessources.size()))
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "getUnitType";
        QJSValue ret = pInterpreter->doFunction(m_loadedRessources[i], function1);
        if (ret.isNumber())
        {
            return ret.toInt();
        }
    }
    return static_cast<qint32>(GameEnums::UnitType_Ground);
}

QString UnitSpriteManager::getMovementType(QString id)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getMovementType";
    QJSValue ret = pInterpreter->doFunction(id, function1);
    if (ret.isString())
    {
        return ret.toString();
    }
    return "";
}

qint32 UnitSpriteManager::getUnitType(QString id)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getUnitType";
    QJSValue ret = pInterpreter->doFunction(id, function1);
    if (ret.isNumber())
    {
        return ret.toInt();
    }
    return static_cast<qint32>(GameEnums::UnitType_Ground);
}

QStringList UnitSpriteManager::getUnitsSorted()
{
    QStringList sortedUnits;
    QVector<qint32> unitTypes;
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getUnitTypeSort";
    QJSValue ret = pInterpreter->doFunction("UNIT", function1);
    auto list = ret.toVariant().toList();
    for (const auto & unitType : qAsConst(list))
    {
        qint32 value = unitType.toInt();
        if (!unitTypes.contains(value))
        {
            unitTypes.append(value);
        }
    }
    for (qint32 i = 0; i < m_loadedRessources.size(); i++)
    {
        qint32 unitType = getUnitType(i);
        if (!unitTypes.contains(unitType))
        {
            unitTypes.append(unitType);
        }
    }
    for (qint32 i2 = 0; i2 < unitTypes.size(); i2++)
    {
        if (pInterpreter->exists("UNIT", "getUnitTypeSort" + QString::number(unitTypes[i2])))
        {
            function1 = "getUnitTypeSort" + QString::number(unitTypes[i2]);
            ret = pInterpreter->doFunction("UNIT", function1);
            sortedUnits.append(ret.toVariant().toStringList());
        }
        for (qint32 i = 0; i < m_loadedRessources.size(); i++)
        {

            if (getUnitType(i) == unitTypes[i2] && !sortedUnits.contains(m_loadedRessources[i]))
            {
                sortedUnits.append(m_loadedRessources[i]);
            }
        }

    }
    return sortedUnits;
}

QString UnitSpriteManager::getUnitTypeText(qint32 type)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getUnitTypeText";
    QJSValueList args;
    args << type;
    QJSValue ret = pInterpreter->doFunction("UNIT", function1, args);
    if (ret.isString())
    {
        return ret.toString();
    }
    return tr("Ground");
}

void UnitSpriteManager::removeRessource(QString id)
{
    for (qint32 i = 0; i < m_loadedRessources.size(); ++i)
    {
        if (m_loadedRessources[i] == id)
        {
            m_loadedRessources.removeAt(i);
            break;
        }
    }
}
