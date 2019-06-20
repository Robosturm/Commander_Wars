#include "campaign.h"

#include "qfile.h"

#include "qtextstream.h"

#include "coreengine/mainapp.h"

#include "game/gamemap.h"

const QString Campaign::scriptName = "campaignScript";

Campaign::Campaign(QString file)
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    scriptFile = file;
    init();
}

Campaign::Campaign()
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
}

Campaign::~Campaign()
{
    Mainapp* pApp = Mainapp::getInstance();
    pApp->getInterpreter()->deleteObject(scriptName);
}

void Campaign::init()
{
    Mainapp* pApp = Mainapp::getInstance();
    if (!scriptFile.isEmpty())
    {
        if (QFile::exists(scriptFile))
        {
            QFile file(scriptFile);
            file.open(QIODevice::ReadOnly);
            QTextStream stream(&file);
            script = stream.readAll();
            file.close();
            pApp->getInterpreter()->loadScript(script, scriptName);
            loaded = true;
        }
        else
        {
            scriptFile = "";
            script = "";
            pApp->getInterpreter()->deleteObject(scriptName);
            loaded = false;
        }
    }
}

std::tuple<QString, QStringList> Campaign::getCampaignMaps()
{
    Interpreter* pInterpreter = Mainapp::getInstance()->getInterpreter();
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
    }
    return std::tuple<QString, QStringList>(folder, files);
}

QStringList Campaign::getSelectableCOs(GameMap* pMap, qint32 player, quint8 coIdx)
{
    Interpreter* pInterpreter = Mainapp::getInstance()->getInterpreter();
    QJSValueList args;
    QJSValue obj = pInterpreter->newQObject(this);
    args << obj;
    QJSValue obj1 = pInterpreter->newQObject(pMap);
    args << obj1;
    args << player;
    args << coIdx;
    QJSValue value = pInterpreter->doFunction(Campaign::scriptName, "getSelectableCOs", args);
    return value.toVariant().toStringList();
}

bool Campaign::getCampaignFinished()
{
    Interpreter* pInterpreter = Mainapp::getInstance()->getInterpreter();
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
    Interpreter* pInterpreter = Mainapp::getInstance()->getInterpreter();
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
    Interpreter* pInterpreter = Mainapp::getInstance()->getInterpreter();
    QJSValue value = pInterpreter->doFunction(Campaign::scriptName, "getAuthor");
    if (value.isString())
    {
        return value.toString();
    }
    return "";
}
QString Campaign::getName()
{
    Interpreter* pInterpreter = Mainapp::getInstance()->getInterpreter();
    QJSValue value = pInterpreter->doFunction(Campaign::scriptName, "getCampaignName");
    if (value.isString())
    {
        return value.toString();
    }
    return "";
}
QString Campaign::getDescription()
{
    Interpreter* pInterpreter = Mainapp::getInstance()->getInterpreter();
    QJSValue value = pInterpreter->doFunction(Campaign::scriptName, "getDescription");
    if (value.isString())
    {
        return value.toString();
    }
    return "";
}
void Campaign::serializeObject(QDataStream& pStream)
{
    pStream << getVersion();
    pStream << script;
    pStream << scriptFile;
    m_Variables.serializeObject(pStream);
}

void Campaign::deserializeObject(QDataStream& pStream)
{
    qint32 version = 0;
    pStream >> version;
    pStream >> script;
    pStream >> scriptFile;
    if (!script.isEmpty())
    {
        Mainapp* pApp = Mainapp::getInstance();
        pApp->getInterpreter()->loadScript(script, scriptName);
        loaded = true;
    }
    m_Variables.deserializeObject(pStream);

}
