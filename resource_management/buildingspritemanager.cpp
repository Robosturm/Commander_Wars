#include "buildingspritemanager.h"

#include "coreengine/mainapp.h"

#include <QFileInfo>

#include <QDirIterator>

BuildingSpriteManager* BuildingSpriteManager::m_pInstance = nullptr;

BuildingSpriteManager::BuildingSpriteManager()
{
    // load terrain
    oxygine::Resources::loadXML("resources/images/building/res.xml");
    Mainapp* pMainapp = Mainapp::getInstance();
    for (qint32 i = 0; i < Settings::getMods().size(); i++)
    {
        if (QFile::exists(Settings::getMods().at(i) + "/images/building/res.xml"))
        {
            oxygine::Resources::loadXML(Settings::getMods().at(i) + "/images/building/res.xml");
        }
    }
}

BuildingSpriteManager* BuildingSpriteManager::getInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new BuildingSpriteManager();
    }
    return m_pInstance;
}

qint32 BuildingSpriteManager::getBuildingIndex(QString id)
{
    for (qint32 i = 0; i < m_loadedBuildings.size(); i++)
    {
        if (m_loadedBuildings[i] == id)
        {
            return i;
        }
    }
    return -1;
}

void BuildingSpriteManager::loadAll()
{
    reset();
    Mainapp* pMainapp = Mainapp::getInstance();
    QStringList searchPaths;
    searchPaths.append("resources/scripts/building");
    // make sure to overwrite existing js stuff
    for (qint32 i = 0; i < Settings::getMods().size(); i++)
    {
        searchPaths.append(Settings::getMods().at(i) + "/scripts/building");
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
            if (!m_loadedBuildings.contains(file.toUpper()))
            {
                loadBuilding(file.toUpper());
            }
        }
    }
    m_loadedBuildings.sort();
}


bool BuildingSpriteManager::loadBuilding(QString buildingID)
{
    Mainapp* pMainapp = Mainapp::getInstance();
    Interpreter* pInterpreter = Interpreter::getInstance();
    QStringList searchPaths;
    searchPaths.append("resources/scripts/building");
    for (qint32 i = 0; i < Settings::getMods().size(); i++)
    {
        searchPaths.append(Settings::getMods().at(i) + "/scripts/building");
    }    
    bool bRet = false;
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString file = searchPaths[i] + "/" + buildingID + ".js";
        QFileInfo checkFile(file);
        if (checkFile.exists() && checkFile.isFile())
        {
            pInterpreter->openScript(file);
            if (!bRet)
            {
                m_loadedBuildings.append(buildingID);
            }
            bRet = true;
        }
    }
    return bRet;
}

void BuildingSpriteManager::reset()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    for (qint32 i = 0; i < m_loadedBuildings.size(); i++)
    {
        pInterpreter->deleteObject(m_loadedBuildings[i]);
    }
    m_loadedBuildings.clear();
}

QString BuildingSpriteManager::getBuildingName(qint32 position)
{
    if ((position >= 0) && (position < m_loadedBuildings.size()))
    {
        Interpreter* pInterpreter = Interpreter::getInstance();
        QJSValue value = pInterpreter->doFunction(m_loadedBuildings[position], "getName");
        if (value.isString())
        {
            return value.toString();
        }
    }
    return "";
}

bool BuildingSpriteManager::existsBuilding(QString buildingID)
{
    for (qint32 i = 0; i < m_loadedBuildings.size(); i++)
    {
        if (m_loadedBuildings[i] == buildingID)
        {
            return true;
        }
    }
    return false;
}
