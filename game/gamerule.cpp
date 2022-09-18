#include "game/gamerule.h"

#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"

const QString GameRule::checkbox = "checkbox";
const QString GameRule::spinbox = "spinbox";

GameRule::GameRule()
{
#ifdef GRAPHICSUPPORT
    setObjectName("GameRule");
#endif
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

GameRule::GameRule(QString ruleID)
    : m_RuleID(ruleID)
{
#ifdef GRAPHICSUPPORT
    setObjectName("GameRule");
#endif
    Interpreter::setCppOwnerShip(this);
    init();
}

void GameRule::init()
{
    Mainapp* pApp = Mainapp::getInstance();
    Interpreter* pInterpreter = Interpreter::getInstance();
    moveToThread(pApp->getWorkerthread());
    QString function1 = "init";
    QJSValueList args({pInterpreter->newQObject(this)});
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
    QJSValueList args({pInterpreter->newQObject(nullptr),
                       QJSValue(itemNumber),
                       pInterpreter->newQObject(this),});
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
    QJSValueList args({pInterpreter->newQObject(this),
                       value,
                       itemNumber});
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
    QJSValueList args({pInterpreter->newQObject(this),
                       itemNumber});
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
    QJSValueList args({pInterpreter->newQObject(nullptr),
                       QJSValue(itemNumber),
                       pInterpreter->newQObject(nullptr)});
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
    pStream << getVersion();
    pStream << m_RuleID;
    m_Variables.serializeObject(pStream);
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

