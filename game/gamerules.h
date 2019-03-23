#ifndef GAMERULES_H
#define GAMERULES_H

#include <QObject>

#include "coreengine/fileserializable.h"

#include "game/victoryrule.h"

#include "game/weather.h"

#include "game/GameEnums.h"

#include "oxygine-framework.h"

class Player;
class Unit;

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
    /**
     * @brief createWeatherSprites
     */
    void createWeatherSprites();

signals:
    void signalVictory(qint32 team);
public slots:
    /**
     * @brief setStartWeather
     * @param index
     * @param duration
     */
    void setStartWeather(qint32 index);
    /**
     * @brief getStartWeather
     * @return
     */
    qint32 getStartWeather() const;
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
     * @brief changeWeatherChance
     * @param index
     * @param weatherChance
     */
    void changeWeatherChance(qint32 index, qint32 weatherChance);
    /**
     * @brief getCurrentWeather
     * @return
     */
    inline Weather* getCurrentWeather()
    {
        if (m_CurrentWeather >= 0)
        {
            return m_Weathers[m_CurrentWeather].get();
        }
        return nullptr;
    }
    inline qint32 getCurrentWeatherId()
    {
        return m_CurrentWeather;
    }
    /**
     * @brief getWeather
     * @param index
     * @return
     */
    Weather* getWeather(qint32 index);
    /**
     * @brief getCurrentWeather
     * @return
     */
    Weather* getWeather(QString weatherId);
    /**
     * @brief getWeatherChance
     * @param index
     * @return
     */
    qint32 getWeatherChance(QString weatherId);
    /**
     * @brief getWeatherCount
     * @return
     */
    inline qint32 getWeatherCount()
    {
        return m_Weathers.size();
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

    bool getRankingSystem() const;
    void setRankingSystem(bool RankingSystem);

    bool getNoPower() const;
    void setNoPower(bool NoPower);

    qint32 getUnitLimit() const;
    void setUnitLimit(const qint32 &UnitLimit);

    bool getRandomWeather() const;
    void setRandomWeather(bool randomWeather);

    GameEnums::Fog getFogMode() const;
    void setFogMode(const GameEnums::Fog &FogMode);
    /**
     * @brief createFogVision
     */
    void createFogVision();
    /**
     * @brief showHideStealthUnit
     * @param pPlayer
     * @param pUnit
     */
    void showHideStealthUnit(Player* pPlayer, Unit* pUnit);
private:
    QVector<spVictoryRule> m_VictoryRules;

    // weather chances
    QVector<spWeather> m_Weathers;
    QVector<qint32> m_WeatherChances;
    qint32 m_weatherDuration{0};
    qint32 m_CurrentWeather{-1};
    qint32 m_StartWeather{-1};
    bool m_randomWeather{true};

    bool m_RankingSystem{true};
    bool m_NoPower{false};
    GameEnums::Fog m_FogMode{GameEnums::Fog_OfWar};
    qint32 m_UnitLimit{0};

    QVector<QVector<oxygine::spSprite>> m_FogSprites;
    QVector<oxygine::spSprite> m_WeatherSprites;
};

#endif // GAMERULES_H
