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
            if (QFile::exists(Settings::getInstance()->getUserPath() + folder + files[i]))
            {
                files[i] = Settings::getInstance()->getUserPath() + folder + files[i];
                CONSOLE_PRINT("adding campaign map: " + Settings::getInstance()->getUserPath() + folder + files[i], GameConsole::eDEBUG);
            }
            else if (QFile::exists(oxygine::Resource::RCC_PREFIX_PATH + folder + files[i]))
            {
                CONSOLE_PRINT("adding campaign map: " + QString(oxygine::Resource::RCC_PREFIX_PATH) + folder + files[i], GameConsole::eDEBUG);
                files[i] = oxygine::Resource::RCC_PREFIX_PATH + folder + files[i];
            }
        }
        addDeveloperMaps(Settings::getInstance()->getUserPath(), folder, files);
        addDeveloperMaps(oxygine::Resource::RCC_PREFIX_PATH, folder, files);
    }
    return CampaignMapInfo(folder, files);
}

void Campaign::addDeveloperMaps(const QString & prefix, const QString & folder, QStringList & files)
{
    if (GameConsole::getDeveloperMode())
    {
        QStringList filter;
        filter << "*.map";
        QDirIterator dirIter(prefix + folder, filter, QDir::Files, QDirIterator::Subdirectories);
        while (dirIter.hasNext())
        {
            dirIter.next();
            QString file = dirIter.fileName();
            if (!files.contains(file))
            {
                CONSOLE_PRINT("adding campaign folder map: " + prefix + folder + file, GameConsole::eDEBUG);
                files.append(prefix + folder + file);
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
                           JsThis::getJsThis(pMap),
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
                       JsThis::getJsThis(pMap)});
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
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(pMap),
                       result});
    pInterpreter->doFunction(Campaign::scriptName, "mapFinished", args);
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
                       JsThis::getJsThis(pMap)});
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
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args({m_jsThis,
                       JsThis::getJsThis(pMap),
                       filePath});
    pInterpreter->doFunction(Campaign::scriptName, "onCampaignMapSelected", args);
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

