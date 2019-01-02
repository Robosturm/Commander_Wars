#include "gamemanager.h"

#include "coreengine/mainapp.h"

#include <QFileInfo>

#include <QDirIterator>

GameManager* GameManager::m_pInstance = nullptr;

GameManager::GameManager()
{
    // load terrain
    Mainapp* pMainapp = Mainapp::getInstance();
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        oxygine::Resources::loadXML(QString(pMainapp->getSettings()->getMods().at(i) + "/images/game/res.xml").toStdString());
    }
    oxygine::Resources::loadXML("resources/images/game/res.xml");
}

GameManager* GameManager::getInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new GameManager();
    }
    return m_pInstance;
}

void GameManager::loadAll()
{
    Mainapp* pMainapp = Mainapp::getInstance();
    QStringList searchPaths;
    searchPaths.append("resources/scripts/actions");
    // make sure to overwrite existing js stuff
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        searchPaths.append(pMainapp->getSettings()->getMods().at(i) + "/scripts/actions");
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
            QString file = dirIter->fileInfo().absoluteFilePath();
            pMainapp->getInterpreter()->openScript(file);
        }
    }
}
