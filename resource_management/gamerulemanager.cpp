#include "resource_management/gamerulemanager.h"

GameRuleManager::GameRuleManager()
    : RessourceManagement<GameRuleManager>("", "")
{
    setObjectName("GameRuleManager");
}

void GameRuleManager::reset()
{
    RessourceManagement<GameRuleManager>::reset(m_loadedWeather);
    RessourceManagement<GameRuleManager>::reset(m_loadedVictoryRules);
    RessourceManagement<GameRuleManager>::reset(m_loadedGameRules);
}

void GameRuleManager::loadAll()
{
    reset();
    m_scriptPath = "/scripts/gamerules/victory";
    RessourceManagement<GameRuleManager>::loadAll(m_loadedVictoryRules);
    m_scriptPath = "/scripts/gamerules/weather";
    RessourceManagement<GameRuleManager>::loadAll(m_loadedWeather);
    m_scriptPath = "/scripts/gamerules/rules";
    RessourceManagement<GameRuleManager>::loadAll(m_loadedGameRules);
    m_loadedVictoryRules.sort();
    m_loadedWeather.sort();
    m_loadedGameRules.sort();
}

