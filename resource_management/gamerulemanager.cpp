#include "resource_management/gamerulemanager.h"

#include <QFileInfo>

#include <QDirIterator>

#include "coreengine/mainapp.h"

GameRuleManager* GameRuleManager::m_pInstance = nullptr;

GameRuleManager::GameRuleManager()
    : QObject()
{
}

void GameRuleManager::reset()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    for (qint32 i = 0; i < m_loadedVictoryRules.size(); i++)
    {
        pInterpreter->deleteObject(m_loadedVictoryRules[i]);
    }
    m_loadedVictoryRules.clear();
    for (qint32 i = 0; i < m_loadedWeather.size(); i++)
    {
        pInterpreter->deleteObject(m_loadedWeather[i]);
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
    m_loadedVictoryRules.sort();
    m_loadedWeather.sort();
}

bool GameRuleManager::loadVictoryRule(QString victoryID)
{
    Mainapp* pMainapp = Mainapp::getInstance();
    Interpreter* pInterpreter = Interpreter::getInstance();
    QStringList searchPaths;
    searchPaths.append("resources/scripts/gamerules/victory");
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        searchPaths.append(pMainapp->getSettings()->getMods().at(i) + "/scripts/gamerules/victory");
    }    
    bool bRet = false;
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString file = searchPaths[i] + "/" + victoryID + ".js";
        QFileInfo checkFile(file);
        if (checkFile.exists() && checkFile.isFile())
        {
            pInterpreter->openScript(file);
            if (!bRet)
            {
                m_loadedVictoryRules.append(victoryID);
            }
            bRet = true;
        }
    }
    return bRet;
}

bool GameRuleManager::loadWeather(QString weatherID)
{
    Mainapp* pMainapp = Mainapp::getInstance();
    Interpreter* pInterpreter = Interpreter::getInstance();
    QStringList searchPaths;
    searchPaths.append("resources/scripts/gamerules/weather");
    for (qint32 i = 0; i < pMainapp->getSettings()->getMods().size(); i++)
    {
        searchPaths.append(pMainapp->getSettings()->getMods().at(i) + "/scripts/gamerules/weather");
    }    
    bool bRet = false;
    for (qint32 i = 0; i < searchPaths.size(); i++)
    {
        QString file = searchPaths[i] + "/" + weatherID + ".js";
        QFileInfo checkFile(file);
        if (checkFile.exists() && checkFile.isFile())
        {
            pInterpreter->openScript(file);
            if (!bRet)
            {
                m_loadedWeather.append(weatherID);
            }
            bRet = true;
        }
    }
    return bRet;
}

GameRuleManager* GameRuleManager::getInstance()
{
    if (m_pInstance == nullptr)
    {
        m_pInstance = new GameRuleManager();
    }
    return m_pInstance;
}
