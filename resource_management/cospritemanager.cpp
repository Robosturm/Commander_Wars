#include <QFileInfo>

#include <QDirIterator>
#include "cospritemanager.h"
#include "coreengine/mainapp.h"

COSpriteManager* COSpriteManager::m_pInstance = nullptr;

COSpriteManager::COSpriteManager()
{
    Mainapp* pMainapp = Mainapp::getInstance();
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        oxygine::Resources::loadXML(QString(pMainapp->getSettings()->getMods().at(i) + "/images/co/res.xml").toStdString());
    }
    oxygine::Resources::loadXML("resources/images/co/res.xml");
}

COSpriteManager* COSpriteManager::getInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new COSpriteManager();
    }
    return m_pInstance;
}

void COSpriteManager::loadAll()
{
    reset();
    Mainapp* pMainapp = Mainapp::getInstance();
    QStringList searchPaths;
    searchPaths.append("resources/scripts/cos");
    // make sure to overwrite existing js stuff
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        searchPaths.append(pMainapp->getSettings()->getMods().at(i) + "/scripts/cos");
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
            loadCO(file.toUpper());
        }
    }
}


bool COSpriteManager::loadCO(const QString& coID)
{
    Mainapp* pMainapp = Mainapp::getInstance();

    QStringList searchPaths;
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        searchPaths.append(pMainapp->getSettings()->getMods().at(i) + "/scripts/cos");
    }
    searchPaths.append("resources/scripts/cos");
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString file = searchPaths[i] + "/" + coID + ".js";
        QFileInfo checkFile(file);
        if (checkFile.exists() && checkFile.isFile())
        {
            pMainapp->getInterpreter()->openScript(file);
            m_loadedCOs.append(coID);
            return true;
        }
    }
    return false;
}

void COSpriteManager::reset()
{
    Mainapp* pMainapp = Mainapp::getInstance();
    for (qint32 i = 0; i < m_loadedCOs.size(); i++)
    {
        pMainapp->getInterpreter()->deleteObject(m_loadedCOs[i]);
    }
    m_loadedCOs.clear();
}

bool COSpriteManager::existsCO(QString coID)
{
    for (qint32 i = 0; i < m_loadedCOs.size(); i++)
    {
        if (m_loadedCOs[i] == coID)
        {
            return true;
        }
    }
    return false;
}
