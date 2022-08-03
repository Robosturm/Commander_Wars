#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "3rd_party/oxygine-framework/oxygine/actor/ColorRectSprite.h"

#include <QObject>
#include <QVector>
#include <memory>
#include <vector>

#include "game/terrain.h"
#include "game/cursor.h"
#include "game/GameEnums.h"
#include "game/gamerules.h"
#include "game/gamerecording/gamerecorder.h"
#include "game/gamescript.h"
#include "game/campaign.h"
#include "game/player.h"

#include "coreengine/fileserializable.h"
#include "coreengine/qmlvector.h"

class GameAction;
using spGameAction = oxygine::intrusive_ptr<GameAction>;

class GameMap;
using spGameMap = oxygine::intrusive_ptr<GameMap>;

class BaseGamemenu;
using spBaseGamemenu = oxygine::intrusive_ptr<BaseGamemenu>;

class GameMap : public QObject, public FileSerializable, public oxygine::Actor
{
    Q_OBJECT
public:
    static const qint32 frameTime;
    static constexpr qint32 defaultImageSize = 32;
    static const char* const PLAINS;
    /**
     * @brief The MapHeaderInfo struct read from the filesystem
     */
    struct MapHeaderInfo
    {
        qint32 m_Version{0};
        QString m_mapName;
        QString m_mapAuthor;
        QString m_mapDescription;
        qint32 m_width{0};
        qint32 m_heigth{0};
        qint32 m_playerCount{0};
        qint32 m_uniqueIdCounter{0};
        mutable GameEnums::MapFilterFlags m_mapFlags{GameEnums::MapFilterFlags_None};
    };

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
    explicit GameMap(QDataStream& stream, bool savegame);
    /**
     * @brief GameMap
     * @param map path to the map which should be loaded
     */
    explicit GameMap(QString map, bool onlyLoad, bool fast, bool savegame);
    void loadMap(QString map, bool onlyLoad, bool fast, bool savegame);
    virtual ~GameMap();
    /**
     * @brief newMap
     * @param width
     * @param heigth
     * @param playerCount
     */
    void newMap(qint32 width, qint32 heigth, qint32 playerCount, const QString & baseTerrain = "PLAINS");
    /**
     * @brief changeMap
     * @param width
     * @param heigth
     * @param playerCount
     */
    void changeMap(qint32 width, qint32 heigth, qint32 playerCount);
    /**
     * @brief resizeMap
     * @param left
     * @param top
     * @param right
     * @param bottom
     */
    void resizeMap(qint32 left, qint32 top, qint32 right, qint32 bottom);
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
    virtual void serializeObject(QDataStream& pStream) const override;
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
     * @brief readMapHeader
     * @param pStream
     * @param headerInfo
     */
    static void readMapHeader(QDataStream& pStream, MapHeaderInfo & headerInfo);
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
    inline virtual qint32 getVersion() const override
    {
        return 12;
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
     * @return a unique id
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
     * @brief initPlayersAndSelectCOs
     */
    void initPlayersAndSelectCOs();
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
    /**
     * @brief anyPlayerAlive
     * @return
     */
    bool anyPlayerAlive();
    /**
     * @brief registerMapAtInterpreter
     */
    void registerMapAtInterpreter();
    /**
     * @brief getMapPath
     * @return
     */
    QString getMapPath() const;
    /**
     * @brief setMapPath
     * @param mapPath
     */
    void setMapPath(const QString &mapPath);
    /**
     * @brief playMusic
     */
    void playMusic();
    /**
     * @brief getSavegame
     * @return
     */
    bool getSavegame() const;
    /**
     * @brief limitPosition
     * @param pMenu
     * @param newX
     * @param newY
     */
    void limitPosition(BaseGamemenu* pMenu, qint32 & newX, qint32 & newY);
    /**
     * @brief setIsHumanMatch
     * @param newIsHumanMatch
     */
    void setIsHumanMatch(bool newIsHumanMatch);
    /**
     * @brief getRowActor
     * @return
     */
    oxygine::spActor getRowActor(qint32 y)
    {
        if (y >= 0 && y < m_rowSprites.size())
        {
            return m_rowSprites[y];
        }
        return oxygine::spActor();
    }
    /**
     * @brief getMarkedFieldsLayer
     * @return
     */
    oxygine::spActor getMarkedFieldsLayer() const;
    /**
     * @brief getMoveArrowLayer
     * @return
     */
    oxygine::spActor getMoveArrowLayer() const;
    /**
     * @brief getUnitsLayer
     * @return
     */
    oxygine::spActor getUnitsLayer() const;
    /**
     * @brief setMenu
     * @param newMenu
     */
    void setMenu(GameMenue *newMenu);

signals:
    void signalExitGame();
    void sigSaveGame();
    void sigVictoryInfo();
    void signalShowCOInfo();
    void sigShowGameInfo(qint32 player);
    void sigShowAttackLog(qint32 player);
    void sigShowUnitInfo(qint32 player);
    void sigQueueAction(spGameAction pAction);
    void sigSurrenderGame();
    void sigShowNicknameUnit(qint32 x, qint32 y);
    void sigShowOptions();
    void sigShowChangeSound();
    void sigShowWiki();
    void sigShowRules();
    void sigShowUnitStatistics(qint32 player);
    void sigMovedMap();
    void sigZoomChanged(float zoom);
    void sigShowDamageCalculator();
    void sigShowMovementPlanner();
    void sigShowLoadSaveGame();
public slots:
    /**
     * @brief getMapTagsText
     * @return
     */
    QString getMapTagsText();
    /**
     * @brief getIsHumanMatch
     * @return
     */
    bool getIsHumanMatch() const;
    /**
     * @brief getMapMusic
     * @return
     */
    QString getMapMusic() const;
    /**
     * @brief setMapMusic
     * @param mapMusic
     */
    void setMapMusic(const QString &mapMusic, qint32 startLoopMs = -1, qint32 endLoopMs = -1);
    /**
     * @brief clearMapMusic
     */
    void clearMapMusic();
    /**
     * @brief getFrameTime
     * @return
     */
    static qint32 getFrameTime();
    /**
     * @brief getVisionCircle
     * @param x
     * @param y
     * @param minVisionRange
     * @param maxVisionRange
     * @param visionHigh
     * @return
     */
    QmlVectorPoint* getVisionCircle(qint32 x, qint32 y, qint32 minVisionRange, qint32 maxVisionRange, qint32 visionHigh);
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
     * @brief isPlayersUnitInArea
     * @param area
     * @param unitID
     * @return
     */
    bool isPlayersUnitInArea(const QRect& area, QList<qint32> & playerIDs);
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
    Unit* getUnitFromTansportUnit(Unit* pUnit, qint32 uniqueID);
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
    spGameAction createAction();
    /**
     * @brief queueAction
     * @param pAction
     */
    void queueAction(spGameAction pAction);
    /**
     * @brief getGameRecorder
     * @return
     */
    GameRecorder* getGameRecorder();
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
     * @brief options
     */
    void options();
    /**
     * @brief changeSound
     */
    void changeSound();
    /**
     * @brief saveGame
     */
    void saveGame();
    /**
     * @brief loadGame
     */
    void loadGame();
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
    void showGameInfo(qint32 player);
    /**
     * @brief showAttackLog
     */
    void showAttackLog(qint32 player);
    /**
     * @brief showUnitInfo
     */
    void showUnitInfo(qint32 player);
    /**
     * @brief showWiki
     */
    void showWiki();
    /**
     * @brief showRules
     */
    void showRules();
    /**
     * @brief showUnitStatistics
     */
    void showUnitStatistics(qint32 player);
    /**
     * @brief showMovementPlanner
     */
    void showMovementPlanner();
    /**
     * @brief getTerrainCount
     * @param terrainId
     * @return
     */
    qint32 getTerrainCount(const QString & terrainId) const;
    /**
     * @brief getBuildingCount
     * @param buildingID
     * @return
     */
    qint32 getBuildingCount(const QString & buildingID) const;
    /**
     * @brief getPlayerBuildingCount
     * @param buildingID
     * @param pPlayer
     * @return
     */
    qint32 getPlayerBuildingCount(const QString & buildingID, Player* pPlayer) const;
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
    Unit* spawnUnit(qint32 x, qint32 y, const QString & unitID, Player* owner, qint32 range = 0, bool ignoreMovement = false);
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
    void updateSprites(qint32 xInput = -1, qint32 yInput = -1, bool editor = false, bool showLoadingScreen = false);
    /**
     * @brief updateSpritesOfTiles
     * @param points
     * @param editor
     * @param showLoadingScreen
     */
    void updateSpritesOfTiles(const QVector<QPoint> & points, bool editor = false, bool showLoadingScreen = false);
    /**
     * @brief updateTileSprites
     * @param x
     * @param y
     * @param flowPoints
     * @param editor
     */
    void updateTileSprites(qint32 x, qint32 y, QVector<QPoint> & flowPoints, bool editor = false);
    /**
     * @brief updateFlowTiles
     * @param flowPoints
     */
    void updateFlowTiles(QVector<QPoint> & flowPoints);
    /**
     * @brief syncTerrainAnimations
     * @param showLoadingScreen
     */
    void syncTerrainAnimations(bool showLoadingScreen = false);
    /**
     * @brief finishUpdateSprites
     * @param showLoadingScreen
     */
    void finishUpdateSprites(bool showLoadingScreen);
    /**
     * @brief syncUnitsAndBuildings
     */
    void syncUnitsAndBuildingAnimations();
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
    bool onMap(qint32 x, qint32 y) const;
    /**
     * @brief centerMap centers the view point to the given location
     * @param x
     * @param y
     */
    void centerMap(qint32 x, qint32 y, bool updateMinimapPosition = true);
    /**
     * @brief getCenteredPosition
     * @return
     */
    QPoint getCenteredPosition();
    /**
     * @brief zoom zooms into or out of the map :)
     * @param zoom
     */
    void setZoom(float zoom);
    inline float getZoom() const
    {
        return getScaleX();
    }
    void moveMap(qint32 x, qint32 y);
    /**
     * @brief getTerrain only use this for js scripts
     * @param x
     * @param y
     * @return the real pointer to the given terrain
     */
    Terrain* getTerrain(qint32 x, qint32 y) const;
    /**
     * @brief canBePlaced
     * @param terrainID the terrain id you want to place
     * @param x position
     * @param y position
     * @return true if this terrain can be placed
     */
    bool canBePlaced(const QString & terrainID, qint32 x, qint32 y);
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
    void replaceTerrain(const QString & terrainID, qint32 x, qint32 y, bool useTerrainAsBaseTerrain = false, bool callUpdateSprites = false, bool checkPlacement = true);
    /**
     * @brief replaceTerrainOnly
     * @param terrainID
     * @param x
     * @param y
     * @param useTerrainAsBaseTerrain
     */
    void replaceTerrainOnly(const QString & terrainID, qint32 x, qint32 y, bool useTerrainAsBaseTerrain = false, bool removeUnit = true);
    /**
     * @brief replaceBuilding
     * @param buildingID
     * @param x
     * @param y
     */
    void replaceBuilding(const QString & buildingID, qint32 x, qint32 y);
    /**
     * @brief getPlayerCount
     * @return number of players on the map
     */
    qint32 getPlayerCount() const
    {
        return m_players.size();
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
     * @brief getCurrentViewPlayer
     * @return
     */
    Player* getCurrentViewPlayer();
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
     * @brief setImagesize
     * @param imagesize
     */
    static void setImagesize(const qint32 &imagesize);
    /**
     * @brief nextTurn next players turn.
     */
    void nextTurn(quint32 dayToDayUptimeMs = 2000);    
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
     * @brief startOfTurnNeutral
     */
    void startOfTurnNeutral();
    /**
     * @brief startOfTurnPlayer
     * @param pPlayer
     */
    void startOfTurnPlayer(Player* pPlayer);
    /**
     * @brief centerOnPlayer
     * @param pPlayer
     */
    void centerOnPlayer(Player* pPlayer);
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
    QmlVectorBuilding* getBuildings(Player* pPlayer, QString id = "");
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

    void setMapNameFromFilename(QString filename);
    QString getMapName() const;
    void setMapName(const QString &value);

    inline GameRules* getGameRules()
    {
        return m_Rules.get();
    }
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
    /**
     * @brief nicknameUnit
     * @param x
     * @param y
     */
    void nicknameUnit(qint32 x, qint32 y);
    /**
     * qbrief killDeadUnits
     */
    void killDeadUnits();
    /**
     * @brief addScreenshake
     */
    void addScreenshake(qint32 startIntensity, float decay, qint32 durationMs, qint32 delayMs = 0, qint32 shakePauseMs = 30);
    /**
     * @brief showGrid
     */
    void showGrid(bool show);
    /**
     * @brief showMiddleCrossGrid
     */
    void showMiddleCrossGrid(bool show);
    /**
     * @brief getMapFlags
     * @return
     */
    GameEnums::MapFilterFlags getMapFlags() const;
    /**
     * @brief setMapFlags
     * @param flags
     */
    void setMapFlags(GameEnums::MapFilterFlags flags);
    /**
     * @brief onWeatherChanged
     */
    void onWeatherChanged(Weather* pWeather);
    /**
     * @brief showDamageCalculator
     */
    void showDamageCalculator();
private slots:
    void zoomChanged();
private:
    void loadMapData();
    QColor getGridColor();
    void updateMapFlags() const;
private:
    QString m_mapPath;
    MapHeaderInfo m_headerInfo;
    std::vector<std::vector<spTerrain>> m_fields;
    std::vector<oxygine::spActor> m_rowSprites;
    oxygine::spActor m_markedFieldsLayer;
    oxygine::spActor m_moveArrowLayer;
    oxygine::spActor m_unitsLayer;
    QVector<spPlayer> m_players;
    QVector<oxygine::spColorRectSprite> m_gridSprites;
    QVector<oxygine::spColorRectSprite> m_middleCrossGridSprites;
    spPlayer m_CurrentPlayer;
    qint32 m_currentDay{0};
    spGameRules m_Rules;
    spCampaign m_Campaign;
    spGameRecorder m_Recorder{spGameRecorder::create(this)};
    spGameScript m_GameScript{spGameScript::create(this)};
    static const QString m_GameAnimationFactory;
    bool m_loaded{false};
    QString m_mapMusic;
    QString m_loadedMapMusic;
    qint32 m_startLoopMs{-1};
    qint32 m_endLoopMs{-1};
    bool m_savegame{false};
    bool m_isHumanMatch{false};
    GameMenue* m_pMenu{nullptr};
    static qint32 m_imagesize;
};

#endif // GAMEMAP_H
