#include "game/gamerule.h"

#include "coreengine/interpreter.h"

const char* const GameRule::checkbox = "checkbox";
const char* const GameRule::spinbox = "spinbox";

GameRule::GameRule()
{
#ifdef GRAPHICSUPPORT
    setObjectName("GameRule");
#endif
    Interpreter::setCppOwnerShip(this);
    setupJsThis(this);
}

GameRule::GameRule(QString ruleID)
    : m_RuleID(ruleID)
{
#ifdef GRAPHICSUPPORT
    setObjectName("GameRule");
#endif
    Interpreter::setCppOwnerShip(this);
    setupJsThis(this);
    init();
}

void GameRule::init()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "init";
    QJSValueList args({m_jsThis});
    pInterpreter->doFunction(m_RuleID, function1, args);
}

QStringList GameRule::getRuleType()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getRuleType";
    QJSValue ret = pInterpreter->doFunction(m_RuleID, function1);
    if (ret.isString())
    {
        return QStringList(ret.toString());
    }
    else
    {
        return ret.toVariant().toStringList();
    }
}

QString GameRule::getRuleName(qint32 itemNumber)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getRuleName";
    QJSValueList args({JsThis::getJsThis(nullptr),
        QJSValue(itemNumber),
        m_jsThis,});
    QJSValue ret = pInterpreter->doFunction(m_RuleID, function1, args);
    if (ret.isString())
    {
        return ret.toString();
    }
    else
    {
        return "";
    }
}

void GameRule::setRuleValue(qint32 value, qint32 itemNumber)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "setRuleValue";
    QJSValueList args({m_jsThis,
                       QJSValue(value),
                       QJSValue(itemNumber)});
    pInterpreter->doFunction(m_RuleID, function1, args);
}

qint32 GameRule::getInfiniteValue(qint32 itemNumber)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getInfiniteValue";
    QJSValueList args({QJSValue(itemNumber)});
    QJSValue ret = pInterpreter->doFunction(m_RuleID, function1, args);
    if (ret.isNumber())
    {
        return ret.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 GameRule::getDefaultValue(qint32 itemNumber)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getDefaultRuleValue";
    QJSValueList args({QJSValue(itemNumber)});
    QJSValue ret = pInterpreter->doFunction(m_RuleID, function1, args);
    if (ret.isNumber())
    {
        return ret.toInt();
    }
    else
    {
        return 0;
    }
}

qint32 GameRule::getRuleValue(qint32 itemNumber)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getRuleValue";
    QJSValueList args({m_jsThis,
                       QJSValue(itemNumber)});
    QJSValue ret = pInterpreter->doFunction(m_RuleID, function1, args);
    if (ret.isNumber())
    {
        return ret.toInt();
    }
    else
    {
        return 0;
    }
}

QString GameRule::getRuleDescription(qint32 itemNumber)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getRuleDescription";
    QJSValueList args({JsThis::getJsThis(nullptr),
                       QJSValue(itemNumber),
                       JsThis::getJsThis(nullptr)});
    QJSValue ret = pInterpreter->doFunction(m_RuleID, function1, args);
    if (ret.isString())
    {
        return ret.toString();
    }
    else
    {
        return "";
    }
}

void GameRule::serializeObject(QDataStream& pStream) const
{
    serializeObject(pStream, false);
}

void GameRule::serializeObject(QDataStream& pStream, bool forHash) const
{
    pStream << getVersion();
    pStream << m_RuleID;
    m_Variables.serializeObject(pStream, forHash);
}

void GameRule::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    pStream >> m_RuleID;
    m_Variables.deserializeObject(pStream);
}

QString GameRule::getRuleID() const
{
    return m_RuleID;
}

