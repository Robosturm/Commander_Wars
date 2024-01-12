#pragma once

#include <QObject>
#include <QJsonObject>

/**
 * @brief This class is meant to read the content of an awbw replay file and create map states based on them in order to train an ai
 */
class AwbwReplayerReader : public QObject
{
    Q_OBJECT

public:

    struct CoInfo
    {
        qint32 coId{0};
        qint32 maxPower{0};
        qint32 maxSuperPower{0};
        qint32 power{0};
    };
    struct PlayerInfo
    {
        qint32 playerIdx{0};
        qint32 playerId{0};
        qint32 userId{0};
        QString team;
        qint32 countryId{0};
        CoInfo coData;
        CoInfo tagCoData;
        qint32 funds{0};
        bool eliminated{false};
        qint32 turnOrder{0};
        bool drawAccepted{false};
        QString powerOn; // "N", "Y" or "S"
        QString coImage;
    };
    struct BuildingInfo
    {
        qint32 ownerId{0};
        qint32 terrainId{0};
        qint32 x{0};
        qint32 y{0};
        qint32 capture{0};
        qint32 lastCapture{0};

    };
    struct UnitInfo
    {
        qint32 unitId{0};
        qint32 playerId{0};
        QString name;
        qint32 movementPoints{0};
        qint32 vision{0};
        qint32 fuel{0};
        qint32 fuelPerTurn{0};
        QString stealthed;
        qint32 ammo{0};
        qint32 minRange{0};
        qint32 maxRange{0};
        bool hasSecondWeapon{false};
        qint32 cost{0};
        QString movementType;
        qint32 x{0};
        qint32 y{0};
        qint32 moved{0};
        qint32 capture{0};
        qint32 fired{0};
        float hp{0.0f};
        qint32 loadedUnitId1{0};
        qint32 loadedUnitId2{0};
        bool carried{false};
    };
    struct GameState
    {
        qint64 mapId{0};
        qint32 fundsPerBuilding{0};
        qint32 startingFunds{0};
        QString weatherType;
        bool fog{false};
        bool usePowers{false};
        qint32 turn{0};
        qint32 day{0};
        QString currentWeather;
        qint32 weatherStart{0};
        QVector<PlayerInfo> players;
        QVector<BuildingInfo> buildings;
        QVector<UnitInfo> units;
    };
    struct ActionInfo
    {
        qint32 playerId{0};
        qint32 day{0};
        QVector<QJsonObject> actionData;
    };
    explicit AwbwReplayerReader() = default;
    void parseReplay(const QString & awbwReplayZip);
    /**
     * @brief getValid
     * @return
     */
    bool getValid() const;
    /**
     * @brief getGameStates
     * @return
     */
    const QVector<GameState> & getGameStates() const;
    /**
     * @brief getActions
     * @return
     */
    const QVector<ActionInfo> & getActions() const;

private:
    void parseGameStates(QList<QByteArray> & data);
    void readPlayers(QTextStream & stream, GameState & newState);
    void readBuildings(QTextStream & stream, GameState & newState);
    void readUnits(QTextStream & stream, GameState & newState);
    void parseActions(QList<QByteArray> & data);
private:
    QVector<GameState> m_gameStates;
    QVector<ActionInfo> m_actions;
    bool m_valid{true};
};
