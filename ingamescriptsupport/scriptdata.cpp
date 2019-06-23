#include "scriptdata.h"

const QString ScriptData::immediateStart = "immediateStart";
const QString ScriptData::victory = "victory";
const QString ScriptData::turnStart = "turnStart";
const QString ScriptData::scriptStart = "scriptStart";
const QString ScriptData::scriptEnd = "scriptEnd";

ScriptData::ScriptData()
    : QObject()
{
}

void ScriptData::clearData()
{
    m_Victory.clear();
    m_DayConditions.clear();
    customCode = "";
    customStartOfTurnCode = "";
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

void ScriptData::readScript(QTextStream& rStream)
{
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
            if (line.endsWith(immediateStart))
            {
                while (!rStream.atEnd())
                {
                    line = rStream.readLine().simplified();
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
            }
            else if (line.endsWith(victory))
            {
                readData(victory, rStream, customVictoryCode, &m_Victory);
            }
            else if (line.endsWith(turnStart))
            {
                readData(turnStart, rStream, customStartOfTurnCode, &m_DayConditions);
            }
            else if (line.endsWith(scriptEnd))
            {
                break;
            }
            else
            {
                customCode += line;
            }
        }
    }
}

void ScriptData::readData(QString id, QTextStream& rStream, QString& customCode, QVector<spScriptCondition>* rVector)
{
    while (!rStream.atEnd())
    {
        qint64 pos = rStream.pos();
        QString line = rStream.readLine();
        QString trimmedLine = line.simplified();
        if (trimmedLine.endsWith(id))
        {
            break;
        }
        rStream.seek(pos);
        ScriptCondition* pCondition = ScriptCondition::createReadCondition(rStream);
        if (pCondition != nullptr)
        {
            rVector->append(pCondition);
        }
        else
        {
            line = rStream.readLine();
            customCode += line + "\n";
        }
    }
}

void ScriptData::writeScript(QTextStream& rStream)
{
    rStream << "var Constructor = function() { // " + scriptStart + "\n";
    rStream << "    this.immediateStart = function() { // " + immediateStart + "\n";
    rStream << "        return " + QVariant(startMode).toString() +  ";\n";
    rStream << "    }; // " + immediateStart + "\n";

    rStream << "    this.victory = function(team) { // " + victory + "\n";
    for (qint32 i = 0; i < m_Victory.size(); i++)
    {
        m_Victory[i]->writeCondition(rStream);
    }
    rStream << customVictoryCode;
    rStream << "    }; // " + victory + "\n";

    rStream << "    this.turnStart = function(turn, player) { // " + turnStart + "\n";
    for (qint32 i = 0; i < m_DayConditions.size(); i++)
    {
        m_DayConditions[i]->writeCondition(rStream);
    }
    rStream << customStartOfTurnCode;
    rStream << "    }; // " + turnStart + "\n";

    rStream << customCode;
    rStream << "// " + scriptEnd + "\n};\n";
    rStream << "Constructor.prototype = BASEGAMESCRIPT;\n";
    rStream << "var gameScript = new Constructor();\n";
}

void ScriptData::removeCondition(spScriptCondition condition)
{
    for (qint32 i = 0; i < m_Victory.size(); i++)
    {
        if (m_Victory[i].get() == condition.get())
        {
            m_Victory.removeAt(i);
            break;
        }
    }
    for (qint32 i = 0; i < m_DayConditions.size(); i++)
    {
        if (m_DayConditions[i].get() == condition.get())
        {
            m_DayConditions.removeAt(i);
            break;
        }
    }
}

spScriptCondition ScriptData::addVictoryCondition()
{
    spScriptCondition condition = ScriptCondition::createCondition(ScriptCondition::ConditionType::victory);
    m_Victory.append(condition);
    return condition;
}

spScriptCondition ScriptData::addDayCondition(ScriptCondition::ConditionType type)
{
    spScriptCondition condition = ScriptCondition::createCondition(type);
    m_DayConditions.append(condition);
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
