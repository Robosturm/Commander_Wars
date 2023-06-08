#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "3rd_party/oxygine-framework/oxygine/actor/ColorRectSprite.h"

#include <QObject>
#include <QVector>
#include <memory>
#include <vector>

#include "game/terrain.h"
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

class EditorMenue;
class BaseGamemenu;
using spBaseGamemenu = oxygine::intrusive_ptr<BaseGamemenu>;

class GameMap final : public QObject, public FileSerializable, public oxygine::Actor
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
    void newMap(qint32 width, qint32 heigth, qint32 playerCount, const QString baseTerrain = "PLAINS");
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
        return 13;
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
     * @brief initProxyAis
     */
    void initProxyAis();
    /**
     * @brief isUnitInArea
     * @param pUnit
     * @param area
     * @param checkFunction
     * @return
     */
    bool isInArea(const QRect& area, std::function<bool (Unit* pUnit)> checkFunction);
    /**
     * @brief applyToArea
     * @param area
     * @param applyFunction
     */
    void applyToArea(const QRect& area, std::function<void (qint32 x, qint32 y)> applyFunction);
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
    void setMenu(BaseGamemenu *newMenu);
    BaseGamemenu *getMenu() const;
    /**
     * @brief setSavegame
     * @param newSavegame
     */
    void setSavegame(bool newSavegame);
    /**
     * @brief getMapHash
     * @return
     */
    QByteArray getMapHash();
    QString getRecordFile() const;
    void setRecordFile(const QString & newRecordFile);

    qint32 getReplayActionCount() const;
    void setReplayActionCount(qint32 newReplayActionCount);
    /**
     * @brief getField changes the coordinates into the given direction
     * @param x x-coordinates
     * @param y y-coordinates
     * @param direction the suitable direction
     */
    static void getField(qint32& x, qint32& y, GameEnums::Directions direction);
    /**
     * @brief updateSpritesOfTiles
     * @param points
     * @param editor
     * @param showLoadingScreen
     */
    void updateSpritesOfTiles(const QVector<QPoint> & points, bool editor = false, bool showLoadingScreen = false);
signals:
    void signalExitGame();
    void sigSaveGame();
    void sigVictoryInfo();
    void signalShowCOInfo();
    void sigShowGameInfo(qint32 player);
    void sigShowAttackLog(qint32 player);
    void sigShowUnitInfo(qint32 player);
    void sigSurrenderGame();
    void sigShowNicknameUnit(qint32 x, qint32 y);
    void sigShowXmlFileDialog(const QString xmlFile, bool saveSettings = false);
    void sigShowWiki();
    void sigShowRules();
    void sigShowUnitStatistics(qint32 player);
    void sigMovedMap();
    void sigZoomChanged(float zoom);
    void sigShowDamageCalculator();
    void sigShowMovementPlanner();
    void sigShowLoadSaveGame();
public:
    /**
     * @brief extendMap
     * @param mapFile
     */
    Q_INVOKABLE void extendMap(const QString mapFile, GameEnums::Directions direction);
    /**
     * @brief setIsHumanMatch
     * @param newIsHumanMatch
     */
    Q_INVOKABLE void setIsHumanMatch(bool newIsHumanMatch);
    /**
     * @brief applyPaletteToArea
     * @param area
     * @param newPalette
     */
    Q_INVOKABLE void applyPaletteToArea(const QRect area, qint32 newPalette);
    /**
     * @brief applyBiomeToArea
     * @param area
     * @param newBiome
     */
    Q_INVOKABLE void applyBiomeToArea(const QRect area, qint32 newBiome);
    /**
     * @brief getMapTagsText
     * @return
     */
    Q_INVOKABLE QString getMapTagsText();
    /**
     * @brief getIsHumanMatch
     * @return
     */
    Q_INVOKABLE bool getIsHumanMatch() const;
    /**
     * @brief getMapMusic
     * @return
     */
    Q_INVOKABLE QString getMapMusic() const;
    /**
     * @brief setMapMusic
     * @param mapMusic
     */
    Q_INVOKABLE void setMapMusic(const QString mapMusic, qint32 startLoopMs = -1, qint32 endLoopMs = -1);
    /**
     * @brief clearMapMusic
     */
    Q_INVOKABLE void clearMapMusic();
    /**
     * @brief getFrameTime
     * @return
     */
    Q_INVOKABLE static qint32 getFrameTime();
    /**
     * @brief getVisionCircle
     * @param x
     * @param y
     * @param minVisionRange
     * @param maxVisionRange
     * @param visionHigh
     * @return
     */
    Q_INVOKABLE QmlVectorPoint* getVisionCircle(qint32 x, qint32 y, qint32 minVisionRange, qint32 maxVisionRange, qint32 visionHigh);
    /**
     * @brief isUnitInArea
     * @param area
     * @param unitID
     * @return
     */
    Q_INVOKABLE bool isUnitInArea(const QRect area, qint32 unitID);
    /**
     * @brief isPlayerUnitInArea
     * @param area
     * @param unitID
     * @return
     */
    Q_INVOKABLE bool isPlayerUnitInArea(const QRect area, qint32 playerID);
    /**
     * @brief isPlayersUnitInArea
     * @param area
     * @param unitID
     * @return
     */
    Q_INVOKABLE bool isPlayersUnitInArea(const QRect area, const QVector<qint32> playerIDs);
    /**
     * @brief getUnit
     * @param uniqueID
     * @return
     */
    Q_INVOKABLE Unit* getUnit(qint32 uniqueID);
    /**
     * @brief getUnit
     * @param pUnit the transporting unit
     * @param uniqueID the unit it should transport
     * @return the found unit
     */
    Q_INVOKABLE Unit* getUnitFromTansportUnit(Unit* pUnit, qint32 uniqueID);
    /**
     * @brief getGameScript
     * @return
     */
    Q_INVOKABLE GameScript* getGameScript();
    /**
     * @brief getCampaign
     * @return
     */
    Q_INVOKABLE Campaign* getCampaign();
    /**
     * @brief getMapAuthor
     * @return
     */
    Q_INVOKABLE QString getMapAuthor() const;
    /**
     * @brief setMapAuthor
     * @param value
     */
    Q_INVOKABLE void setMapAuthor(const QString value);
    /**
     * @brief getMapDescription
     * @return
     */
    Q_INVOKABLE QString getMapDescription() const;
    /**
     * @brief setMapDescription
     * @param value
     */
    Q_INVOKABLE void setMapDescription(const QString &value);
    /**
     * @brief getAllUnitIDs
     * @return
     */
    Q_INVOKABLE QStringList getAllUnitIDs();
    /**
     * @brief getGameRecorder
     * @return
     */
    Q_INVOKABLE GameRecorder* getGameRecorder();
    /**
     * @brief getCurrentDay
     * @return
     */
    Q_INVOKABLE qint32 getCurrentDay() const;
    /**
     * @brief exitGame
     */
    Q_INVOKABLE void exitGame();
    /**
     * @brief options
     */
    Q_INVOKABLE void showXmlFileDialog(const QString xmlFile, bool saveSettings = false);
    /**
     * @brief saveGame
     */
    Q_INVOKABLE void saveGame();
    /**
     * @brief loadGame
     */
    Q_INVOKABLE void loadGame();
    /**
     * @brief victoryInfo
     */
    Q_INVOKABLE void victoryInfo();
    /**
     * @brief showCOInfo
     */
    Q_INVOKABLE void showCOInfo();
    /**
     * @brief showGameInfo
     */
    Q_INVOKABLE void showGameInfo(qint32 player);
    /**
     * @brief showAttackLog
     */
    Q_INVOKABLE void showAttackLog(qint32 player);
    /**
     * @brief showUnitInfo
     */
    Q_INVOKABLE void showUnitInfo(qint32 player);
    /**
     * @brief showWiki
     */
    Q_INVOKABLE void showWiki();
    /**
     * @brief showRules
     */
    Q_INVOKABLE void showRules();
    /**
     * @brief showUnitStatistics
     */
    Q_INVOKABLE void showUnitStatistics(qint32 player);
    /**
     * @brief showMovementPlanner
     */
    Q_INVOKABLE void showMovementPlanner();
    /**
     * @brief getTerrainCount
     * @param terrainId
     * @return
     */
    Q_INVOKABLE qint32 getTerrainCount(const QString terrainId) const;
    /**
     * @brief getBuildingCount
     * @param buildingID
     * @return
     */
    Q_INVOKABLE qint32 getBuildingCount(const QString buildingID) const;
    /**
     * @brief getPlayerBuildingCount
     * @param buildingID
     * @param pPlayer
     * @return
     */
    Q_INVOKABLE qint32 getPlayerBuildingCount(const QString buildingID, Player* pPlayer) const;
    /**
     * @brief getMapWidth
     * @return width of the map
     */
    Q_INVOKABLE qint32 getMapWidth() const;
    /**
     * @brief getMapHeight
     * @return height of the map
     */
    Q_INVOKABLE qint32 getMapHeight() const;
    /**
     * @brief spawnUnit
     * @param x coordinates starting at 0
     * @param y coordinates starting at 0
     * @param unitID type, id of the unit checkout the unit scripts to get the id's
     * @param owner get a player from the map
     * @param range the unit will be spawned on an empty field that can be crossed by the unit. This range is the test range where the game tries to spawn the unit. From 0 to anything
     * @return the spawned unit
     */
    Q_INVOKABLE Unit* spawnUnit(qint32 x, qint32 y, const QString unitID, Player* owner, qint32 range = 0, bool ignoreMovement = false);
    /**
     * @brief refillAll refills all units ammo and fuel
     */
    Q_INVOKABLE void refillAll();
    /**
     * @brief refillTransportedUnits
     * @param pUnit
     */
    Q_INVOKABLE void refillTransportedUnits(Unit* pUnit);
    /**
     * @brief updateTerrainSprites updates the terrain sprites refreshes them to fit the current map
     * @param xInput around given coordinates -1 whole map
     * @param yInput around given coordinates -1 whole map
     */
    Q_INVOKABLE void updateSprites(qint32 xInput = -1, qint32 yInput = -1, bool editor = false, bool showLoadingScreen = false, bool applyRulesPalette = false);
    /**
     * @brief syncTerrainAnimations
     * @param showLoadingScreen
     */
    Q_INVOKABLE void syncTerrainAnimations(bool showLoadingScreen = false);
    /**
     * @brief finishUpdateSprites
     * @param showLoadingScreen
     */
    Q_INVOKABLE void finishUpdateSprites(bool showLoadingScreen);
    /**
     * @brief syncUnitsAndBuildings
     */
    Q_INVOKABLE void syncUnitsAndBuildingAnimations();
    /**
     * @brief onMap returns if the given coordinates are on the map
     * @param x
     * @param y
     * @return true if it's still on the map
     */
    Q_INVOKABLE bool onMap(qint32 x, qint32 y) const;
    /**
     * @brief centerMap centers the view point to the given location
     * @param x
     * @param y
     */
    Q_INVOKABLE void centerMap(qint32 x, qint32 y, bool updateMinimapPosition = true);
    /**
     * @brief getCenteredPosition
     * @return
     */
    Q_INVOKABLE QPoint getCenteredPosition() const;
    /**
     * @brief zoom zooms into or out of the map :)
     * @param zoom
     */
    Q_INVOKABLE void setZoom(qint32 zoom);
    Q_INVOKABLE inline float getZoom() const
    {
        return getScaleX();
    }
    Q_INVOKABLE void moveMap(qint32 x, qint32 y);
    /**
     * @brief getTerrain only use this for js scripts
     * @param x
     * @param y
     * @return the real pointer to the given terrain
     */
    Q_INVOKABLE Terrain* getTerrain(qint32 x, qint32 y) const;
    /**
     * @brief canBePlaced
     * @param terrainID the terrain id you want to place
     * @param x position
     * @param y position
     * @return true if this terrain can be placed
     */
    Q_INVOKABLE bool canBePlaced(const QString terrainID, qint32 x, qint32 y);
    /**
     * @brief updateTerrain updates the given fields around. So all terrains are placeable.
     * @param x
     * @param y
     */
    Q_INVOKABLE void updateTerrain(qint32 x, qint32 y);
    /**
     * @brief replaceTerrain
     * @param terrainID
     * @param x
     * @param y
     */
    Q_INVOKABLE void replaceTerrain(const QString terrainID, qint32 x, qint32 y, bool useTerrainAsBaseTerrain = false, bool callUpdateSprites = false, bool checkPlacement = true, const QString palette = "", bool changePalette = false, bool includeBaseTerrain = true);
    /**
     * @brief replaceTerrainOnly
     * @param terrainID
     * @param x
     * @param y
     * @param useTerrainAsBaseTerrain
     */
    Q_INVOKABLE void replaceTerrainOnly(const QString terrainID, qint32 x, qint32 y, bool useTerrainAsBaseTerrain = false, bool removeUnit = true, const QString palette = "", bool changePalette = false, bool includeBaseTerrain = true);
    /**
     * @brief replaceBuilding
     * @param buildingID
     * @param x
     * @param y
     */
    Q_INVOKABLE void replaceBuilding(const QString buildingID, qint32 x, qint32 y);
    /**
     * @brief getPlayerCount
     * @return number of players on the map
     */
    Q_INVOKABLE qint32 getPlayerCount() const
    {
        return m_players.size();
    }
    /**
     * @brief getPlayer
     * @param player player index in the vector
     * @return pointer to the player
     */
    Q_INVOKABLE Player* getPlayer(qint32 player);
    /**
     * @brief getCurrentPlayer the current player
     * @return
     */
    Q_INVOKABLE Player* getCurrentPlayer() const;
    /**
     * @brief getCurrentViewPlayer
     * @return
     */
    Q_INVOKABLE Player* getCurrentViewPlayer() const;
    /**
     * @brief setCurrentPlayer changes the current player to this one
     * @param player
     */
    Q_INVOKABLE void setCurrentPlayer(qint32 player);
    /**
     * @brief getImageSize
     * @return the size of an field in pixel
     */
    Q_INVOKABLE static qint32 getImageSize();
    /**
     * @brief setImagesize
     * @param imagesize
     */
    Q_INVOKABLE static void setImagesize(const qint32 imagesize);
    /**
     * @brief nextTurn next players turn.
     */
    Q_INVOKABLE void nextTurn(quint32 dayToDayUptimeMs = 2000);
    /**
     * @brief enableUnits makes all units of the given player movable again
     * @param pPlayer
     */
    Q_INVOKABLE void enableUnits(Player* pPlayer);
    /**
     * @brief endOfTurn
     * @param pPlayer
     */
    Q_INVOKABLE void endOfTurn(Player* pPlayer);
    /**
     * @brief endOfTurnPlayer
     * @param pPlayer
     */
    Q_INVOKABLE void endOfTurnPlayer(Player* pPlayer);
    /**
     * @brief startOfTurn calls all start of turn calls of each unit and building owned by this player
     * @param pPlayer
     */
    Q_INVOKABLE void startOfTurn(Player* pPlayer);
    /**
     * @brief startOfTurnNeutral
     */
    Q_INVOKABLE void startOfTurnNeutral();
    /**
     * @brief startOfTurnPlayer
     * @param pPlayer
     */
    Q_INVOKABLE void startOfTurnPlayer(Player* pPlayer);
    /**
     * @brief centerOnPlayer
     * @param pPlayer
     */
    Q_INVOKABLE void centerOnPlayer(Player* pPlayer);
    /**
     * @brief nextPlayer next player
     */
    Q_INVOKABLE bool nextPlayer();
    /**
     * @brief checkFuel checks fuel of the unit and if it's negative kills it.
     * @param pPlayer
     */
    Q_INVOKABLE void checkFuel(Player* pPlayer);
    /**
     * @brief getUnits
     * @param pPlayer
     * @return
     */
    Q_INVOKABLE QmlVectorUnit* getUnits(Player* pPlayer);
    /**
     * @brief getBuildings
     * @param pPlayer
     * @return
     */
    Q_INVOKABLE QmlVectorBuilding* getBuildings(Player* pPlayer, QString id = "");
    /**
     * @brief getBuildingsListCount
     * @param pPlayer
     * @param ids
     * @return
     */
    Q_INVOKABLE QmlVectorBuilding* getBuildingsListCount(Player* pPlayer, const QStringList ids);
    /**
     * @brief importTxtMap imports a map in old CoW text-format
     */
    Q_INVOKABLE void importTxtMap(QString file);
    /**
     * @brief importAWDSMap
     * @param file
     */
    Q_INVOKABLE void importAWDSMap(QString file, EditorMenue* pMenu);
    /**
     * @brief exportAWDSMap
     * @param file
     */
    Q_INVOKABLE void exportAWDSMap(QString file);
    /**
     * @brief importAW4Map
     * @param file
     */
    Q_INVOKABLE void importAW4Map(QString file, EditorMenue* pMenu);
    /**
     * @brief importAWByWebMap
     * @param file
     */
    Q_INVOKABLE void importAWByWebMap(QString file, EditorMenue* pMenu);
    /**
     * @brief exportAW4Map
     * @param file
     */
    Q_INVOKABLE void exportAW4Map(QString file);
    Q_INVOKABLE void setMapNameFromFilename(QString filename);
    Q_INVOKABLE QString getMapName() const;
    Q_INVOKABLE void setMapName(const QString value);
    Q_INVOKABLE inline GameRules* getGameRules() const
    {
        return m_Rules.get();
    }
    /**
     * @brief updateUnitIcons
     */
    Q_INVOKABLE void updateUnitIcons();
    /**
     * @brief getWinnerTeam
     * @return -1 for no winner
     */
    Q_INVOKABLE qint32 getWinnerTeam();
    /**
     * @brief surrenderGame the current player surrenders the game
     */
    Q_INVOKABLE void surrenderGame();
    /**
     * @brief nicknameUnit
     * @param x
     * @param y
     */
    Q_INVOKABLE void nicknameUnit(qint32 x, qint32 y);
    /**
     * qbrief killDeadUnits
     */
    Q_INVOKABLE void killDeadUnits();
    /**
     * @brief addScreenshake
     */
    Q_INVOKABLE void addScreenshake(qint32 startIntensity, float decay, qint32 durationMs, qint32 delayMs = 0, qint32 shakePauseMs = 30);
    /**
     * @brief showGrid
     */
    Q_INVOKABLE void showGrid(bool show);
    /**
     * @brief showMiddleCrossGrid
     */
    Q_INVOKABLE void showMiddleCrossGrid(bool show);
    /**
     * @brief getMapFlags
     * @return
     */
    Q_INVOKABLE GameEnums::MapFilterFlags getMapFlags() const;
    /**
     * @brief hasMapFlags
     * @param flags
     * @return
     */
    Q_INVOKABLE bool hasMapFlags(GameEnums::MapFilterFlags flags) const;
    /**
     * @brief setMapFlags
     * @param flags
     */
    Q_INVOKABLE void setMapFlags(GameEnums::MapFilterFlags flags);
    /**
     * @brief onWeatherChanged
     */
    Q_INVOKABLE void onWeatherChanged(Weather* pWeather);
    /**
     * @brief showDamageCalculator
     */
    Q_INVOKABLE void showDamageCalculator();
private slots:
    void zoomChanged();
private:
    /**
     * @brief updateFlowTiles
     * @param flowPoints
     */
    void updateFlowTiles(QVector<QPoint> & flowPoints, bool applyRulesPalette = false);
    /**
     * @brief updateTileSprites
     * @param x
     * @param y
     * @param flowPoints
     * @param editor
     */
    void updateTileSprites(qint32 x, qint32 y, QVector<QPoint> & flowPoints, bool editor = false, bool applyRulesPalette = false);
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
    static const char* const m_GameAnimationFactory;
    bool m_loaded{false};
    QString m_mapMusic;
    QString m_loadedMapMusic;
    qint32 m_startLoopMs{-1};
    qint32 m_endLoopMs{-1};
    bool m_savegame{false};
    bool m_isHumanMatch{false};
    BaseGamemenu* m_pMenu{nullptr};
    QString m_recordFile;
    qint32 m_replayActionCount{0};
    static qint32 m_imagesize;
};

#endif // GAMEMAP_H
