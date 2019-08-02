#include <QFileInfo>

#include <QDirIterator>
#include "unitspritemanager.h"
#include "coreengine/mainapp.h"

UnitSpriteManager* UnitSpriteManager::m_pInstance = nullptr;

UnitSpriteManager::UnitSpriteManager()
{
    oxygine::Resources::loadXML("resources/images/units/res.xml");
    Mainapp* pMainapp = Mainapp::getInstance();
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        if (QFile::exists(pMainapp->getSettings()->getMods().at(i) + "/images/units/res.xml"))
        {
            oxygine::Resources::loadXML(QString(pMainapp->getSettings()->getMods().at(i) + "/images/units/res.xml").toStdString());
        }
    }
}

UnitSpriteManager* UnitSpriteManager::getInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new UnitSpriteManager();
    }
    return m_pInstance;
}

void UnitSpriteManager::loadAll()
{
    reset();
    Mainapp* pMainapp = Mainapp::getInstance();
    QStringList searchPaths;
    searchPaths.append("resources/scripts/units");
    // make sure to overwrite existing js stuff
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        searchPaths.append(pMainapp->getSettings()->getMods().at(i) + "/scripts/units");
    }
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString path =  QCoreApplication::applicationDirPath() + "/" + searchPaths[i];
        QStringList filter;
        filter << "*.js";
        QDirIterator* dirIter = new QDirIterator(path, filter, QDir::Files, QDirIterator::Subdirectories);
        while (dirIter->hasNext())
        {
            dirIter->next();
            QString file = dirIter->fileInfo().fileName().split(".").at(0);
            if (!m_loadedUnits.contains(file.toUpper()))
            {
                loadUnit(file.toUpper());
            }
        }
    }
}


bool UnitSpriteManager::loadUnit(const QString& unitID)
{
    Mainapp* pMainapp = Mainapp::getInstance();

    QStringList searchPaths;
    searchPaths.append("resources/scripts/units");
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        searchPaths.append(pMainapp->getSettings()->getMods().at(i) + "/scripts/units");
    }
    bool bRet = false;
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString file = searchPaths[i] + "/" + unitID + ".js";
        QFileInfo checkFile(file);
        if (checkFile.exists() && checkFile.isFile())
        {
            pMainapp->getInterpreter()->openScript(file);
            if (!bRet)
            {
                m_loadedUnits.append(unitID);
            }
            bRet = true;
        }
    }
    return bRet;
}

qint32 UnitSpriteManager::getUnitIndex(QString id)
{
    for (qint32 i = 0; i < m_loadedUnits.size(); i++)
    {
        if (m_loadedUnits[i] == id)
        {
            return i;
        }
    }
    return -1;
}

void UnitSpriteManager::reset()
{
    Mainapp* pMainapp = Mainapp::getInstance();
    for (qint32 i = 0; i < m_loadedUnits.size(); i++)
    {
        pMainapp->getInterpreter()->deleteObject(m_loadedUnits[i]);
    }
    m_loadedUnits.clear();
}

bool UnitSpriteManager::existsUnit(QString unitID)
{
    for (qint32 i = 0; i < m_loadedUnits.size(); i++)
    {
        if (m_loadedUnits[i] == unitID)
        {
            return true;
        }
    }
    return false;
}

QString UnitSpriteManager::getUnitName(qint32 position)
{
    if ((position >= 0) && (position < m_loadedUnits.size()))
    {
        Mainapp* pApp = Mainapp::getInstance();
        Interpreter* pInterpreter = pApp->getInterpreter();
        QJSValue value = pInterpreter->doFunction(m_loadedUnits[position], "getName");
        if (value.isString())
        {
            return value.toString();
        }
    }
    return "";
}
