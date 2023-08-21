#ifndef GAMERULES_H
#define GAMERULES_H

#include <QObject>

#include "coreengine/fileserializable.h"
#include "coreengine/timer.h"

#include "game/victoryrule.h"
#include "game/gamerule.h"
#include "game/weather.h"
#include "game/GameEnums.h"

#include "multiplayer/password.h"

class Player;
class Unit;
class GameMap;

class GameRules;
using spGameRules = std::shared_ptr<GameRules>;

/**
 * @brief The GameRules class
 * this class holds the general rules on how the game is played.
 * There is a small amount of rules that's hard coded in this object.
 * But most rules are taken from javascript scripts
 */
class GameRules final : public QObject, public FileSerializable
{
    Q_OBJECT
public:
    enum class DayToDayScreen
    {
        Default,
        Permanent
    };

    explicit GameRules(GameMap* pMap);
    ~GameRules() = default;
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& pStream) const override;
    /**
     * @brief serializeObject
     * @param pStream
     * @param forHash
     */
    void serializeObject(QDataStream& pStream, bool forHash) const;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& pStream) override;
    /**
     * @brief deserializer
     * @param pStream
     * @param fast
     */
    void deserializer(QDataStream& pStream, bool fast);
    /**
     * @brief getVersion version of the file
     * @return
     */
    inline virtual qint32 getVersion() const override
    {
        return 29;
    }
    void addVictoryRule(spVictoryRule rule);
    /**
     * @brief addGameRule
     * @param rule
     */
    void addGameRule(spGameRule rule);
    /**
     * @brief createWeatherSprites
     */
    void createWeatherSprites();

    Timer* getRoundTimer()
    {
        return &m_RoundTimer;
    }
    /**
     * @brief init
     */
    void init();

    DayToDayScreen getDayToDayScreen() const;
    void setDayToDayScreen(const DayToDayScreen &DayToDayScreen);
    /**
     * @brief getObserverList
     * @return
     */
    QVector<quint64> &getObserverList();
    /**
     * @brief onGameStart
     */
    void onGameStart();
    /**
     * @brief getPassword
     * @return
     */
    const Password & getPassword() const;
    spGameRule getSpGameRule(const QString & rule);
    spVictoryRule getSpVictoryRule(const QString & rule);
signals:
    void sigVictory(qint32 team);
public:
    /**
     * @brief getParallelCos
     * @return
     */
    Q_INVOKABLE bool getParallelCos() const;
    /**
     * @brief setParallelCos
     * @param newParallelCos
     */
    Q_INVOKABLE void setParallelCos(bool newParallelCos);
    /**
     * @brief getDamageFormula
     * @return
     */
    Q_INVOKABLE GameEnums::DamageFormula getDamageFormula() const;
    /**
     * @brief setDamageFormula
     * @param newDamageFormula
     */
    Q_INVOKABLE void setDamageFormula(const GameEnums::DamageFormula newDamageFormula);
    /**
     * @brief getMapPalette
     * @return
     */
    Q_INVOKABLE qint32 getMapPalette() const;
    /**
     * @brief setMapPalette
     * @param newMapPalette
     */
    Q_INVOKABLE void setMapPalette(qint32 newMapPalette);
    /**
     * @brief getMatchType
     * @return
     */
    Q_INVOKABLE QString getMatchType() const;
    /**
     * @brief setMatchType
     * @param newMatchType
     */
    Q_INVOKABLE void setMatchType(const QString & newMatchType);
    /**
     * @brief reset
     */
    Q_INVOKABLE void reset();
    /**
     * @brief resetWeatherSprites
     */
    Q_INVOKABLE void resetWeatherSprites();
    /**
     * @brief resetFogSprites
     */
    Q_INVOKABLE void resetFogSprites();
    /**
     * @brief getMultiplayerObserver
     * @return
     */
    Q_INVOKABLE qint32 getMultiplayerObserver() const;
    /**
     * @brief setMultiplayerObserver
     * @param newMultiplayerObserver
     */
    Q_INVOKABLE void setMultiplayerObserver(qint32 newMultiplayerObserver);
    /**
     * @brief getMap
     * @return
     */
    Q_INVOKABLE GameMap *getMap() const;
    /**
     * @brief getHpDefenseReduction
     * @return
     */
    Q_INVOKABLE bool getHpDefenseReduction() const;
    /**
     * @brief setHpDefenseReduction
     * @param newHpDefenseReduction
     */
    Q_INVOKABLE void setHpDefenseReduction(bool newHpDefenseReduction);
    /**
     * @brief getPowerUsageReduction
     * @return
     */
    Q_INVOKABLE qreal getPowerUsageReduction() const;
    /**
     * @brief setPowerUsageReduction
     * @param newPowerUsageReduction
     */
    Q_INVOKABLE void setPowerUsageReduction(qreal newPowerUsageReduction);
    /**
     * @brief getPowerUsageReduction
     * @return
     */
    Q_INVOKABLE qreal getPowerLoose() const;
    /**
     * @brief setPowerUsageReduction
     * @param newPowerUsageReduction
     */
    Q_INVOKABLE void setPowerLoose(qreal newPowerLoose);
    /**
     * @brief getPowerGainZone
     * @return
     */
    Q_INVOKABLE GameEnums::PowerGainZone getPowerGainZone() const;
    /**
     * @brief setPowerGainZone
     * @param newPowerGainZone
     */
    Q_INVOKABLE void setPowerGainZone(const GameEnums::PowerGainZone newPowerGainZone);
    /**
     * @brief getPowerGainMode
     * @return
     */
    Q_INVOKABLE GameEnums::PowerGainMode getPowerGainMode() const;
    /**
     * @brief setPowerGainMode
     * @param newPowerGainMode
     */
    Q_INVOKABLE void setPowerGainMode(const GameEnums::PowerGainMode newPowerGainMode);
    /**
     * @brief getTransporterRefresh
     * @return
     */
    Q_INVOKABLE bool getTransporterRefresh() const;
    /**
     * @brief setTransporterRefresh
     * @param newTransporterRefresh
     */
    Q_INVOKABLE void setTransporterRefresh(bool newTransporterRefresh);
    /**
     * @brief GameRules::getEnableDayToDayCoAbilities
     * @return
     */
    Q_INVOKABLE bool getEnableDayToDayCoAbilities() const;
    /**
     * @brief setEnableDayToDayCoAbilities
     * @param newEnableDayToDayCoAbilities
     */
    Q_INVOKABLE void setEnableDayToDayCoAbilities(bool newEnableDayToDayCoAbilities);
    /**
     * @brief getShipBridges
     * @return
     */
    Q_INVOKABLE bool getShipBridges() const;
    /**
     * @brief setShipBridges
     * @param newShipBridges
     */
    Q_INVOKABLE void setShipBridges(bool newShipBridges);
    /**
     * @brief getResellValue
     * @return
     */
    Q_INVOKABLE qreal getResellValue() const;
    /**
     * @brief setResellValue
     * @param newResellValue
     */
    Q_INVOKABLE void setResellValue(qreal newResellValue);
    /**
     * @brief getVictory
     * @return
     */
    Q_INVOKABLE bool getVictory() const;
    /**
     * @brief getTerrainDefense
     * @return
     */
    Q_INVOKABLE quint8 getTerrainDefense() const;
    /**
     * @brief setTerrainDefense
     * @param terrainDefense
     */
    Q_INVOKABLE void setTerrainDefense(const quint8 terrainDefense);
    /**
     * @brief getN_cosmeticModsAllowed
     * @return
     */
    Q_INVOKABLE bool getCosmeticModsAllowed() const;
    /**
     * @brief setN_cosmeticModsAllowed
     * @param value
     */
    Q_INVOKABLE void setCosmeticModsAllowed(bool value);
    /**
     * @brief getSingleCo
     * @return
     */
    Q_INVOKABLE bool getSingleCo() const;
    /**
     * @brief setSingleCo
     * @param singleCo
     */
    Q_INVOKABLE void setSingleCo(bool singleCo);
    /**
     * @brief getPassword
     * @return
     */
    Q_INVOKABLE QString getPasswordText() const;
    /**
     * @brief setPassword
     * @param password
     */
    Q_INVOKABLE void setPassword(const QString & password);
    /**
     * @brief getDescription
     * @return
     */
    Q_INVOKABLE QString getDescription() const;
    /**
     * @brief setDescription
     * @param description
     */
    Q_INVOKABLE void setDescription(const QString & description);
    /**
     * @brief getPowerGainSpeed
     * @return
     */
    Q_INVOKABLE qreal getPowerGainSpeed() const;
    /**
     * @brief setPowerGainSpeed
     * @param powerGainSpeed
     */
    Q_INVOKABLE void setPowerGainSpeed(qreal powerGainSpeed);
    /**
     * @brief getAllowedPerks
     * @return
     */
    Q_INVOKABLE QStringList getAllowedPerks() const;
    /**
     * @brief setAllowedPerks
     * @param allowedPerks
     */
    Q_INVOKABLE void setAllowedPerks(const QStringList & allowedPerks);
    /**
     * @brief getAllowedActions
     * @return
     */
    Q_INVOKABLE QStringList getAllowedActions() const;
    /**
     * @brief setAllowedActions
     * @param allowedActions
     */
    Q_INVOKABLE void setAllowedActions(const QStringList & allowedActions);
    /**
     * @brief getTeamFacingUnits
     * @return
     */
    Q_INVOKABLE bool getTeamFacingUnits() const;
    /**
     * @brief setTeamFacingUnits
     * @param teamFacingUnits
     */
    Q_INVOKABLE void setTeamFacingUnits(bool teamFacingUnits);
    /**
     * @brief getSingleRandomCO
     * @return
     */
    Q_INVOKABLE bool getSingleRandomCO() const;
    /**
     * @brief setSingleRandomCO
     * @param singleRandomCO
     */
    Q_INVOKABLE void setSingleRandomCO(bool singleRandomCO);
    /**
     * @brief setMaxPerkCost
     * @return
     */
    Q_INVOKABLE qint32 getMaxPerkCost() const;
    /**
     * @brief setMaxPerkCost
     * @param maxPerkCost
     */
    Q_INVOKABLE void setMaxPerkCost(const qint32 maxPerkCost);
    /**
     * @brief getMaxPerkCount
     * @return
     */
    Q_INVOKABLE qint32 getMaxPerkCount() const;
    /**
     * @brief setMaxPerkCount
     * @param newMaxPerkCount
     */
    Q_INVOKABLE void setMaxPerkCount(qint32 newMaxPerkCount);
    /**
     * @brief getVisionBuildingVisionHide
     * @return
     */
    Q_INVOKABLE bool getBuildingVisionHide() const;
    /**
     * @brief setVisionBuildingVisionHide
     * @param VisionBuildingVisionHide
     */
    Q_INVOKABLE void setBuildingVisionHide(bool VisionBuildingVisionHide);
    /**
     * @brief getVisionBlock
     * @return
     */
    Q_INVOKABLE bool getVisionBlock() const;
    /**
     * @brief setVisionBlock
     * @param VisionBlock
     */
    Q_INVOKABLE void setVisionBlock(bool VisionBlock);
    /**
     * @brief getWeatherPrediction
     * @return
     */
    Q_INVOKABLE bool getWeatherPrediction() const;
    /**
     * @brief setWeatherPrediction
     * @param WeatherPrediction
     */
    Q_INVOKABLE void setWeatherPrediction(bool WeatherPrediction);
    /**
     * @brief getCOBannlist
     * @return
     */
    Q_INVOKABLE QStringList getCOBannlist() const;
    /**
     * @brief setCOBannlist
     * @param COBannlist
     */
    Q_INVOKABLE void setCOBannlist(const QStringList & COBannlist);
    /**
     * @brief getAiAttackTerrain
     * @return
     */
    Q_INVOKABLE bool getAiAttackTerrain() const;
    /**
     * @brief setAiAttackTerrain
     * @param AiAttackTerrain
     */
    Q_INVOKABLE void setAiAttackTerrain(bool AiAttackTerrain);
    /**
     * @brief setStartWeather
     * @param index
     * @param duration
     */
    Q_INVOKABLE void setStartWeather(qint32 index);
    /**
     * @brief getStartWeather
     * @return
     */
    Q_INVOKABLE qint32 getStartWeather() const;
    /**
     * @brief checkVictory checks for defeats of a player and for the win of a team;
     */
    Q_INVOKABLE void checkVictory();
    Q_INVOKABLE qint32 getVictoryTeam();
    Q_INVOKABLE void addVictoryRule(const QString & rule);
    Q_INVOKABLE bool hasVictoryRule(const QString & rule);
    Q_INVOKABLE void removeVictoryRule(const QString & rule);
    /**
     * @brief hasGameRule
     * @param rule
     * @return
     */
    Q_INVOKABLE bool hasGameRule(const QString & rule);
    Q_INVOKABLE VictoryRule* getVictoryRule(const QString & rule);
    Q_INVOKABLE qint32 getVictoryRuleSize()
    {
        return m_VictoryRules.size();
    }
    Q_INVOKABLE VictoryRule* getVictoryRuleAtIndex(qint32 index)
    {
        return m_VictoryRules[index].get();
    }
    Q_INVOKABLE void addGameRule(const QString & rule);
    Q_INVOKABLE GameRule* getGameRule(const QString & rule);
    Q_INVOKABLE void removeGameRule(const QString & rule);
    Q_INVOKABLE qint32 getGameRuleSize()
    {
        return m_GameRules.size();
    }
    Q_INVOKABLE GameRule* getGameRuleAtIndex(qint32 index)
    {
        return m_GameRules[index].get();
    }

    /**
     * @brief addWeather
     * @param weatherId
     * @param weatherChance
     */
    Q_INVOKABLE void addWeather(const QString & weatherId, qint32 weatherChance);
    /**
     * @brief changeWeatherChance
     * @param weatherId
     * @param weatherChance
     */
    Q_INVOKABLE void changeWeatherChanceByName(const QString & weatherId, qint32 weatherChance);
    /**
     * @brief changeWeatherChance
     * @param index
     * @param weatherChance
     */
    Q_INVOKABLE void changeWeatherChance(qint32 index, qint32 weatherChance);
    /**
     * @brief getCurrentWeather
     * @return
     */
    Q_INVOKABLE inline Weather* getCurrentWeather()
    {
        if (m_CurrentWeather >= 0 && m_CurrentWeather < m_Weathers.size())
        {
            return m_Weathers[m_CurrentWeather].get();
        }
        return nullptr;
    }
    /**
     * @brief getWeatherAtDay
     * @param futureDay
     * @param player
     * @return
     */
    Q_INVOKABLE Weather* getWeatherAtDay(qint32 futureDay, qint32 player);
    Q_INVOKABLE inline qint32 getCurrentWeatherId()
    {
        return m_CurrentWeather;
    }
    /**
     * @brief getWeather
     * @param index
     * @return
     */
    Q_INVOKABLE Weather* getWeather(qint32 index);
    /**
     * @brief getCurrentWeather
     * @return
     */
    Q_INVOKABLE Weather* getWeather(const QString & weatherId);
    /**
     * @brief getWeatherChance
     * @param index
     * @return
     */
    Q_INVOKABLE qint32 getWeatherChance(const QString & weatherId);
    /**
     * @brief getWeatherCount
     * @return
     */
    Q_INVOKABLE inline qint32 getWeatherCount()
    {
        return m_Weathers.size();
    }
    /**
     * @brief startOfTurn
     */
    Q_INVOKABLE void startOfTurn(bool newDay);
    /**
     * @brief changeWeather
     * @param weatherId
     * @param duration
     */
    Q_INVOKABLE void changeWeather(const QString & weatherId, qint32 duration, qint32 startDay = 0);
    /**
     * @brief changeWeather changes the weather for n-days
     * @param weatherId
     * @param duration
     */
    Q_INVOKABLE void changeWeather(qint32 weatherId, qint32 duration, qint32 startDay = 0, bool apply = true);
    /**
     * @brief setCurrentWeather sets the current weather
     * @param weatherId
     */
    Q_INVOKABLE void setCurrentWeather(qint32 weatherId);
    /**
     * @brief onWeatherChanged
     */
    Q_INVOKABLE void onWeatherChanged();
    Q_INVOKABLE bool getRankingSystem() const;
    Q_INVOKABLE void setRankingSystem(bool RankingSystem);
    Q_INVOKABLE bool getNoPower() const;
    Q_INVOKABLE void setNoPower(bool NoPower);
    Q_INVOKABLE qint32 getUnitLimit() const;
    Q_INVOKABLE void setUnitLimit(const qint32 UnitLimit);
    Q_INVOKABLE bool getRandomWeather() const;
    Q_INVOKABLE void setRandomWeather(bool randomWeather);
    Q_INVOKABLE GameEnums::Fog getFogMode() const;
    Q_INVOKABLE void setFogMode(const GameEnums::Fog FogMode);
    /**
     * @brief createFogVision
     */
    Q_INVOKABLE void createFogVision();
    /**
     * @brief createFieldFogClear
     * @param x
     * @param y
     * @param pPlayer
     */
    Q_INVOKABLE void createFieldFogClear(qint32 x, qint32 y, Player* pPlayer);
    /**
     * @brief createFieldFogWar
     * @param x
     * @param y
     * @param pPlayer
     */
    Q_INVOKABLE void createFieldFogWar(qint32 x, qint32 y, Player* pPlayer, QColor fogOfWarColor = QColor(70, 70, 70, 100));
    /**
     * @brief createFieldFogShrouded
     * @param x
     * @param y
     * @param pPlayer
     */
    Q_INVOKABLE void createFieldFogShrouded(qint32 x, qint32 y, Player* pPlayer, QColor fogOfMistColor = QColor(180, 180, 180, 100), QColor fogOfWarColor = QColor(70, 70, 70, 100));
    /**
     * @brief createFieldFogMist
     * @param x
     * @param y
     * @param pPlayer
     * @param fogOfMistColor
     */
    Q_INVOKABLE void createFieldFogMist(qint32 x, qint32 y, Player* pPlayer, QColor fogOfMistColor = QColor(180, 180, 180, 100));
    /**
     * @brief showHideStealthUnit
     * @param pPlayer
     * @param pUnit
     */
    Q_INVOKABLE void showHideStealthUnit(Player* pPlayer, Unit* pUnit);
    /**
     * @brief setRoundTimeMs
     * @param timeMs
     */
    Q_INVOKABLE void setRoundTimeMs(qint32 timeMs)
    {
        roundTime = timeMs;
    }
    /**
     * @brief getRoundTimeMs
     * @return
     */
    Q_INVOKABLE qint32 getRoundTimeMs()
    {
        return roundTime;
    }
    /**
     * @brief pauseRoundTime
     */
    Q_INVOKABLE void pauseRoundTime()
    {
        m_RoundTimer.pause();
    }
    /**
     * @brief resumeRoundTime
     */
    Q_INVOKABLE void resumeRoundTime();
    /**
     * @brief startRoundTime
     */
    Q_INVOKABLE void initRoundTime();
    /**
      * @brief getCoUnits
      * @return
      */
    Q_INVOKABLE bool getCoUnits() const;
    /**
     * @brief setCoUnits
     * @param coUnits
     */
    Q_INVOKABLE void setCoUnits(bool coUnits);
private:
    /**
     * @brief resetArrays
     */
    void resetArrays();

private:
    QVector<spGameRule> m_GameRules;
    // victory conditions
    QVector<spVictoryRule> m_VictoryRules;
    // weather chances
    QVector<spWeather> m_Weathers;
    QVector<qint32> m_WeatherChances;
    QVector<QVector<qint32>> m_WeatherDays;
    qint32 m_CurrentWeather{-1};
    qint32 m_StartWeather{-1};
    qint32 m_DayWeather{-1};
    bool m_randomWeather{true};

    bool m_RankingSystem{true};
    bool m_NoPower{false};
    GameEnums::Fog m_FogMode{GameEnums::Fog_Off};
    qint32 m_UnitLimit{0};

    std::vector<std::vector<oxygine::spSprite>> m_FogSprites;
    std::vector<oxygine::spSprite> m_WeatherSprites;
    oxygine::spSprite m_weatherMaster;
    oxygine::spSprite m_fogMaster;

    qint32 roundTime{0};
    Timer m_RoundTimer;

    bool m_AiAttackTerrain{true};
    QStringList m_COBannlist;
    bool m_COBannlistEdited{false};
    bool m_WeatherPrediction{true};
    bool m_coUnits{true};
    bool m_VisionBlock{false};
    bool m_BuildingVisionHide{false};
    bool m_singleRandomCO{false};
    DayToDayScreen m_DayToDayScreen{DayToDayScreen::Default};
    qint32 m_maxPerkCost{4};
    qint32 m_maxPerkCount{4};
    bool m_teamFacingUnits{false};
    bool m_perkBannlistEdited{false};
    QStringList m_allowedPerks;
    bool m_actionBannlistEdited{false};
    bool m_singleCo{false};
    QStringList m_allowedActions;
    qreal m_powerGainSpeed{1.0};
    quint8 m_terrainDefense{10};
    bool m_hpDefenseReduction{true};
    bool m_victory{false};
    qreal m_resellValue{0.5};
    bool m_transporterRefresh{true};
    bool m_shipBridges{true};
    bool m_enableDayToDayCoAbilities{true};
    GameEnums::PowerGainZone m_powerGainZone{GameEnums::PowerGainZone_GlobalCoZoneBonus};
    GameEnums::PowerGainMode m_powerGainMode{GameEnums::PowerGainMode_Money};
    qreal m_powerUsageReduction{0.2};
    qreal m_powerLoose{0.0};
    GameMap* m_pMap{nullptr};
    qint32 m_mapPalette{0};

    GameEnums::DamageFormula m_damageFormula{GameEnums::DamageFormula::DamageFormula_AdvanceWars4};
    bool m_parallelCos{true};

    // multiplayer rule section
    Password m_password;
    QString m_description;
    bool m_cosmeticModsAllowed{false};
    qint32 m_multiplayerObserver{0};
    QVector<quint64> m_observerList;
    QString m_matchType;
};

Q_DECLARE_INTERFACE(GameRules, "GameRules");

#endif // GAMERULES_H
