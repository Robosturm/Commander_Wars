#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <QObject>

#include <QVector>

#include "QRandomGenerator"

#include "oxygine/core/ref_counter.h"
#include "oxygine-framework.h"
#include "game/smartpointers.h"

#include "memory"

#include "game/terrain.h"

#include "objects/cursor.h"

#include "coreengine/fileserializable.h"

#include "coreengine/qmlvector.h"

#include "game/GameEnums.h"

class GameAction;

class GameMap : public QObject, public oxygine::Actor, public FileSerializable
{
    Q_OBJECT

    Q_PROPERTY(qint32 width READ getWidth WRITE setWidth)
public:
    static const qint32 frameTime;
    static const qint32 Imagesize = 24;

    /**
     * @brief GameMap creates an empty ma (filled with plains) with two players and the given size
     * @param width
     * @param heigth
     */
    explicit GameMap(qint32 width, qint32 heigth, qint32 playerCount);

    /**
     * @brief GameMap
     * @param map path to the map which should be loaded
     */
    explicit GameMap(QString map, bool gamestart);
    virtual ~GameMap();
    /**
     * @brief newMap
     * @param width
     * @param heigth
     * @param playerCount
     */
    void newMap(qint32 width, qint32 heigth, qint32 playerCount);
    /**
     * @brief changeMap
     * @param width
     * @param heigth
     * @param playerCount
     */
    void changeMap(qint32 width, qint32 heigth, qint32 playerCount);
    /**
     * @brief flipX
     */
    void flipX();
    /**
     * @brief flipY
     */
    void flipY();
    /**
     * @brief rotateX
     */
    void rotateX();
    /**
     * @brief rotateY
     */
    void rotateY();
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
    qint32 randomMap(qint32 width,qint32 heigth, qint32 playerCount,
                              bool roadSupport = true, qint32 seed = -1,
                              float forestchance = 0.0286f, float mountainChance= 0.0125f, float seachance = 0.007024f, float buildingchance = 0.1f);
    /**
     * @brief placeGroup
     * @param startX
     * @param startY
     * @param count
     * @param terrainID
     * @param terrainRadius
     * @param randInt
     */
    void placeGroup(qint32 startX, qint32 startY, qint32 count, QString terrainID, qint32 terrainRadius, QRandomGenerator& randInt);
    /**
     * @brief addTerrainPoint
     * @param points
     * @param x
     * @param y
     * @param terrainID
     * @param terrainRadius
     */
    void addTerrainPoint(QVector<QPoint>& points, qint32 x, qint32 y, QString terrainID, qint32 terrainRadius);
    /**
     * @brief placeReaf
     * @param randInt
     */
    void placeReaf(QRandomGenerator& randInt);
    /**
     * @brief placeBeach
     * @param randInt
     */
    void placeBeach(QRandomGenerator& randInt);
    /**
     * @brief createBuildings
     * @param buildings
     * @param roadSupport
     * @param randInt
     * @param noHarbour
     */
    void createBuildings(qint32 buildings, bool roadSupport, QRandomGenerator& randInt, bool noHarbour = false);
    /**
     * @brief getInstance
     * @return
     */
    inline static GameMap* getInstance()
    {
        return m_pInstance;
    }
    /**
     * @brief getTerrain smart pointer
     * @param x
     * @param y
     * @return the smart pointer
     */
    spTerrain getSpTerrain(qint32 x, qint32 y);
    /**
     * @brief getspPlayer
     * @param player index of the player
     * @return smart pointer to the selected player
     */
    spPlayer getspPlayer(qint32 player);
    /**
     * @brief getSpCurrentPlayer the current player
     * @return
     */
    spPlayer getSpCurrentPlayer();
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
     * @brief getVersion stream version for serialization
     * @return
     */
    inline virtual qint32 getVersion() override
    {
        return 2;
    }
    /**
     * @brief clearMap
     */
    void clearMap();
signals:

public slots:
    /**
     * @brief getMapWidth
     * @return width of the map
     */
    qint32 getMapWidth() const;
    /**
     * @brief getMapHeight
     * @return height of the map
     */
    qint32 getMapHeight() const;
    /**
     * @brief spawnUnit
     * @param x
     * @param y
     * @param unitID
     * @param owner
     * @param range
     * @return
     */
    Unit* spawnUnit(qint32 x, qint32 y, QString unitID, Player* owner, qint32 range = 0);
    /**
     * @brief updateTerrainSprites updates the terrain sprites refreshes them to fit the current map
     * @param xInput around given coordinates -1 whole map
     * @param yInput around given coordinates -1 whole map
     */
    void updateSprites(qint32 xInput = -1, qint32 yInput = -1);
    /**
     * @brief getField changes the coordinates into the given direction
     * @param x x-coordinates
     * @param y y-coordinates
     * @param direction the suitable direction
     */
    static void getField(qint32& x, qint32& y, GameEnums::Directions direction);
    /**
     * @brief onMap returns if the given coordinates are on the map
     * @param x
     * @param y
     * @return true if it's still on the map
     */
    bool onMap(qint32 x, qint32 y);
    /**
     * @brief centerMap centers the view point to the given location
     * @param x
     * @param y
     */
    void centerMap(qint32 x, qint32 y);
    /**
     * @brief zoom zooms into or out of the map :)
     * @param zoom
     */
    void zoom(float zoom);
    inline float getZoom() const
    {
        return m_zoom;
    }
    void moveMap(qint32 x, qint32 y);
    /**
     * @brief getTerrain only use this for js scripts
     * @param x
     * @param y
     * @return the real pointer to the given terrain
     */
    Terrain* getTerrain(qint32 x, qint32 y);

    /**
     * @brief canBePlaced
     * @param terrainID the terrain id you want to place
     * @param x position
     * @param y position
     * @return true if this terrain can be placed
     */
    bool canBePlaced(const QString& terrainID, qint32 x, qint32 y);
    /**
     * @brief updateTerrain updates the given fields around. So all terrains are placeable.
     * @param x
     * @param y
     */
    void updateTerrain(qint32 x, qint32 y);
    /**
     * @brief replaceTerrain
     * @param terrainID
     * @param x
     * @param y
     */
    void replaceTerrain(const QString& terrainID, qint32 x, qint32 y, bool useTerrainAsBaseTerrain = false, bool updateSprites = false);
    /**
     * @brief getPlayerCount
     * @return number of players on the map
     */
    qint32 getPlayerCount()
    {
        return players.size();
    }
    /**
     * @brief getPlayer
     * @param player player index in the vector
     * @return pointer to the player
     */
    Player* getPlayer(qint32 player);
    /**
     * @brief getCurrentPlayer the current player
     * @return
     */
    Player* getCurrentPlayer();
    /**
     * @brief setCurrentPlayer changes the current player to this one
     * @param player
     */
    void setCurrentPlayer(qint32 player);
    /**
     * @brief getImageSize
     * @return the size of an field in pixel
     */
    static qint32 getImageSize();
    /**
     * @brief nextTurn next players turn.
     */
    void nextTurn();
    /**
     * @brief enableUnits makes all units of the given player movable again
     * @param pPlayer
     */
    void enableUnits(Player* pPlayer);
    /**
     * @brief startOfTurn calls all start of turn calls of each unit and building owned by this player
     * @param pPlayer
     */
    void startOfTurn(Player* pPlayer);
    /**
     * @brief nextPlayer next player
     */
    void nextPlayer();
    /**
     * @brief checkFuel checks fuel of the unit and if it's negative kills it.
     * @param pPlayer
     */
    void checkFuel(Player* pPlayer);
    /**
     * @brief getUnits
     * @param pPlayer
     * @return
     */
    QmlVectorUnit* getUnits(Player* pPlayer);
    /**
     * @brief importTxtMap imports a map in old CoW text-format
     */
    void importTxtMap(QString file);

    QString getMapName() const;
    void setMapName(const QString &value);
private:
    static GameMap* m_pInstance;

    QString mapName;
    QVector<QVector<spTerrain>*> fields;
    QVector<spPlayer> players;
    spPlayer m_CurrentPlayer;

    quint32 currentDay{0};

    static const QString m_JavascriptName;
    static const QString m_GameAnimationFactory;
    float m_zoom{1.0f};

    void loadMapData();
};

#endif // GAMEMAP_H
