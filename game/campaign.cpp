#include <qfile.h>
#include <qtextstream.h>
#include <qdir.h>
#include <qdiriterator.h>

#include "game/campaign.h"
#include "game/gamemap.h"

#include "coreengine/mainapp.h"
#include "coreengine/console.h"

const QString Campaign::scriptName = "campaignScript";

Campaign::Campaign(QString file)
    : QObject()
{
    setObjectName("Campaign");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    m_scriptFile = file;
    init();
}

Campaign::Campaign()
    : QObject()
{
    setObjectName("Campaign");
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

Campaign::~Campaign()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    pInterpreter->deleteObject(scriptName);
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
            pInterpreter->deleteObject(scriptName);
            m_loaded = false;
        }
    }
}

std::tuple<QString, QStringList> Campaign::getCampaignMaps()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args;
    QJSValue obj = pInterpreter->newQObject(this);
    args << obj;
    QJSValue value = pInterpreter->doFunction(Campaign::scriptName, "getCurrentCampaignMaps", args);
    QStringList files = value.toVariant().toStringList();
    QString folder = "";
    if (files.size() > 0)
    {
        folder = files[0];
        files.removeAt(0);
        addDeveloperMaps(folder, files);
    }
    return std::tuple<QString, QStringList>(folder, files);
}

void Campaign::addDeveloperMaps(QString & folder, QStringList & files)
{
    if (Console::getDeveloperMode())
    {
        QStringList filter;
        filter << "*.map";
        QDirIterator dirIter(folder, filter, QDir::Files, QDirIterator::Subdirectories);
        while (dirIter.hasNext())
        {
            dirIter.next();
            QString file = dirIter.fileInfo().fileName();
            if (!files.contains(file))
            {
                files.append(file);
            }
        }
    }
}

QStringList Campaign::getSelectableCOs(GameMap* pMap, qint32 player, quint8 coIdx)
{
    QStringList ret;
    if (!Console::getDeveloperMode())
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValueList args;
        QJSValue obj = pInterpreter->newQObject(this);
        args << obj;
        QJSValue obj1 = pInterpreter->newQObject(pMap);
        args << obj1;
        args << player;
        args << coIdx;
        QJSValue value = pInterpreter->doFunction(Campaign::scriptName, "getSelectableCOs", args);
        ret = value.toVariant().toStringList();
    }
    return ret;
}

bool Campaign::getCampaignFinished()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args;
    QJSValue obj = pInterpreter->newQObject(this);
    args << obj;
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

void Campaign::mapFinished(bool result)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args;
    QJSValue obj = pInterpreter->newQObject(this);
    args << obj;
    QJSValue obj1 = pInterpreter->newQObject(GameMap::getInstance());
    args << obj1;
    args << result;
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
void Campaign::serializeObject(QDataStream& pStream) const
{
    pStream << getVersion();
    pStream << m_script;
    pStream << m_scriptFile;
    m_Variables.serializeObject(pStream);
}

void Campaign::deserializeObject(QDataStream& pStream)
{
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
