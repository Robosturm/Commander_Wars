#include "game/weather.h"
#include "game/gamemap.h"

#include "coreengine/interpreter.h"

Weather::Weather(QString weatherId, GameMap* pMap)
    : m_WeatherId(weatherId),
      m_pMap{pMap}
{
#ifdef GRAPHICSUPPORT
    setObjectName("Weather");
#endif
    Interpreter::setCppOwnerShip(this);
    setupJsThis(this);
}

Weather::Weather(GameMap* pMap)
    : m_pMap{pMap}
{
#ifdef GRAPHICSUPPORT
    setObjectName("Weather");
#endif
    Interpreter::setCppOwnerShip(this);
    setupJsThis(this);
}

void Weather::serializeObject(QDataStream& pStream) const
{
    serializeObject(pStream, false);
}

void Weather::serializeObject(QDataStream& pStream, bool forHash) const
{
    pStream << getVersion();
    pStream << m_WeatherId;
    m_Variables.serializeObject(pStream, forHash);
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
    QJSValueList args({m_jsThis,
                       GameMap::getMapJsThis(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_WeatherId, function1, args);
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
    QJSValueList args({m_jsThis,
                       GameMap::getMapJsThis(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_WeatherId, function1, args);
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
    QJSValueList args({m_jsThis,
                       GameMap::getMapJsThis(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_WeatherId, function1, args);
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
    QJSValueList args({m_jsThis,
                       GameMap::getMapJsThis(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_WeatherId, function1, args);
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
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(pUnit),
                       JsThis::getJsThis(pTerrain),
                       GameMap::getMapJsThis(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_WeatherId, function1, args);
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
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(pUnit),
                       JsThis::getJsThis(pTerrain),
                       GameMap::getMapJsThis(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_WeatherId, function1, args);
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
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(pUnit),
                       fuelCost,
                       GameMap::getMapJsThis(m_pMap)});
    QJSValue erg = pInterpreter->doFunction(m_WeatherId, function1, args);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 Weather::getVisionrangeModifier(Unit* pUnit, Terrain* pTerrain)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getVisionrangeModifier";
    QJSValueList args({m_jsThis,
                       GameMap::getMapJsThis(m_pMap),
                       JsThis::getJsThis(pUnit),
                       JsThis::getJsThis(pTerrain),});
    QJSValue erg = pInterpreter->doFunction(m_WeatherId, function1, args);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 Weather::getFirerangeModifier(Unit* pUnit, Terrain* pTerrain)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getFirerangeModifier";
    QJSValueList args({m_jsThis,
                       GameMap::getMapJsThis(m_pMap),
                       JsThis::getJsThis(pUnit),
                       JsThis::getJsThis(pTerrain),});
    QJSValue erg = pInterpreter->doFunction(m_WeatherId, function1, args);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 Weather::getMinFirerangeModifier(Unit* pUnit, Terrain* pTerrain)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getMinFirerangeModifier";
    QJSValueList args({m_jsThis,
                       GameMap::getMapJsThis(m_pMap),
                       JsThis::getJsThis(pUnit),
                       JsThis::getJsThis(pTerrain),});
    QJSValue erg = pInterpreter->doFunction(m_WeatherId, function1, args);
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
    QJSValueList args({m_jsThis,
                       GameMap::getMapJsThis(m_pMap)});
    pInterpreter->doFunction(m_WeatherId, function1, args);
}

void Weather::deactivate()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "deactivate";
    QJSValueList args({m_jsThis,
                       GameMap::getMapJsThis(m_pMap)});
    pInterpreter->doFunction(m_WeatherId, function1, args);
}

QString Weather::getWeatherId() const
{
    return m_WeatherId;
}
