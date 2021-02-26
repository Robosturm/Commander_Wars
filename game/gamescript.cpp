#include "qfile.h"
#include "qtextstream.h"

#include "game/gamescript.h"

#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"

const QString GameScript::scriptName = "gameScript";

GameScript::GameScript()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

GameScript::~GameScript()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    pInterpreter->deleteObject(scriptName);
}

void GameScript::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    pStream << script;
    pStream << scriptFile;
    m_Variables.serializeObject(pStream);
}

void GameScript::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    pStream >> script;
    pStream >> scriptFile;
    if (!script.isEmpty())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        pInterpreter->loadScript(script, scriptName);
        loaded = true;
    }
    m_Variables.deserializeObject(pStream);

}

void GameScript::init()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    if (!scriptFile.isEmpty())
    {
        if (QFile::exists(scriptFile))
        {
            QFile file(scriptFile);
            file.open(QIODevice::ReadOnly);
            QTextStream stream(&file);
            script = stream.readAll();
            file.close();
            pInterpreter->loadScript(script, scriptName);
            loaded = true;
        }
        else
        {
            scriptFile = "";
            script = "";
            pInterpreter->deleteObject(scriptName);
            loaded = false;
        }
    }
}

QString GameScript::getVictoryInfo()
{
    if (loaded)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "getVictoryInfo";
        QJSValue ret = pInterpreter->doFunction(scriptName, function1);
        if (ret.isString())
        {
            return ret.toString();
        }
    }
    return tr("This is a normal game with the following Victory Conditions.");
}

bool GameScript::immediateStart()
{
    if (loaded)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "immediateStart";
        QJSValue ret = pInterpreter->doFunction(scriptName, function1);
        if (ret.isBool())
        {
            return ret.toBool();
        }
    }
    return false;
}

bool GameScript::victory(qint32 team)
{
    if (loaded && !victoryCalled)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValueList args;
        args << team;
        QString function1 = "victory";
        pInterpreter->doFunction(scriptName, function1, args);
        victoryCalled = true;
        return false;
    }
    else
    {
        return true;
    }
}

void GameScript::gameStart()
{
    if (loaded)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "gameStart";
        pInterpreter->doFunction(scriptName, function1);
    }
}

void GameScript::actionDone(spGameAction pAction)
{
    if (loaded)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "actionDone";
        QJSValueList args;
        QJSValue obj2 = pInterpreter->newQObject(pAction.get());
        args << obj2;
        pInterpreter->doFunction(scriptName, function1, args);
    }
}

void GameScript::turnStart(qint32 turn, qint32 player)
{
    if (loaded)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "turnStart";
        QJSValueList args;
        args << turn;
        args << player;
        pInterpreter->doFunction(scriptName, function1, args);
    }
}

QString GameScript::getScriptFile() const
{
    return scriptFile;
}

void GameScript::setScriptFile(const QString &value)
{
    scriptFile = value;
}
