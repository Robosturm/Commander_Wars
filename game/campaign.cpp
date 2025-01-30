#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QDirIterator>

#include "game/campaign.h"
#include "game/gamemap.h"
#include "game/jsData/campaignmapdata.h"

#include "coreengine/interpreter.h"
#include "coreengine/gameconsole.h"
#include "coreengine/settings.h"
#include "coreengine/mainapp.h"
#include "coreengine/virtualpaths.h"

const char* const Campaign::scriptName = "campaignScript";

Campaign::Campaign(QString file)
{
#ifdef GRAPHICSUPPORT
    setObjectName("Campaign");
#endif
    Interpreter::setCppOwnerShip(this);
    setupJsThis(this);
    m_scriptFile = file;
    init();
}

Campaign::Campaign()
{
#ifdef GRAPHICSUPPORT
    setObjectName("Campaign");
#endif
    Interpreter::setCppOwnerShip(this);
    setupJsThis(this);
}

void Campaign::init()
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
            pInterpreter->loadScript(m_script, scriptName);
            m_loaded = true;
        }
        else
        {
            m_scriptFile = "";
            m_script = "";
            m_loaded = false;
        }
    }
}

Campaign::CampaignMapInfo Campaign::getCampaignMaps()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({m_jsThis});
    QJSValue value = pInterpreter->doFunction(Campaign::scriptName, "getCurrentCampaignMaps", args);
    QStringList files = value.toVariant().toStringList();
    QString folder = "";
    if (files.size() > 0)
    {
        folder = files[0];
        files.removeAt(0);
        for (qint32 i = 0; i < files.size(); ++i)
        {
            QString path = VirtualPaths::find(folder + files[i]);
            if (QFile::exists(path))
            {
                files[i] = path;
                CONSOLE_PRINT("adding campaign map: " + path, GameConsole::eDEBUG);
            }
        }

        QStringList searchPath = VirtualPaths::createSearchPath(folder, false);
        for (qint32 i = 0; i < searchPath.size(); i++)
        {
            addDeveloperMaps(searchPath[i], files);
        }
    }
    return CampaignMapInfo(folder, files);
}

void Campaign::addDeveloperMaps(const QString & prefix, QStringList & files)
{
    if (GameConsole::getDeveloperMode())
    {
        QStringList filter;
        filter << "*.map";
        QDirIterator dirIter(prefix, filter, QDir::Files, QDirIterator::Subdirectories);
        while (dirIter.hasNext())
        {
            dirIter.next();
            QString file = dirIter.fileName();
            if (!files.contains(file))
            {
                CONSOLE_PRINT("adding campaign folder map: " + prefix + file, GameConsole::eDEBUG);
                files.append(prefix + file);
            }
        }
    }
}

QStringList Campaign::getSelectableCOs(GameMap* pMap, qint32 player, quint8 coIdx)
{
    QStringList ret;
    if (!GameConsole::getDeveloperMode())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValueList args({m_jsThis,
                           GameMap::getMapJsThis(pMap),
                           player,
                           coIdx,});
        QJSValue value = pInterpreter->doFunction(Campaign::scriptName, "getSelectableCOs", args);
        ret = value.toVariant().toStringList();
    }
    return ret;
}

bool Campaign::getAllowArmyCustomization(GameMap* pMap)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({m_jsThis,
                       GameMap::getMapJsThis(pMap)});
    QJSValue value = pInterpreter->doFunction(Campaign::scriptName, "getAllowArmyCustomization", args);
    if (value.isBool())
    {
        return value.toBool();
    }
    return false;
}

bool Campaign::getCampaignFinished()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({m_jsThis});
    QJSValue value = pInterpreter->doFunction(Campaign::scriptName, "getCampaignFinished", args);
    if (value.isBool())
    {
        return value.toBool();
    }
    else
    {
        return false;
    }
}

void Campaign::mapFinished(GameMap* pMap, bool result)
{
    Mainapp::getInstance()->pauseRendering();
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({m_jsThis,
                       GameMap::getMapJsThis(pMap),
                       result});
    pInterpreter->doFunction(Campaign::scriptName, "mapFinished", args);
    Mainapp::getInstance()->continueRendering();
}

QString Campaign::getAuthor()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue value = pInterpreter->doFunction(Campaign::scriptName, "getAuthor");
    if (value.isString())
    {
        return value.toString();
    }
    return "";
}
QString Campaign::getName()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue value = pInterpreter->doFunction(Campaign::scriptName, "getCampaignName");
    if (value.isString())
    {
        return value.toString();
    }
    return "";
}
QString Campaign::getDescription()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue value = pInterpreter->doFunction(Campaign::scriptName, "getDescription");
    if (value.isString())
    {
        return value.toString();
    }
    return "";
}
bool Campaign::getAutoSelectPlayerColors(GameMap* pMap)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({m_jsThis,
                       GameMap::getMapJsThis(pMap)});
    QJSValue value = pInterpreter->doFunction(Campaign::scriptName, "getAutoSelectPlayerColors", args);
    if (value.isBool())
    {
        return value.toBool();
    }
    return false;
}

bool Campaign::getUsesCampaignMap()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({m_jsThis});
    QJSValue value = pInterpreter->doFunction(Campaign::scriptName, "getUsesCampaignMap", args);
    if (value.isBool())
    {
        return value.toBool();
    }
    return false;
}

void Campaign::getCampaignMapData(CampaignMapData & pCampaignMapData)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({m_jsThis,
                       pInterpreter->newQObject(&pCampaignMapData)});
    pInterpreter->doFunction(Campaign::scriptName, "getCampaignMapData", args);
}

void Campaign::onCampaignMapSelected(GameMap* pMap, const QString & filePath)
{
    Mainapp::getInstance()->pauseRendering();
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({m_jsThis,
                       GameMap::getMapJsThis(pMap),
                       filePath});
    pInterpreter->doFunction(Campaign::scriptName, "onCampaignMapSelected", args);
    Mainapp::getInstance()->continueRendering();
}

void Campaign::serializeObject(QDataStream& pStream) const
{
    serializeObject(pStream, false);
}

void Campaign::serializeObject(QDataStream& pStream, bool forHash) const
{
    CONSOLE_PRINT("Campaign::serializeObject", GameConsole::eDEBUG);
    pStream << getVersion();
    pStream << m_script;
    if (!forHash)
    {
        pStream << m_scriptFile;
    }
    m_Variables.serializeObject(pStream, forHash);
}

void Campaign::deserializeObject(QDataStream& pStream)
{
    CONSOLE_PRINT("Campaign::deserializeObject", GameConsole::eDEBUG);
    qint32 version = 0;
    pStream >> version;
    pStream >> m_script;
    pStream >> m_scriptFile;
    if (!m_script.isEmpty())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        pInterpreter->loadScript(m_script, scriptName);
        m_loaded = true;
    }
    m_Variables.deserializeObject(pStream);

}

