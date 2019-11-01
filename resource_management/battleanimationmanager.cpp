#include "battleanimationmanager.h"

#include <QFileInfo>

#include <QDirIterator>
#include "coreengine/mainapp.h"

BattleAnimationManager* BattleAnimationManager::m_pInstance = nullptr;


BattleAnimationManager::BattleAnimationManager()
{
    oxygine::Resources::loadXML("resources/images/battleanimations/res.xml");
    Mainapp* pMainapp = Mainapp::getInstance();
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        if (QFile::exists(pMainapp->getSettings()->getMods().at(i) + "/images/battleanimations/res.xml"))
        {
            oxygine::Resources::loadXML(pMainapp->getSettings()->getMods().at(i) + "/images/battleanimations/res.xml");
        }
    }
}

BattleAnimationManager* BattleAnimationManager::getInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new BattleAnimationManager();
    }
    return m_pInstance;
}

void BattleAnimationManager::loadAll()
{
    reset();
    Mainapp* pMainapp = Mainapp::getInstance();
    QStringList searchPaths;
    searchPaths.append("resources/scripts/battleanimations");
    // make sure to overwrite existing js stuff
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        searchPaths.append(pMainapp->getSettings()->getMods().at(i) + "/scripts/battleanimations");
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
            if (!m_loadedBattleAnimations.contains(file.toUpper()))
            {
                loadBattleAnimations(file.toUpper());
            }
        }
    }
}


bool BattleAnimationManager::loadBattleAnimations(const QString& battleAnimationsID)
{
    Mainapp* pMainapp = Mainapp::getInstance();

    QStringList searchPaths;
    searchPaths.append("resources/scripts/battleanimations");
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        searchPaths.append(pMainapp->getSettings()->getMods().at(i) + "/scripts/battleanimations");
    }
    bool bRet = false;
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString file = searchPaths[i] + "/" + battleAnimationsID + ".js";
        QFileInfo checkFile(file);
        if (checkFile.exists() && checkFile.isFile())
        {
            pMainapp->getInterpreter()->openScript(file);
            if (!bRet)
            {
                m_loadedBattleAnimations.append(battleAnimationsID);
            }
            bRet = true;
        }
    }
    return bRet;
}

qint32 BattleAnimationManager::getBattleAnimationsIndex(QString id)
{
    for (qint32 i = 0; i < m_loadedBattleAnimations.size(); i++)
    {
        if (m_loadedBattleAnimations[i] == id)
        {
            return i;
        }
    }
    return -1;
}

void BattleAnimationManager::reset()
{
    Mainapp* pMainapp = Mainapp::getInstance();
    for (qint32 i = 0; i < m_loadedBattleAnimations.size(); i++)
    {
        pMainapp->getInterpreter()->deleteObject(m_loadedBattleAnimations[i]);
    }
    m_loadedBattleAnimations.clear();
}

bool BattleAnimationManager::existsBattleAnimations(QString battleAnimationsID)
{
    for (qint32 i = 0; i < m_loadedBattleAnimations.size(); i++)
    {
        if (m_loadedBattleAnimations[i] == battleAnimationsID)
        {
            return true;
        }
    }
    return false;
}
