#include "resource_management/gamerulemanager.h"

#include <QFileInfo>

#include <QDirIterator>

#include "coreengine/mainapp.h"

GameRuleManager* GameRuleManager::m_pInstance = nullptr;

GameRuleManager::GameRuleManager() : QObject()
{
    oxygine::Resources::loadXML("resources/images/gamerule/res.xml");
    // load rule sprites
    Mainapp* pMainapp = Mainapp::getInstance();
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        if (QFile::exists(pMainapp->getSettings()->getMods().at(i) + "/images/gamerule/res.xml"))
        {
            oxygine::Resources::loadXML(QString(pMainapp->getSettings()->getMods().at(i) + "/images/gamerule/res.xml").toStdString());
        }
    }
}

void GameRuleManager::reset()
{
    Mainapp* pMainapp = Mainapp::getInstance();
    for (qint32 i = 0; i < m_loadedVictoryRules.size(); i++)
    {
        pMainapp->getInterpreter()->deleteObject(m_loadedVictoryRules[i]);
    }
    m_loadedVictoryRules.clear();
    for (qint32 i = 0; i < m_loadedWeather.size(); i++)
    {
        pMainapp->getInterpreter()->deleteObject(m_loadedWeather[i]);
    }
    m_loadedWeather.clear();
}

void GameRuleManager::loadAll()
{
    reset();
    Mainapp* pMainapp = Mainapp::getInstance();
    QStringList searchPaths;
    searchPaths.append("resources/scripts/gamerules");
    // make sure to overwrite existing js stuff
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        searchPaths.append(pMainapp->getSettings()->getMods().at(i) + "/scripts/gamerules");
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
            if (dirIter->fileInfo().filePath().contains("gamerules/victory"))
            {
                if (!m_loadedVictoryRules.contains(file.toUpper()))
                {
                    loadVictoryRule(file.toUpper());
                }
            }
            else if (dirIter->fileInfo().filePath().contains("gamerules/weather"))
            {
                if (!m_loadedWeather.contains(file.toUpper()))
                {
                    loadWeather(file.toUpper());
                }
            }

        }
    }
}

bool GameRuleManager::loadVictoryRule(const QString& victoryID)
{
    Mainapp* pMainapp = Mainapp::getInstance();

    QStringList searchPaths;
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        searchPaths.append(pMainapp->getSettings()->getMods().at(i) + "/scripts/gamerules/victory");
    }
    searchPaths.append("resources/scripts/gamerules/victory");
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString file = searchPaths[i] + "/" + victoryID + ".js";
        QFileInfo checkFile(file);
        if (checkFile.exists() && checkFile.isFile())
        {
            pMainapp->getInterpreter()->openScript(file);
            m_loadedVictoryRules.append(victoryID);
            return true;
        }
    }
    return false;
}

bool GameRuleManager::loadWeather(const QString& weatherID)
{
    Mainapp* pMainapp = Mainapp::getInstance();

    QStringList searchPaths;
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        searchPaths.append(pMainapp->getSettings()->getMods().at(i) + "/scripts/gamerules/weather");
    }
    searchPaths.append("resources/scripts/gamerules/weather");
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString file = searchPaths[i] + "/" + weatherID + ".js";
        QFileInfo checkFile(file);
        if (checkFile.exists() && checkFile.isFile())
        {
            pMainapp->getInterpreter()->openScript(file);
            m_loadedWeather.append(weatherID);
            return true;
        }
    }
    return false;
}

GameRuleManager* GameRuleManager::getInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new GameRuleManager();
    }
    return m_pInstance;
}
