#ifndef GAMERULEMANAGER_H
#define GAMERULEMANAGER_H

#include "resource_management/ressourcemanagement.h"

class GameRuleManager : public QObject, public RessourceManagement<GameRuleManager>
{
    Q_OBJECT
public:
    /**
     * @brief loadAll
     */
    virtual void loadAll() override;
    /**
     * @brief reset
     */
    virtual void reset() override;
    /**
     * @brief getVictoryRuleID
     * @param position the position in the loaded victory list
     * @return the id of the victory rule at the given index
     */
    inline QString getVictoryRuleID(qint32 position)
    {
        if ((position >= 0) && (position < m_loadedVictoryRules.size()))
        {
            return m_loadedVictoryRules.at(position);
        }
        return "";
    }
    /**
     * @brief getVictoryRuleCount
     * @return the amount of victory conditions loaded at the current time
     */
    inline qint32 getVictoryRuleCount()
    {
        return m_loadedVictoryRules.size();
    }
    /**
     * @brief getWeatherID
     * @param position the position in the loaded wearther list
     * @return the id of the weather at the given index
     */
    inline QString getWeatherID(qint32 position)
    {
        if ((position >= 0) && (position < m_loadedWeather.size()))
        {
            return m_loadedWeather.at(position);
        }
        return "";
    }
    /**
     * @brief getWeatherCount
     * @return the amount of weathers loaded at the current time
     */
    inline qint32 getWeatherCount()
    {
        return m_loadedWeather.size();
    }
    /**
     * @brief existsWeather
     * @param weatherID
     * @return
     */
    inline bool existsWeather(QString weatherID)
    {
        return m_loadedWeather.contains(weatherID);
    }
    /**
     * @brief existsVictoryRule
     * @param ruleID
     * @return
     */
    inline bool existsVictoryRule(QString ruleID)
    {
        return m_loadedVictoryRules.contains(ruleID);
    }

    /**
     * @brief getGameRuleID
     * @param position
     * @return
     */
    inline QString getGameRuleID(qint32 position)
    {
        if ((position >= 0) && (position < m_loadedGameRules.size()))
        {
            return m_loadedGameRules.at(position);
        }
        return "";
    }
    /**
     * @brief getGameRuleCount
     * @return
     */
    inline qint32 getGameRuleCount()
    {
        return m_loadedGameRules.size();
    }
    /**
     * @brief existsGameRule
     * @param ruleID
     * @return
     */
    inline bool existsGameRule(QString ruleID)
    {
        return m_loadedGameRules.contains(ruleID);
    }
protected:
    friend RessourceManagement<GameRuleManager>;
    GameRuleManager();

private:
    virtual ~GameRuleManager() = default;
    QStringList m_loadedWeather;
    QStringList m_loadedVictoryRules;
    QStringList m_loadedGameRules;
};

#endif // TERRAINMANAGER_H
