#ifndef GAMERULEMANAGER_H
#define GAMERULEMANAGER_H

#include <QObject>

class GameRuleManager : public QObject
{
    Q_OBJECT
public:
    static GameRuleManager* getInstance();

signals:

public slots:
    void loadAll();





    /**
     * @brief loadVictoryRule
     * @param victoryID the victory that should be loaded.
     * @return true if all is fine
     */
    bool loadVictoryRule(QString victoryID);
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
     * @brief loadWeather
     * @param weatherID the weather that should be loaded.
     * @return true if all is fine
     */
    bool loadWeather(QString weatherID);
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

    void reset();
private:
    QStringList m_loadedWeather;
    QStringList m_loadedVictoryRules;
    explicit GameRuleManager();
    virtual ~GameRuleManager() = default;
    static GameRuleManager* m_pInstance;
};



#endif // TERRAINMANAGER_H
