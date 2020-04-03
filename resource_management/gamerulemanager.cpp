#include "resource_management/gamerulemanager.h"

GameRuleManager::GameRuleManager()
    : RessourceManagement<GameRuleManager>("", "")
{
}

void GameRuleManager::reset()
{
    RessourceManagement<GameRuleManager>::reset(m_loadedWeather);
    RessourceManagement<GameRuleManager>::reset(m_loadedVictoryRules);
}

void GameRuleManager::loadAll()
{
    reset();
    scriptPath = "/scripts/gamerules/victory";
    RessourceManagement<GameRuleManager>::loadAll(m_loadedVictoryRules);
    scriptPath = "/scripts/gamerules/weather";
    RessourceManagement<GameRuleManager>::loadAll(m_loadedWeather);
    m_loadedVictoryRules.sort();
    m_loadedWeather.sort();
}
