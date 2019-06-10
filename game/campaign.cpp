#include "campaign.h"

#include "qfile.h"

#include "qtextstream.h"

#include "coreengine/mainapp.h"

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
