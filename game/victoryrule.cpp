#include "coreengine/interpreter.h"
#include "coreengine/gameconsole.h"

#include "game/victoryrule.h"
#include "game/gamemap.h"
#include "game/player.h"

const char* const VictoryRule::checkbox = "checkbox";
const char* const VictoryRule::spinbox = "spinbox";

VictoryRule::VictoryRule(GameMap* pMap)
    : m_pMap{pMap}
{
#ifdef GRAPHICSUPPORT
    setObjectName("VictoryRule");
#endif
    Interpreter::setCppOwnerShip(this);
}

VictoryRule::VictoryRule(QString ruleID, GameMap* pMap)
    : m_RuleID(ruleID),
      m_pMap{pMap}
{
#ifdef GRAPHICSUPPORT
    setObjectName("VictoryRule");
#endif
    init();
}

void VictoryRule::init()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    Interpreter::setCppOwnerShip(this);
    QString function1 = "init";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(m_pMap)});
    pInterpreter->doFunction(m_RuleID, function1, args);
}

QStringList VictoryRule::getRuleType()
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

QString VictoryRule::getRuleName(qint32 itemNumber)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getRuleName";
    QJSValueList args({pInterpreter->newQObject(this),
                       itemNumber,
                       pInterpreter->newQObject(m_pMap)});
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

void VictoryRule::setRuleValue(qint32 value, qint32 itemNumber)
{
    CONSOLE_PRINT("VictoryRule::setRuleValue for " + m_RuleID + " for item " + QString::number(itemNumber) + " and value " + QString::number(value), GameConsole::eDEBUG);
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "setRuleValue";
    QJSValueList args({pInterpreter->newQObject(this),
                       value,
                       itemNumber,
                       pInterpreter->newQObject(m_pMap)});
    pInterpreter->doFunction(m_RuleID, function1, args);
}

qint32 VictoryRule::getInfiniteValue(qint32 itemNumber)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getInfiniteValue";
    QJSValueList args({itemNumber,
                       pInterpreter->newQObject(m_pMap)});
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

qint32 VictoryRule::getDefaultValue(qint32 itemNumber)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getDefaultRuleValue";
    QJSValueList args({itemNumber,
                      pInterpreter->newQObject(m_pMap)});
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

qint32 VictoryRule::getRuleValue(qint32 itemNumber)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getRuleValue";
    QJSValueList args({pInterpreter->newQObject(this),
                      itemNumber,
                      pInterpreter->newQObject(m_pMap)});
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

QString VictoryRule::getRuleDescription(qint32 itemNumber)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getRuleDescription";
    QJSValueList args({pInterpreter->newQObject(this),
                       itemNumber,
                       pInterpreter->newQObject(m_pMap)});
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

qint32 VictoryRule::getRuleProgress(Player* pPlayer)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getRuleProgress";
    QJSValueList args({pInterpreter->newQObject(this),
                       pInterpreter->newQObject(pPlayer),
                       pInterpreter->newQObject(m_pMap)});
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

void VictoryRule::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    pStream << m_RuleID;
    m_Variables.serializeObject(pStream);
}

void VictoryRule::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    pStream >> m_RuleID;
    m_Variables.deserializeObject(pStream);
}

void VictoryRule::checkDefeat()
{
    CONSOLE_PRINT("VictoryRule::checkDefeat for " + m_RuleID, GameConsole::eDEBUG);
    Interpreter* pInterpreter = Interpreter::getInstance();
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); i++)
    {
        Player* pPlayer = m_pMap->getPlayer(i);
        if (!pPlayer->getIsDefeated())
        {
            QString function1 = "checkDefeat";
            QJSValueList args({pInterpreter->newQObject(this),
                               pInterpreter->newQObject(pPlayer),
                               pInterpreter->newQObject(m_pMap)});
            QJSValue erg = pInterpreter->doFunction(m_RuleID, function1, args);
            if (erg.isNumber())
            {
                GameEnums::DefeatType type = static_cast<GameEnums::DefeatType>(erg.toNumber());
                switch (type)
                {
                    case GameEnums::DefeatType_Alive:
                    {
                        break;
                    }
                    case GameEnums::DefeatType_Defeated:
                    {
                        CONSOLE_PRINT("Defeating player caused by rule " + m_RuleID, GameConsole::eDEBUG);
                        pPlayer->defeatPlayer(nullptr);
                        break;
                    }
                    case GameEnums::DefeatType_ByCurrentPlayer:
                    {
                        CONSOLE_PRINT("Defeating player and moving ownership of buildings to current player. Caused by rule " + m_RuleID, GameConsole::eDEBUG);
                        pPlayer->defeatPlayer(m_pMap->getCurrentPlayer());
                        break;
                    }
                    case GameEnums::DefeatType_Domination:
                    {
                        CONSOLE_PRINT("Defeating player and moving ownership of units and buildings to current player. Caused by rule " + m_RuleID, GameConsole::eDEBUG);
                        pPlayer->defeatPlayer(m_pMap->getCurrentPlayer(), true);
                        break;
                    }
                }
            }
        }
    }
}

GameMap *VictoryRule::getMap() const
{
    return m_pMap;
}

QString VictoryRule::getRuleID() const
{
    return m_RuleID;
}

