#include <QFile>
#include <QTextStream>

#include "game/gamescript.h"
#include "game/gamemap.h"

#include "coreengine/interpreter.h"
#include "coreengine/gameconsole.h"

#include "menue/basegamemenu.h"

const char* const GameScript::m_scriptName = "gameScript";

GameScript::GameScript(GameMap* pMap)
    : m_pMap(pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("GameScript");
#endif
    Interpreter::setCppOwnerShip(this);
}

GameScript::~GameScript()
{
    if (!m_script.isEmpty())
    {
        CONSOLE_PRINT("Deleting gameScript", GameConsole::eDEBUG);
        Interpreter* pInterpreter = Interpreter::getInstance();
        if (pInterpreter != nullptr)
        {
            pInterpreter->deleteObject(m_scriptName);
        }
    }
}

void GameScript::serializeObject(QDataStream& pStream) const
{
    serializeObject(pStream, false);
}

void GameScript::serializeObject(QDataStream& pStream, bool forHash) const
{
    pStream << getVersion();
    pStream << m_script;
    if (!forHash)
    {
        pStream << m_scriptFile;
    }
    m_Variables.serializeObject(pStream, forHash);
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
        QFile file;
        if (QFile::exists(m_scriptFile))
        {
            file.setFileName(m_scriptFile);
        }
        else if (QFile::exists(Settings::getUserPath() + m_scriptFile))
        {
            file.setFileName(Settings::getUserPath() + m_scriptFile);
        }
        else if (QFile::exists(oxygine::Resource::RCC_PREFIX_PATH + m_scriptFile))
        {
            file.setFileName(oxygine::Resource::RCC_PREFIX_PATH + m_scriptFile);
        }
        if (file.exists())
        {
            CONSOLE_PRINT("Loading map script " + file.fileName(), GameConsole::eDEBUG);
            file.open(QIODevice::ReadOnly);
            QTextStream stream(&file);
            m_script = stream.readAll();
            file.close();
            pInterpreter->loadScript(m_script, m_scriptName);
            m_loaded = true;
        }
        else
        {
            CONSOLE_PRINT("Unable to load map script " + file.fileName(), GameConsole::eWARNING);
            m_scriptFile = "";
            m_script = "";
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

GameMap *GameScript::getMap() const
{
    return m_pMap;
}

bool GameScript::immediateStart()
{
    if (m_loaded && !GameConsole::getDeveloperMode())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "immediateStart";
        QJSValueList args({pInterpreter->newQObject(m_pMap)});
        QJSValue ret = pInterpreter->doFunction(m_scriptName, function1, args);
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
        CONSOLE_PRINT("Game script on victory", GameConsole::eDEBUG);
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValueList args({team,
                           pInterpreter->newQObject(m_pMap)});
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
        CONSOLE_PRINT("Game script on game start", GameConsole::eDEBUG);
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "gameStart";
        QJSValueList args({pInterpreter->newQObject(m_pMap)});
        pInterpreter->doFunction(m_scriptName, function1, args);
    }
}

void GameScript::actionDone(spGameAction & pAction)
{
    if (m_loaded)
    {
        CONSOLE_PRINT("Game script on action done", GameConsole::eDEBUG);
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "actionDone";
        QJSValueList args({pInterpreter->newQObject(pAction.get()),
                           pInterpreter->newQObject(m_pMap)});
        pInterpreter->doFunction(m_scriptName, function1, args);
    }
}

void GameScript::turnStart(qint32 turn, qint32 player)
{
    if (m_loaded)
    {
        CONSOLE_PRINT("Game script on turn start", GameConsole::eDEBUG);
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "turnStart";
        QJSValueList args({turn,
                           player,
                           pInterpreter->newQObject(m_pMap)});
        pInterpreter->doFunction(m_scriptName, function1, args);
    }
}

void GameScript::onGameLoaded(BaseGamemenu* pMenu)
{
    if (m_loaded)
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QString function1 = "onGameLoaded";
        QJSValueList args({pInterpreter->newQObject(pMenu),
                           pInterpreter->newQObject(m_pMap)});
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
