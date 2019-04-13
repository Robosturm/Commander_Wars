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
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        if (QFile::exists(pMainapp->getSettings()->getMods().at(i) + "/images/building/res.xml"))
        {
            oxygine::Resources::loadXML(QString(pMainapp->getSettings()->getMods().at(i) + "/images/building/res.xml").toStdString());
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

void BuildingSpriteManager::loadAll()
{
    reset();
    Mainapp* pMainapp = Mainapp::getInstance();
    QStringList searchPaths;
    searchPaths.append("resources/scripts/building");
    // make sure to overwrite existing js stuff
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        searchPaths.append(pMainapp->getSettings()->getMods().at(i) + "/scripts/building");
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
}


bool BuildingSpriteManager::loadBuilding(const QString& buildingID)
{
    Mainapp* pMainapp = Mainapp::getInstance();

    QStringList searchPaths;
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        searchPaths.append(pMainapp->getSettings()->getMods().at(i) + "/scripts/building");
    }
    searchPaths.append("resources/scripts/building");
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString file = searchPaths[i] + "/" + buildingID + ".js";
        QFileInfo checkFile(file);
        if (checkFile.exists() && checkFile.isFile())
        {
            pMainapp->getInterpreter()->openScript(file);
            m_loadedBuildings.append(buildingID);
            return true;
        }
    }
    return false;
}

void BuildingSpriteManager::reset()
{
    Mainapp* pMainapp = Mainapp::getInstance();
    for (qint32 i = 0; i < m_loadedBuildings.size(); i++)
    {
        pMainapp->getInterpreter()->deleteObject(m_loadedBuildings[i]);
    }
    m_loadedBuildings.clear();
}
