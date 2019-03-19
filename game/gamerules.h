#ifndef GAMERULES_H
#define GAMERULES_H

#include <QObject>

#include "QRandomGenerator"

#include "coreengine/fileserializable.h"

#include "game/victoryrule.h"

#include "game/weather.h"

#include "oxygine-framework.h"

class GameRules;
typedef oxygine::intrusive_ptr<GameRules> spGameRules;

/**
 * @brief The GameRules class
 * this class holds the general rules on how the game is played.
 * There is a small amount of rules that's hard coded in this object.
 * But most rules are taken from javascript scripts
 */
class GameRules : public QObject, public FileSerializable, public oxygine::ref_counter
{
    Q_OBJECT
public:
    explicit GameRules();
    virtual ~GameRules() = default;
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serialize(QDataStream& pStream) override;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserialize(QDataStream& pStream) override;
    /**
     * @brief getVersion version of the file
     * @return
     */
    inline virtual qint32 getVersion() override
    {
        return 1;
    }
    void addVictoryRule(spVictoryRule rule);
signals:
    void signalVictory(qint32 team);
public slots:
    /**
     * @brief checkVictory checks for defeats of a player and for the win of a team;
     */
    void checkVictory();
    void addVictoryRule(QString rule);
    void removeVictoryRule(QString rule);
    /**
     * @brief addWeather
     * @param weatherId
     * @param weatherChance
     */
    void addWeather(QString weatherId, qint32 weatherChance);
    /**
     * @brief changeWeatherChance
     * @param weatherId
     * @param weatherChance
     */
    void changeWeatherChance(QString weatherId, qint32 weatherChance);
    /**
     * @brief getCurrentWeather
     * @return
     */
    inline Weather* getCurrentWeather()
    {
        return m_Weathers[m_CurrentWeather].get();
    }
    /**
     * @brief startOfTurn
     */
    void startOfTurn();
    /**
     * @brief changeWeather
     * @param weatherId
     * @param duration
     */
    void changeWeather(QString weatherId, qint32 duration);
private:
    QVector<spVictoryRule> m_VictoryRules;

    // weather chances
    QVector<spWeather> m_Weathers;
    QVector<qint32> m_WeatherChances;
    qint32 m_weatherDuration{0};
    qint32 m_CurrentWeather{0};
    QRandomGenerator randInt;
    qint32 randCounter{0};
};

#endif // GAMERULES_H
