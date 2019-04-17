#include "weather.h"

#include "coreengine/mainapp.h"

Weather::Weather(QString weatherId)
    : QObject(),
      m_WeatherId(weatherId)
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

Weather::Weather()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

void Weather::serialize(QDataStream& pStream)
{
    pStream << getVersion();
    pStream << m_WeatherId;
    m_Variables.serialize(pStream);
}

void Weather::deserialize(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    pStream >> m_WeatherId;
    m_Variables.deserialize(pStream);
}

QString Weather::getWeatherName()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getWeatherName";
    QJSValue erg = pApp->getInterpreter()->doFunction(m_WeatherId, function1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getWeatherTerrainSprite";
    QJSValue erg = pApp->getInterpreter()->doFunction(m_WeatherId, function1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getWeatherSymbol";
    QJSValue erg = pApp->getInterpreter()->doFunction(m_WeatherId, function1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getOffensiveModifier";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    QJSValue erg = pApp->getInterpreter()->doFunction(m_WeatherId, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getDefensiveModifier";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    QJSValue erg = pApp->getInterpreter()->doFunction(m_WeatherId, function1, args1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 Weather::getMovementCostModifier()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getMovementCostModifier";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    QJSValue erg = pApp->getInterpreter()->doFunction(m_WeatherId, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getVisionrangeModifier";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    QJSValue erg = pApp->getInterpreter()->doFunction(m_WeatherId, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "getFirerangeModifier";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    QJSValue erg = pApp->getInterpreter()->doFunction(m_WeatherId, function1, args1);
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
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "activate";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    pApp->getInterpreter()->doFunction(m_WeatherId, function1, args1);
}

void Weather::deactivate()
{
    Mainapp* pApp = Mainapp::getInstance();
    QString function1 = "deactivate";
    QJSValueList args1;
    QJSValue obj1 = pApp->getInterpreter()->newQObject(this);
    args1 << obj1;
    pApp->getInterpreter()->doFunction(m_WeatherId, function1, args1);
}

QString Weather::getWeatherId() const
{
    return m_WeatherId;
}
