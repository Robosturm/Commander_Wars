#include "ingamescriptsupport/scriptdata.h"

#include "coreengine/console.h"

const QString ScriptData::victoryInfo = "victoryInfo";
const QString ScriptData::immediateStart = "immediateStart";
const QString ScriptData::victory = "victory";
const QString ScriptData::turnStart = "turnStart";
const QString ScriptData::scriptStart = "scriptStart";
const QString ScriptData::scriptEnd = "scriptEnd";
const QString ScriptData::actionConditions = "actionConditions";

quint32 ScriptData::m_variableCounter = 0;

ScriptData::ScriptData()
    : QObject()
{
}

void ScriptData::clearData()
{
    m_Victory.clear();
    m_DayConditions.clear();
    m_ActionConditions.clear();
    customCode = "";
    customStartOfTurnCode = "";
    customActionConditions = "";
    customVictoryCode = "";
}

bool ScriptData::getStartMode() const
{
    return startMode;
}

void ScriptData::setStartMode(bool value)
{
    startMode = value;
}

QString ScriptData::getVariableName()
{
    m_variableCounter++;
    return "variable" + QString::number(m_variableCounter);
}

void ScriptData::readScript(QTextStream& rStream)
{
    Console::print("ScriptData::readScript", Console::eDEBUG);
    clearData();
    bool started = false;
    while (!rStream.atEnd())
    {
        QString line = rStream.readLine();
        if (line.endsWith(scriptStart) && started == false)
        {
            started = true;
        }
        else if (started)
        {
            if (line.endsWith(victoryInfo))
            {
                Console::print("Reading victory info", Console::eDEBUG);
                while (!rStream.atEnd())
                {
                    line = rStream.readLine();
                    line = line.simplified();
                    if (line.endsWith(victoryInfo))
                    {
                        break;
                    }
                }
                Console::print("Read victory info", Console::eDEBUG);
            }
            else if (line.endsWith(immediateStart))
            {
                Console::print("Reading immediate start", Console::eDEBUG);
                while (!rStream.atEnd())
                {
                    line = rStream.readLine();
                    line = line.simplified();
                    if (line.endsWith("return true;"))
                    {
                        startMode = true;
                    }
                    else if (line.endsWith("return false;"))
                    {
                        startMode = false;
                    }
                    if (line.endsWith(immediateStart))
                    {
                        break;
                    }
                }
                Console::print("Read immediate start", Console::eDEBUG);
            }
            else if (line.endsWith(victory))
            {
                Console::print("Reading victory code", Console::eDEBUG);
                readData(victory, rStream, customVictoryCode, &m_Victory);
                Console::print("Read victory code", Console::eDEBUG);
            }
            else if (line.endsWith(turnStart))
            {
                Console::print("Reading turn start code", Console::eDEBUG);
                readData(turnStart, rStream, customStartOfTurnCode, &m_DayConditions);
                Console::print("Read turn start code", Console::eDEBUG);
            }
            else if (line.endsWith(actionConditions))
            {
                Console::print("Reading action code", Console::eDEBUG);
                readData(actionConditions, rStream, customActionConditions, &m_ActionConditions);
                Console::print("Read action code", Console::eDEBUG);
            }
            else if (line.endsWith(scriptEnd))
            {
                Console::print("Reading script end", Console::eDEBUG);
                break;
            }
            else
            {
                Console::print("Reading custom code", Console::eDEBUG);
                customCode += line;
            }
        }
    }
    if (rStream.status() != QTextStream::Ok)
    {
        Console::print("Error occured while reading ScriptData", Console::eERROR);
    }
}

void ScriptData::readData(QString id, QTextStream& rStream, QString& customCode, QVector<spScriptCondition>* rVector)
{
    while (!rStream.atEnd())
    {
        QString line = rStream.readLine();
        QString trimmedLine = line.simplified();
        if (trimmedLine.endsWith(id))
        {
            Console::print("Read end for " + id, Console::eDEBUG);
            break;
        }
        if (line.endsWith("precondition"))
        {
            Console::print("Reading precondition", Console::eDEBUG);
            while (!rStream.atEnd())
            {
                QString line = rStream.readLine();
                line = line.simplified();
                if (line.endsWith("preconditionend"))
                {
                    Console::print("Read precondition", Console::eDEBUG);
                    break;
                }
            }
            continue;
        }
        spScriptCondition pCondition = ScriptCondition::createReadCondition(rStream, line);
        if (pCondition.get() != nullptr)
        {
            rVector->append(pCondition);
        }
        else
        {
            customCode += line + "\n";
        }
    }
}

void ScriptData::writeScript(QTextStream& rStream)
{
    Console::print("ScriptData::writeScript", Console::eDEBUG);
    m_variableCounter = 0;
    rStream << "var Constructor = function() { // " + scriptStart + "\n";
    rStream << "    this.immediateStart = function() { // " + immediateStart + "\n";
    rStream << "        return " + QVariant(startMode).toString() +  ";\n";
    rStream << "    }; // " + immediateStart + "\n";

    rStream << "    this.getVictoryInfo = function() // " + victoryInfo + "\n";
    rStream << "    {\n";
    rStream << "        var " << variables << " = map.getGameScript().getVariables();\n";
    rStream << "        var textData = " << variables << ".createVariable(\"victory_info\");\n";
    rStream << "        return textData.readDataString();\n";
    rStream << "    }; // " + victoryInfo + "\n";

    // victory
    rStream << "    this.victory = function(team) { // " + victory + "\n";
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
    rStream << customVictoryCode;
    rStream << "    }; // " + victory + "\n";


    // turn start
    rStream << "    this.turnStart = function(turn, player) { // " + turnStart + "\n";
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
    rStream << customStartOfTurnCode;
    rStream << "    }; // " + turnStart + "\n";

    // action conditions
    rStream << "    this.actionDone = function(action) { // " + actionConditions + "\n";
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
    rStream << customActionConditions;
    rStream << "    }; // " + actionConditions + "\n";

    rStream << customCode;
    rStream << "// " + scriptEnd + "\n};\n";
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
                    parent->setSubCondition(nullptr);
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
    return nullptr;
}

spScriptCondition ScriptData::getDayCondition(qint32 index)
{
    if (index >= 0 && index < m_DayConditions.size())
    {
        return m_DayConditions[index];
    }
    return nullptr;
}

spScriptCondition ScriptData::getActionCondition(qint32 index)
{
    if (index >= 0 && index < m_ActionConditions.size())
    {
        return m_ActionConditions[index];
    }
    return nullptr;
}
