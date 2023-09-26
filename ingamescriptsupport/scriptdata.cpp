#include "ingamescriptsupport/scriptdata.h"

#include "coreengine/gameconsole.h"
#include "coreengine/interpreter.h"

const char* const ScriptData::victoryInfo = "victoryInfo";
const char* const ScriptData::immediateStart = "immediateStart";
const char* const ScriptData::victory = "victory";
const char* const ScriptData::turnStart = "turnStart";
const char* const ScriptData::scriptStart = "scriptStart";
const char* const ScriptData::scriptEnd = "scriptEnd";
const char* const ScriptData::actionConditions = "actionConditions";
const char* const ScriptData::campaignVariables = "campaignVariables";
const char* const ScriptData::variables = "variables";

quint32 ScriptData::m_variableCounter = 0;

ScriptData::ScriptData(GameMap* pMap)
    : m_pMap(pMap)
{
    Interpreter::setCppOwnerShip(this);
#ifdef GRAPHICSUPPORT
    setObjectName("ScriptData");
#endif
}

void ScriptData::clearData()
{
    m_Victory.clear();
    m_DayConditions.clear();
    m_ActionConditions.clear();
    m_customCode = "";
    m_customStartOfTurnCode = "";
    m_customActionConditions = "";
    m_customVictoryCode = "";
}

bool ScriptData::getStartMode() const
{
    return m_startMode;
}

void ScriptData::setStartMode(bool value)
{
    m_startMode = value;
}

QString ScriptData::getVariableName()
{
    m_variableCounter++;
    return "variable" + QString::number(m_variableCounter);
}

void ScriptData::readScript(QTextStream& rStream)
{
    CONSOLE_PRINT("ScriptData::readScript", GameConsole::eDEBUG);
    clearData();
    bool started = false;
    while (!rStream.atEnd())
    {
        QString orgLine = rStream.readLine();
        QString line = orgLine;
        if (line.endsWith(scriptStart) && started == false)
        {
            started = true;
        }
        else if (started)
        {
            if (line.endsWith(victoryInfo))
            {
                CONSOLE_PRINT("Reading victory info", GameConsole::eDEBUG);
                while (!rStream.atEnd())
                {
                    line = rStream.readLine();
                    line = line.simplified();
                    if (line.endsWith(victoryInfo))
                    {
                        break;
                    }
                }
                CONSOLE_PRINT("Read victory info", GameConsole::eDEBUG);
            }
            else if (line.endsWith(immediateStart))
            {
                CONSOLE_PRINT("Reading immediate start", GameConsole::eDEBUG);
                while (!rStream.atEnd())
                {
                    line = rStream.readLine();
                    line = line.simplified();
                    if (line.endsWith("return true;"))
                    {
                        m_startMode = true;
                    }
                    else if (line.endsWith("return false;"))
                    {
                        m_startMode = false;
                    }
                    if (line.endsWith(immediateStart))
                    {
                        break;
                    }
                }
                CONSOLE_PRINT("Read immediate start", GameConsole::eDEBUG);
            }
            else if (line.endsWith(victory))
            {
                CONSOLE_PRINT("Reading victory code", GameConsole::eDEBUG);
                readData(victory, rStream, m_customVictoryCode, &m_Victory);
                CONSOLE_PRINT("Read victory code", GameConsole::eDEBUG);
            }
            else if (line.endsWith(turnStart))
            {
                CONSOLE_PRINT("Reading turn start code", GameConsole::eDEBUG);
                readData(turnStart, rStream, m_customStartOfTurnCode, &m_DayConditions);
                CONSOLE_PRINT("Read turn start code", GameConsole::eDEBUG);
            }
            else if (line.endsWith(actionConditions))
            {
                CONSOLE_PRINT("Reading action code", GameConsole::eDEBUG);
                readData(actionConditions, rStream, m_customActionConditions, &m_ActionConditions);
                CONSOLE_PRINT("Read action code", GameConsole::eDEBUG);
            }
            else if (line.endsWith(scriptEnd))
            {
                CONSOLE_PRINT("Reading script end", GameConsole::eDEBUG);
                break;
            }
            else
            {
                CONSOLE_PRINT("Reading custom code", GameConsole::eDEBUG);
                m_customCode += orgLine + "\n";
            }
        }
    }
    if (rStream.status() != QTextStream::Ok)
    {
        CONSOLE_PRINT("Error occured while reading ScriptData", GameConsole::eERROR);
    }
}

void ScriptData::readData(QString id, QTextStream& rStream, QString& customCode, QVector<spScriptCondition>* rVector)
{
    while (!rStream.atEnd())
    {
        QString orgLine = rStream.readLine();
        QString line = orgLine;
        QString trimmedLine = line.simplified();
        if (trimmedLine.endsWith(id))
        {
            CONSOLE_PRINT("Read end for " + id, GameConsole::eDEBUG);
            break;
        }
        if (line.endsWith("precondition"))
        {
            CONSOLE_PRINT("Reading precondition", GameConsole::eDEBUG);
            while (!rStream.atEnd())
            {
                QString line = rStream.readLine();
                line = line.simplified();
                if (line.endsWith("preconditionend"))
                {
                    CONSOLE_PRINT("Read precondition", GameConsole::eDEBUG);
                    break;
                }
            }
            continue;
        }
        CONSOLE_PRINT("Adding new top condition", GameConsole::eDEBUG);
        spScriptCondition pCondition = ScriptCondition::createReadCondition(m_pMap, rStream, line);
        if (pCondition.get() != nullptr)
        {
            rVector->append(pCondition);
        }
        else
        {
            customCode += orgLine + "\n";
        }
    }
}

void ScriptData::writeScript(QTextStream& rStream)
{
    CONSOLE_PRINT("ScriptData::writeScript", GameConsole::eDEBUG);
    m_variableCounter = 0;
    rStream << "var Constructor = function() { // " + QString(scriptStart) + "\n";
    rStream << "    this.immediateStart = function(map) { // " + QString(immediateStart) + "\n";
    rStream << "        return " + QVariant(m_startMode).toString() +  ";\n";
    rStream << "    }; // " + QString(immediateStart) + "\n";

    rStream << "    this.getVictoryInfo = function(map) // " + QString(victoryInfo) + "\n";
    rStream << "    {\n";
    rStream << "        var " << variables << " = map.getGameScript().getVariables();\n";
    rStream << "        var textData = " << variables << ".createVariable(\"victory_info\");\n";
    rStream << "        return textData.readDataString();\n";
    rStream << "    }; // " + QString(victoryInfo) + "\n";

    // victory
    rStream << "    this.victory = function(team, map) { // " + QString(victory) + "\n";
    rStream << "    // precondition\n";
    rStream << "        var " << variables << " = map.getGameScript().getVariables();\n";
    rStream << "        var " << campaignVariables << ";\n";
    rStream << "        if (map.getCampaign() !== null){\n";
    rStream << "            var " << campaignVariables << " = map.getCampaign().getVariables();\n";
    rStream << "        }\n";
    for (qint32 i = 0; i < m_Victory.size(); i++)
    {
        m_Victory[i]->writePreCondition(rStream);
    }
    rStream << "    // preconditionend\n";
    for (qint32 i = 0; i < m_Victory.size(); i++)
    {
        m_Victory[i]->writeCondition(rStream);
    }
    rStream << m_customVictoryCode;
    rStream << "    }; // " + QString(victory) + "\n";


    // turn start
    rStream << "    this.turnStart = function(turn, player, map) { // " + QString(turnStart) + "\n";
    rStream << "    // precondition\n";
    rStream << "        var " << variables << " = map.getGameScript().getVariables();\n";
    rStream << "        var " << campaignVariables << ";\n";
    rStream << "        if (map.getCampaign() !== null){\n";
    rStream << "            var " << campaignVariables << " = map.getCampaign().getVariables();\n";
    rStream << "        }\n";
    for (qint32 i = 0; i < m_DayConditions.size(); i++)
    {
        m_DayConditions[i]->writePreCondition(rStream);
    }
    rStream << "    // preconditionend\n";
    for (qint32 i = 0; i < m_DayConditions.size(); i++)
    {
        m_DayConditions[i]->writeCondition(rStream);
    }
    rStream << m_customStartOfTurnCode;
    rStream << "    }; // " + QString(turnStart) + "\n";

    // action conditions
    rStream << "    this.actionDone = function(action, map) { // " + QString(actionConditions) + "\n";
    rStream << "    // precondition\n";
    rStream << "        var " << variables << " = map.getGameScript().getVariables();\n";
    rStream << "        var " << campaignVariables << ";\n";
    rStream << "        if (map.getCampaign() !== null){\n";
    rStream << "            var " << campaignVariables << " = map.getCampaign().getVariables();\n";
    rStream << "        }\n";
    for (qint32 i = 0; i < m_ActionConditions.size(); i++)
    {
        m_ActionConditions[i]->writePreCondition(rStream);
    }
    rStream << "    // preconditionend\n";
    for (qint32 i = 0; i < m_ActionConditions.size(); i++)
    {
        m_ActionConditions[i]->writeCondition(rStream);
    }
    rStream << m_customActionConditions;
    rStream << "    }; // " + QString(actionConditions) + "\n";

    rStream << m_customCode;
    rStream << "// " + QString(scriptEnd) + "\n};\n";
    rStream << "Constructor.prototype = BASEGAMESCRIPT;\n";
    rStream << "var gameScript = new Constructor();\n";
}

void ScriptData::removeCondition(spScriptCondition condition)
{
    removeCondition(m_Victory, condition);
    removeCondition(m_DayConditions, condition);
    removeCondition(m_ActionConditions, condition);
}

void ScriptData::removeCondition(QVector<spScriptCondition>& data, spScriptCondition condition)
{
    for (qint32 i = 0; i < data.size(); i++)
    {
        if (data[i].get() == condition.get())
        {
            data.removeAt(i);
            break;
        }
        else
        {
            spScriptCondition parent = data[i];
            spScriptCondition cond = data[i]->getSubCondition();
            while (cond.get() != nullptr)
            {
                if (cond == condition)
                {
                    parent->setSubCondition(spScriptCondition());
                    break;
                }
                else
                {
                    parent = cond;
                    cond = parent->getSubCondition();
                }
            }
        }
    }
}

spScriptCondition ScriptData::addVictoryCondition(spScriptCondition condition)
{
    m_Victory.append(condition);
    return condition;
}

spScriptCondition ScriptData::addDayCondition(spScriptCondition condition)
{
    m_DayConditions.append(condition);
    return condition;
}

spScriptCondition ScriptData::addActionCondition(spScriptCondition condition)
{
    m_ActionConditions.append(condition);
    return condition;
}

spScriptCondition ScriptData::getVictoryCondition(qint32 index)
{
    if (index >= 0 && index < m_Victory.size())
    {
        return m_Victory[index];
    }
    return spScriptCondition();
}

spScriptCondition ScriptData::getDayCondition(qint32 index)
{
    if (index >= 0 && index < m_DayConditions.size())
    {
        return m_DayConditions[index];
    }
    return spScriptCondition();
}

spScriptCondition ScriptData::getActionCondition(qint32 index)
{
    if (index >= 0 && index < m_ActionConditions.size())
    {
        return m_ActionConditions[index];
    }
    return spScriptCondition();
}
