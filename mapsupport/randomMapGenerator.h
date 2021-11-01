#pragma once

#include <QObject>
#include <QVector>
#include <QString>
#include <QPoint>
#include <QRandomGenerator>

#include <game/GameEnums.h>

class RandomMapGenerator : public QObject
{
    Q_OBJECT
public:
    enum class MirrorMode
    {
        none,
        flip,
        rotate,
        max
    };
    /**
     * @brief randomMap
     * @param width
     * @param heigth
     * @param playerCount
     * @param roadSupport
     * @param seed
     * @param forestchance
     * @param mountainChance
     * @param seachance
     * @param buildingchance
     * @return seed used to create the map
     */
    static qint32 randomMap(qint32 width, qint32 heigth, qint32 playerCount,
                            bool roadSupport, qint32 seed,
                            const QVector<std::tuple<QString, float>> & terrains,
                            const QVector<std::tuple<QString, float>> & buildings,
                            const QVector<float> & ownedBaseSize,
                            float startBaseSize,
                            const QVector<std::tuple<QString, float>> & units,
                            qint32 unitCount,
                            float startBaseUnitSize,
                            const QVector<float> & unitDistribution,
                            bool unitsDistributed,
                            bool mirrored);
private:
    /**
     * @brief placeGroup
     * @param startX
     * @param startY
     * @param count
     * @param terrainID
     * @param terrainRadius
     * @param randInt
     */
    static qint32 randomMapPlaceGroup(qint32 startX, qint32 startY, qint32 count, QString terrainID, qint32 terrainRadius, QRandomGenerator& randInt);
    /**
     * @brief randomMapTerrainPlaceable
     * @param x
     * @param y
     * @param terrainID
     * @return
     */
    static bool randomMapTerrainPlaceable(qint32 x, qint32 y, QString terrainID);
    /**
     * @brief randomMapPlaceLine
     * @param startX
     * @param startY
     * @param count
     * @param terrainID
     * @param randInt
     * @return
     */
    static qint32 randomMapPlaceLine(qint32 startX, qint32 startY, qint32 count, QString terrainID, QRandomGenerator& randInt);
    /**
     * @brief addTerrainPoint
     * @param points
     * @param x
     * @param y
     * @param terrainID
     * @param terrainRadius
     */
    static bool randomMapAddTerrainPoint(QVector<QPoint>& points, qint32 x, qint32 y, QString terrainID, qint32 terrainRadius);
    /**
     * @brief createBuildings
     * @param buildings
     * @param roadSupport
     * @param randInt
     * @param noHarbour
     */
    static QVector<QPoint> randomMapCreateBuildings(qint32 buildings, QRandomGenerator& randInt, const QVector<std::tuple<QString, float>> & buildingDistributions, const QVector<float> & ownedBaseSize,
                                                    float startBaseSize, qint32& progress, qint32 maxProgess);
    /**
     * @brief randomMapCreateBuildings
     * @param buildings
     * @param randInt
     * @param buildingDistributions
     * @param ownedBaseSize
     * @param startBaseSize
     * @param progress
     * @param maxProgess
     * @param mirrorX
     * @param mirrorY
     * @return
     */
    static QVector<QPoint> randomMapCreateBuildings(qint32 width, qint32 heigth, qint32 buildings, QRandomGenerator& randInt, const QVector<std::tuple<QString, float>> & buildingDistributions, const QVector<float> & ownedBaseSize,
                                                    float startBaseSize, qint32& progress, qint32 maxProgess, MirrorMode mirrorX, MirrorMode mirrorY);
    /**
     * @brief RandomMapGenerator::randomMapPlaceBuildings
     * @param width
     * @param heigth
     * @param buildingId
     * @param baseTerrainID
     * @param buildings
     * @param playerPositions
     * @param ownedBaseSize
     * @param chance
     * @param startBaseSize
     * @param randInt
     * @param mirrorX
     * @param mirrorY
     */
    static void randomMapPlaceBuildings(qint32 width, qint32 heigth, QString buildingId, QString baseTerrainID, qint32 buildings, const QVector<QPoint> & playerPositions,
                                        const QVector<float> & ownedBaseSize, float chance, float startBaseSize, QRandomGenerator& randInt, MirrorMode mirrorX, MirrorMode mirrorY);
    /**
     * @brief createRoad
     */
    static void randomMapCreateRoad(QRandomGenerator& randInt, QVector<QPoint>& playerPositions);
    /**
     * @brief isBuildingPlace
     * @param x
     * @param y
     * @return
     */
    static bool randomMapIsBuildingPlace(QString buildingId, qint32 x, qint32 y);
    /**
     * @brief placeBuildings
     * @param buildingId
     * @param baseTerrainID
     * @param buildings
     * @param playerPositions
     * @param ownedBaseSize
     * @param chance
     * @param startBaseSize
     * @param randInt
     */
    static void randomMapPlaceBuildings(QString buildingId, QString baseTerrainID, qint32 buildings, const QVector<QPoint> & playerPositions, const QVector<float> & ownedBaseSize, float chance, float startBaseSize, QRandomGenerator& randInt);
    /**
     * @brief randomMapPlaceTerain
     * @param terrainID
     * @param width
     * @param heigth
     * @param placeChance
     * @param placeSize
     * @param topTerrainIDs
     * @param placeChances
     * @param type
     * @param randInt
     */
    static void randomMapPlaceTerain(QString terrainID, qint32 width, qint32 heigth, float placeChance, QPoint placeSize, QStringList topTerrainIDs, QList<QVariant> placeChances, GameEnums::RandomMapTerrainType type, QRandomGenerator& randInt);
    /**
     * @brief placeOnTop
     * @param terrainID
     * @param topId
     * @param chance
     * @param randInt
     */
    static void randomMapPlaceOnTop(QString terrainID, QString topId, float chance, QRandomGenerator& randInt);
    /**
     * @brief randomMapPlaceUnits
     * @param units
     * @param unitCount
     * @param startBaseUnitSize
     * @param unitDistribution
     * @param progress
     * @param maxProgess
     */
    static void randomMapPlaceUnits(QVector<std::tuple<QString, float>> units,
                                    qint32 unitCount,
                                    float startBaseUnitSize,
                                    QVector<float> unitDistribution,
                                    QVector<QPoint> playerPositions,
                                    bool distributed,
                                    qint32& progress, qint32 maxProgess,
                                    QRandomGenerator& randInt,
                                    MirrorMode mirrorX, MirrorMode mirrorY);
    /**
     * @brief randomMapSpawnUnit
     * @param unitId
     * @param owner
     * @param nearHq
     * @param playerPositions
     * @param randInt
     */
    static void randomMapSpawnUnit(QString unitId, qint32 hqPos, bool nearHq, const QVector<QPoint> & playerPositions, QRandomGenerator& randInt, MirrorMode mirrorX, MirrorMode mirrorY);
    /**
     * @brief getMirroredPosition
     * @param index
     * @param position
     * @param mirrorX
     * @param mirrorY
     * @return
     */
    static QPoint getMirroredPosition(qint32 index, QPoint position, MirrorMode mirrorX, MirrorMode mirrorY);
    /**
     * @brief getMirroredPlayer
     * @param rotation
     * @param mirrorCount
     * @param mirrorX
     * @param mirrorY
     * @return
     */
    static qint32 getMirroredPlayer(qint32 rotation, qint32 mirrorCount, MirrorMode mirrorX, MirrorMode mirrorY);
    /**
     * @brief getPlayerForHqPos
     * @param hqPos
     * @param mirrorX
     * @param mirrorY
     * @return
     */
    static qint32 getPlayerForHqPos(qint32 hqPos, MirrorMode mirrorX, MirrorMode mirrorY);
    /**
     * @brief randomMapCreateRoad
     * @param randInt
     * @param playerPositions
     * @param mirrorX
     * @param mirrorY
     */
    static void randomMapCreateRoad(qint32 width, qint32 heigth, QRandomGenerator& randInt, QVector<QPoint>& playerPositions, MirrorMode mirrorX, MirrorMode mirrorY);
    /**
     * @brief connectPositionsWithRoad
     * @param randInt
     * @param position
     * @param endPoint
     * @param mirrorX
     * @param mirrorY
     * @param mirrorPlacing
     */
    static void connectPositionsWithRoad(QRandomGenerator& randInt, QPoint position, QPoint endPoint, MirrorMode mirrorX, MirrorMode mirrorY, qint32 mirrorPlacing);
};
