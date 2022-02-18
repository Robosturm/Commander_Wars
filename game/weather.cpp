#include "game/weather.h"
#include "game/gamemap.h"

#include "coreengine/mainapp.h"

Weather::Weather(QString weatherId, GameMap* pMap)
    : QObject(),
      m_WeatherId(weatherId),
      m_pMap{pMap}
{
    setObjectName("Weather");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

Weather::Weather(GameMap* pMap)
    : m_pMap{pMap}
{
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

void Weather::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    pStream << m_WeatherId;
    m_Variables.serializeObject(pStream);
}

void Weather::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    pStream >> m_WeatherId;
    m_Variables.deserializeObject(pStream);
}

QString Weather::getWeatherName()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getWeatherName";
    QJSValue erg = pInterpreter->doFunction(m_WeatherId, function1);
    if (erg.isString())
    {
        return erg.toString();
    }
    else
    {
        return "";
    }
}

QString Weather::getWeatherTerrainSprite()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getWeatherTerrainSprite";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue objArg5 = pInterpreter->newQObject(m_pMap);
    args1 << objArg5;
    QJSValue erg = pInterpreter->doFunction(m_WeatherId, function1, args1);
    if (erg.isString())
    {
        return erg.toString();
    }
    else
    {
        return "";
    }
}

QString Weather::getWeatherSymbol()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getWeatherSymbol";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue objArg5 = pInterpreter->newQObject(m_pMap);
    args1 << objArg5;
    QJSValue erg = pInterpreter->doFunction(m_WeatherId, function1, args1);
    if (erg.isString())
    {
        return erg.toString();
    }
    else
    {
        return "";
    }
}

qint32 Weather::getOffensiveModifier()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getOffensiveModifier";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue objArg5 = pInterpreter->newQObject(m_pMap);
    args1 << objArg5;
    QJSValue erg = pInterpreter->doFunction(m_WeatherId, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 Weather::getDefensiveModifier()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getDefensiveModifier";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue objArg5 = pInterpreter->newQObject(m_pMap);
    args1 << objArg5;
    QJSValue erg = pInterpreter->doFunction(m_WeatherId, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 Weather::getMovementCostModifier(Unit* pUnit, Terrain* pTerrain)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getMovementCostModifier";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue obj2 = pInterpreter->newQObject(pUnit);
    args1 << obj2;
    QJSValue obj3 = pInterpreter->newQObject(pTerrain);
    args1 << obj3;
    QJSValue objArg5 = pInterpreter->newQObject(m_pMap);
    args1 << objArg5;
    QJSValue erg = pInterpreter->doFunction(m_WeatherId, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 Weather::getMovementpointModifier(Unit* pUnit, Terrain* pTerrain)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getMovementpointModifier";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue obj2 = pInterpreter->newQObject(pUnit);
    args1 << obj2;
    QJSValue obj3 = pInterpreter->newQObject(pTerrain);
    args1 << obj3;
    QJSValue objArg5 = pInterpreter->newQObject(m_pMap);
    args1 << objArg5;
    QJSValue erg = pInterpreter->doFunction(m_WeatherId, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 Weather::getMovementFuelCostModifier(Unit* pUnit, qint32 fuelCost)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getMovementFuelCostModifier";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue obj2 = pInterpreter->newQObject(pUnit);
    args1 << obj2;
    args1 << fuelCost;
    QJSValue objArg5 = pInterpreter->newQObject(m_pMap);
    args1 << objArg5;
    QJSValue erg = pInterpreter->doFunction(m_WeatherId, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 Weather::getVisionrangeModifier()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getVisionrangeModifier";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue objArg5 = pInterpreter->newQObject(m_pMap);
    args1 << objArg5;
    QJSValue erg = pInterpreter->doFunction(m_WeatherId, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 Weather::getFirerangeModifier()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getFirerangeModifier";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue objArg5 = pInterpreter->newQObject(m_pMap);
    args1 << objArg5;
    QJSValue erg = pInterpreter->doFunction(m_WeatherId, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 Weather::getMinFirerangeModifier()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getMinFirerangeModifier";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue objArg5 = pInterpreter->newQObject(m_pMap);
    args1 << objArg5;
    QJSValue erg = pInterpreter->doFunction(m_WeatherId, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

void Weather::activate()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "activate";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue objArg5 = pInterpreter->newQObject(m_pMap);
    args1 << objArg5;
    pInterpreter->doFunction(m_WeatherId, function1, args1);
}

void Weather::deactivate()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "deactivate";
    QJSValueList args1;
    QJSValue obj1 = pInterpreter->newQObject(this);
    args1 << obj1;
    QJSValue objArg5 = pInterpreter->newQObject(m_pMap);
    args1 << objArg5;
    pInterpreter->doFunction(m_WeatherId, function1, args1);
}

QString Weather::getWeatherId() const
{
    return m_WeatherId;
}
