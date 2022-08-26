#include "resource_management/gamerulemanager.h"

GameRuleManager::GameRuleManager()
    : RessourceManagement<GameRuleManager>("", "")
{
    Interpreter::setCppOwnerShip(this);    
#ifdef GRAPHICSUPPORT
    setObjectName("GameRuleManager");
#endif
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

qint32 GameRuleManager::getDefaultWeatherChance(QString weatherId)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString function1 = "getDefaultWeatherChance";
    QJSValue erg = pInterpreter->doFunction(weatherId, function1);
    if (erg.isNumber())
    {
        return erg.toInt();
    }
    else
    {
        return 0;
    }
}
