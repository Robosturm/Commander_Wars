#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <QObject>

#include <QVector>

#include <QRandomGenerator>

#include "oxygine/core/ref_counter.h"
#include "oxygine-framework.h"

#include "memory"

#include "game/terrain.h"

#include "objects/cursor.h"

#include "coreengine/fileserializable.h"

#include "coreengine/qmlvector.h"

#include "game/GameEnums.h"

#include "game/gamerules.h"

#include "game/gamerecording/gamerecorder.h"

#include "game/gamescript.h"
#include "game/campaign.h"

#include "player.h"

class GameAction;

class GameMap;
typedef oxygine::intrusive_ptr<GameMap> spGameMap;

class GameMap : public QObject, public FileSerializable, public oxygine::Actor
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
     * @param stream
     */
    explicit GameMap(QDataStream& stream);
    /**
     * @brief GameMap
     * @param map path to the map which should be loaded
     */
    explicit GameMap(QString map, bool onlyLoad);
    virtual ~GameMap();
    /**
     * @brief deleteMap
     */
    static void deleteMap();
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
                     bool roadSupport, qint32 seed,
                     float forestchance, float mountainChance, float seachance, float buildingchance,
                     float factoryChance, float airPortChance, float harbourChance);
    /**
     * @brief placeGroup
     * @param startX
     * @param startY
     * @param count
     * @param terrainID
     * @param terrainRadius
     * @param randInt
     */
    qint32 placeGroup(qint32 startX, qint32 startY, qint32 count, QString terrainID, qint32 terrainRadius, QRandomGenerator& randInt);
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
    QVector<QPoint> createBuildings(qint32 buildings, QRandomGenerator& randInt, float factoryChance, float airPortChance, float harbourChance);
    /**
     * @brief createRoad
     */
    void createRoad(QRandomGenerator& randInt, QVector<QPoint>& playerPositions);
    /**
     * @brief isBuildingPlace
     * @param x
     * @param y
     * @return
     */
    bool isBuildingPlace(qint32 x, qint32 y);
    /**
     * @brief getInstance
     * @return
     */
    inline static GameMap* getInstance()
    {
        return m_pInstance.get();
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
    virtual void serializeObject(QDataStream& pStream) override;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserializeObject(QDataStream& pStream) override;
    /**
     * @brief readMapName
     * @param pStream
     * @return
     */
    static QString readMapName(QDataStream& pStream);
    /**
     * @brief getVersion stream version for serialization
     * @return
     */
    inline virtual qint32 getVersion() override
    {
        return 8;
    }
    /**
     * @brief clearMap
     */
    void clearMap();
    /**
     * @brief startGame
     */
    void startGame();
    /**
     * @brief removePlayer removes a player from the map only use this in the editor menue
     * @param index
     */
    void removePlayer(qint32 index);
    /**
     * @brief getUniqueIdCounter
     * @return a new unique id
     */
    qint32 getUniqueIdCounter();
    /**
     * @brief setCampaign
     * @param Campaign
     */
    void setCampaign(spCampaign campaign);
    /**
     * @brief getSpCampaign
     * @return
     */
    spCampaign getSpCampaign();
    /**
     * @brief initPlayers
     */
    void initPlayers();
    /**
     * @brief isUnitInArea
     * @param pUnit
     * @param area
     * @param checkFunction
     * @return
     */
    bool isInArea(const QRect& area, std::function<bool (Unit* pUnit)> checkFunction);
signals:
    void signalExitGame();
    void signalSaveGame();
    void signalVictoryInfo();
    void signalShowCOInfo();
    void sigShowGameInfo();
    void sigShowAttackLog();
    void sigQueueAction(GameAction* pAction);
    void sigSurrenderGame();
public slots:
    /**
     * @brief isUnitInArea
     * @param area
     * @param unitID
     * @return
     */
    bool isUnitInArea(const QRect& area, qint32 unitID);
    /**
     * @brief isPlayerUnitInArea
     * @param area
     * @param unitID
     * @return
     */
    bool isPlayerUnitInArea(const QRect& area, qint32 playerID);
    /**
     * @brief getUnit
     * @param uniqueID
     * @return
     */
    Unit* getUnit(qint32 uniqueID);
    /**
     * @brief getUnit
     * @param pUnit the transporting unit
     * @param uniqueID the unit it should transport
     * @return the found unit
     */
    Unit* getUnit(Unit* pUnit, qint32 uniqueID);
    /**
     * @brief getGameScript
     * @return
     */
    GameScript* getGameScript();
    /**
     * @brief getCampaign
     * @return
     */
    Campaign* getCampaign();
    /**
     * @brief getMapAuthor
     * @return
     */
    QString getMapAuthor() const;
    /**
     * @brief setMapAuthor
     * @param value
     */
    void setMapAuthor(const QString &value);
    /**
     * @brief getMapDescription
     * @return
     */
    QString getMapDescription() const;
    /**
     * @brief setMapDescription
     * @param value
     */
    void setMapDescription(const QString &value);
    /**
     * @brief getAllUnitIDs
     * @return
     */
    QStringList getAllUnitIDs();
    /**
     * @brief createAction
     * @return
     */
    GameAction* createAction();
    /**
     * @brief queueAction
     * @param pAction
     */
    void queueAction(GameAction* pAction);
    /**
     * @brief getGameRecorder
     * @return
     */
    inline GameRecorder* getGameRecorder()
    {
        return m_Recorder.get();
    }
    /**
     * @brief getCurrentDay
     * @return
     */
    qint32 getCurrentDay() const;
    /**
     * @brief exitGame
     */
    void exitGame();
    /**
     * @brief saveGame
     */
    void saveGame();
    /**
     * @brief victoryInfo
     */
    void victoryInfo();
    /**
     * @brief showCOInfo
     */
    void showCOInfo();
    /**
     * @brief showGameInfo
     */
    void showGameInfo();    
    /**
     * @brief showAttackLog
     */
    void showAttackLog();
    /**
     * @brief getBuildingCount
     * @param buildingID
     * @return
     */
    qint32 getBuildingCount(QString buildingID);
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
     * @param x coordinates starting at 0
     * @param y coordinates starting at 0
     * @param unitID type, id of the unit checkout the unit scripts to get the id's
     * @param owner get a player from the map
     * @param range the unit will be spawned on an empty field that can be crossed by the unit. This range is the test range where the game tries to spawn the unit. From 0 to anything
     * @return the spawned unit
     */
    Unit* spawnUnit(qint32 x, qint32 y, QString unitID, Player* owner, qint32 range = 0);
    /**
     * @brief refillAll refills all units ammo and fuel
     */
    void refillAll();
    /**
     * @brief refillTransportedUnits
     * @param pUnit
     */
    void refillTransportedUnits(Unit* pUnit);
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
    bool canBePlaced(QString terrainID, qint32 x, qint32 y);
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
    void replaceTerrain(QString terrainID, qint32 x, qint32 y, bool useTerrainAsBaseTerrain = false, bool updateSprites = false);
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
    bool nextPlayer();
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
     * @brief getBuildings
     * @param pPlayer
     * @return
     */
    QmlVectorBuilding* getBuildings(Player* pPlayer);
    /**
     * @brief importTxtMap imports a map in old CoW text-format
     */
    void importTxtMap(QString file);
    /**
     * @brief importAWDSMap
     * @param file
     */
    void importAWDSMap(QString file);
    /**
     * @brief exportAWDSMap
     * @param file
     */
    void exportAWDSMap(QString file);
    /**
     * @brief importAWDCMap
     * @param file
     */
    void importAWDCMap(QString file);
    /**
     * @brief importAWByWebMap
     * @param file
     */
    void importAWByWebMap(QString file);
    /**
     * @brief exportAWDCMap
     * @param file
     */
    void exportAWDCMap(QString file);


    QString getMapName() const;
    void setMapName(const QString &value);

    inline GameRules* getGameRules()
    {
        return m_Rules.get();
    }
    /**
     * @brief getCurrentViewPlayer
     * @return
     */
    Player* getCurrentViewPlayer();
    /**
     * @brief updateUnitIcons
     */
    void updateUnitIcons();
    /**
     * @brief getWinnerTeam
     * @return -1 for no winner
     */
    qint32 getWinnerTeam();
    /**
     * @brief surrenderGame the current player surrenders the game
     */
    void surrenderGame();
private:
    static spGameMap m_pInstance;
    QString mapName;
    QString mapAuthor;
    QString mapDescription;
    QVector<QVector<spTerrain>*> fields;
    QVector<spPlayer> players;
    spPlayer m_CurrentPlayer;
    qint32 currentDay{0};
    spGameRules m_Rules;
    spCampaign m_Campaign;
    spGameRecorder m_Recorder{new GameRecorder()};
    spGameScript m_GameScript{new GameScript()};
    static const QString m_JavascriptName;
    static const QString m_GameAnimationFactory;
    float m_zoom{1.0f};
    bool loaded{false};
    qint32 m_UniqueIdCounter{0};

    void loadMapData();
};

#endif // GAMEMAP_H
