#include "qfile.h"
#include "qtextstream.h"

#include "game/gamescript.h"

#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"
#include "coreengine/console.h"

const QString GameScript::m_scriptName = "gameScript";

GameScript::GameScript()
    : QObject()
{
    setObjectName("GameScript");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

GameScript::~GameScript()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    pInterpreter->deleteObject(m_scriptName);
}

void GameScript::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    pStream << m_script;
    pStream << m_scriptFile;
    m_Variables.serializeObject(pStream);
}

void GameScript::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    pStream >> m_script;
    pStream >> m_scriptFile;
    if (!m_script.isEmpty())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        pInterpreter->loadScript(m_script, m_scriptName);
        m_loaded = true;
    }
    m_Variables.deserializeObject(pStream);
}

void GameScript::init()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    if (!m_scriptFile.isEmpty())
    {
        if (QFile::exists(m_scriptFile))
        {
            QFile file(m_scriptFile);
            file.open(QIODevice::ReadOnly);
            QTextStream stream(&file);
            m_script = stream.readAll();
            file.close();
            pInterpreter->loadScript(m_script, m_scriptName);
            m_loaded = true;
        }
        else
        {
            m_scriptFile = "";
            m_script = "";
            pInterpreter->deleteObject(m_scriptName);
            m_loaded = false;
        }
    }
}

QString GameScript::getVictoryInfo()
{
    if (m_loaded)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "getVictoryInfo";
        QJSValue ret = pInterpreter->doFunction(m_scriptName, function1);
        if (ret.isString())
        {
            return ret.toString();
        }
    }
    return tr("This is a normal game with the following Victory Conditions.");
}

bool GameScript::immediateStart()
{
    if (m_loaded && !Console::getDeveloperMode())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "immediateStart";
        QJSValue ret = pInterpreter->doFunction(m_scriptName, function1);
        if (ret.isBool())
        {
            return ret.toBool();
        }
    }
    return false;
}

bool GameScript::victory(qint32 team)
{
    if (m_loaded && !m_victoryCalled)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValueList args;
        args << team;
        QString function1 = "victory";
        pInterpreter->doFunction(m_scriptName, function1, args);
        m_victoryCalled = true;
        return false;
    }
    else
    {
        return true;
    }
}

void GameScript::gameStart()
{
    if (m_loaded)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "gameStart";
        pInterpreter->doFunction(m_scriptName, function1);
    }
}

void GameScript::actionDone(spGameAction pAction)
{
    if (m_loaded)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "actionDone";
        QJSValueList args;
        QJSValue obj2 = pInterpreter->newQObject(pAction.get());
        args << obj2;
        pInterpreter->doFunction(m_scriptName, function1, args);
    }
}

void GameScript::turnStart(qint32 turn, qint32 player)
{
    if (m_loaded)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "turnStart";
        QJSValueList args;
        args << turn;
        args << player;
        pInterpreter->doFunction(m_scriptName, function1, args);
    }
}

QString GameScript::getScriptFile() const
{
    return m_scriptFile;
}

void GameScript::setScriptFile(const QString &value)
{
    m_scriptFile = value;
}
