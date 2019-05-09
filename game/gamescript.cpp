#include "gamescript.h"
#include "qfile.h"
#include "qtextstream.h"

#include "coreengine/mainapp.h"

const QString GameScript::scriptName = "gameScript";

GameScript::GameScript()
    : QObject()
{
}

GameScript::~GameScript()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->getInterpreter()->deleteObject(scriptName);
}

void GameScript::serializeObject(QDataStream& pStream)
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
    if (!scriptFile.isEmpty())
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->getInterpreter()->loadScript(scriptFile, scriptName);
        loaded = true;
    }
    m_Variables.deserializeObject(pStream);

}

void GameScript::init()
{
    Mainapp* pApp = Mainapp::getInstance();
    if (!script.isEmpty())
    {
        if (QFile::exists(script))
        {
            QFile file(script);
            file.open(QIODevice::ReadOnly);
            QTextStream stream(&file);
            scriptFile = stream.readAll();
            file.close();
            pApp->getInterpreter()->loadScript(scriptFile, scriptName);
            loaded = true;
        }
        else
        {
            scriptFile = "";
            pApp->getInterpreter()->deleteObject(scriptName);
            loaded = false;
        }
    }
}

bool GameScript::immediateStart()
{
    if (loaded)
    {
        Mainapp* pApp = Mainapp::getInstance();
        QString function1 = "immediateStart";
        QJSValue ret = pApp->getInterpreter()->doFunction(scriptName, function1);
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
        Mainapp* pApp = Mainapp::getInstance();
        QJSValueList args;
        args << team;
        QString function1 = "victory";
        pApp->getInterpreter()->doFunction(scriptName, function1, args);
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
        Mainapp* pApp = Mainapp::getInstance();
        QString function1 = "gameStart";
        pApp->getInterpreter()->doFunction(scriptName, function1);
    }
}

void GameScript::actionDone()
{
    if (loaded)
    {
        Mainapp* pApp = Mainapp::getInstance();
        QString function1 = "actionDone";
        pApp->getInterpreter()->doFunction(scriptName, function1);
    }
}
