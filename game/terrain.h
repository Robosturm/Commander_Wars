#ifndef TERRAIN_H
#define TERRAIN_H

#include <QObject>
#include <QVector>

#include "coreengine/fileserializable.h"
#include "coreengine/JsCallback.h"
#include "coreengine/scriptvariables.h"
#include "coreengine/jsthis.h"
#include "objects/base/tooltip.h"
#include "game/GameEnums.h"

class Weather;
class GameAction;
class Building;
using spBuilding = std::shared_ptr<Building>;
class Player;
class Unit;
using spUnit = std::shared_ptr<Unit>;
class TerrainFindingSystem;
class GameMap;
using spTerrainFindingSystem = std::shared_ptr<TerrainFindingSystem>;

class Terrain;
using spTerrain = std::shared_ptr<Terrain>;

class Terrain final : public Tooltip, public FileSerializable, public JsThis
{
    Q_OBJECT

    struct TerrainOverlay
    {
        qint32 duration{-1};
        QString resAnim;
        QPoint offset;
        float scale{1.0f};
        QColor color;
        oxygine::spSprite sprite;

    };
public:
    /**
     * @brief The DrawPriority enum z-priority for sprites
     */
    enum class DrawPriority
    {
        Terrain = 0,
        TerrainOverlay,
        Shroud,
        Building,
        TerrainMarker,

    };
    enum class ExtraDrawPriority
    {
        BuildingLayer = 2,
    };
    static spTerrain createTerrain(const QString & terrainID, qint32 x, qint32 y, const QString & currentTerrainID, GameMap* pMap, const QString & currentTerrainPalette = "");
    virtual ~Terrain();

    void init();
    /**
     * @brief syncAnimation call this on all terrains to make their animation synchronized.
     */
    void syncAnimation(oxygine::timeMS syncTime);
    /**
     * @brief getTweenElapsed
     * @return
     */
    oxygine::timeMS getTweenElapsed();
    /**
     * @brief setBaseTerrain replace base terrain
     * @param terrain
     */
    void setBaseTerrain(spTerrain terrain);
    /**
     * @brief setUnit will place the units on this terrain ignoring all rules nullptr to remove it.
     * @param pUnit
     */
    void setUnit(spUnit pUnit);

    /**
     * @brief setBuilding will place the building on this terrain ignoring all rules nullptr to remove it.
     * @param pBuilding
     */
    void setBuilding(spBuilding pBuilding);
    /**
     * @brief getUnit the unit on this terrain
     * @return
     */
    spUnit getSpUnit();
    /**
     * @brief setBuilding
     * @param pBuilding
     * @param * @param OnlyDownStream normally the terrain knows the building and vice versa for large buildings like black hole cannon. This rule doesn't apply only the terrain knows the building.
     */
    void setSpBuilding(spBuilding pBuilding, bool OnlyDownStream);
    spBuilding getSpBuilding();
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serializeObject(QDataStream& pStream) const override;
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
     * @brief getVersion stream version for serialization
     * @return
     */
    inline virtual qint32 getVersion() const override
    {
        return 14;
    }
    /**
     * @brief isValid
     * @return
     */
    bool isValid();
    /**
     * @brief getMapTerrainDrawPriority
     * @return
     */
    qint32 getMapTerrainDrawPriority();
    /**
     * @brief setMapForExtending
     * @param newMap
     */
    void setMapForExtending(GameMap * newMap);
    /**
     * @brief getPalette
     * @return
     */
    Q_INVOKABLE QString getPalette() const;
    /**
     * @brief setPalette
     * @param newPalette
     */
    Q_INVOKABLE void setPalette(const QString & newPalette);
    Q_INVOKABLE QString getDefaultPalette();
    /**
     * @brief setTerrainPalette
     * @param newPalette
     */
    Q_INVOKABLE void setTerrainPalette(const QString newPalette, bool includeBaseTerrain = true);
    /**
     * @brief setTerrainPaletteGroup
     * @param newPaletteGroup
     */
    Q_INVOKABLE void setTerrainPaletteGroup(qint32 newPaletteGroup);
    /**
     * @brief resetTerrainOverlayPalettes
     */
    Q_INVOKABLE void resetTerrainOverlayPalettes();
    /**
     * @brief getFixedOverlaySprites
     * @return
     */
    Q_INVOKABLE bool getFixedOverlaySprites() const;
    /**
     * @brief setFixedOverlaySprites
     * @param newFixedOverlaySprites
     */
    Q_INVOKABLE void setFixedOverlaySprites(bool newFixedOverlaySprites);
    /**
     * @brief getCustomOverlays
     * @return
     */
    Q_INVOKABLE QStringList getCustomOverlays() const;
    /**
     * @brief setCustomOverlays
     * @param newCustomOverlays
     */
    Q_INVOKABLE void setCustomOverlays(const QStringList & newCustomOverlays, const QStringList & newPalettes = QStringList());
    /**
     * @brief addCustomOverlay
     * @param newCustomOverlay
     */
    Q_INVOKABLE void addCustomOverlay(const QString & customOverlay, const QString & palette = "");
    /**
     * @brief removeCustomOverlay
     * @param customOverlay
     */
    Q_INVOKABLE void removeCustomOverlay(const QString & customOverlay);
    /**
     * @brief setSpriteVisibility
     * @param value
     */
    Q_INVOKABLE void setSpriteVisibility(bool value);
    /**
     * @brief setFixedSprite only avaible for ingame editor
     * @return
     */
    Q_INVOKABLE void setFixedSprite(bool FixedSprite);
    /**
     * @brief setTerrainSpriteName only avaible for ingame editor
     * @return
     */
    Q_INVOKABLE void setTerrainSpriteName(const QString & terrainSpriteName);
    /**
     * @brief getShowInEditor
     * @param unitId
     * @return if the given terrain should be shown in the editor
     */
    Q_INVOKABLE static bool getShowInEditor(const QString & terrainId);
    /**
     * @brief getMap
     * @return
     */
    Q_INVOKABLE GameMap *getMap() const;
    /**
     * @brief getHasFlowDirection
     * @return
     */
    Q_INVOKABLE bool getHasFlowDirection() const;
    /**
     * @brief getTileHasFlowDirection
     * @return
     */
    Q_INVOKABLE bool getTileHasFlowDirection() const;
    /**
     * @brief setHasFlowDirection
     * @param newHasFlowDirection
     */
    Q_INVOKABLE void setHasFlowDirection(bool newHasFlowDirection);
    /**
     * @brief getWeatherOverlaySpeed
     * @return
     */
    Q_INVOKABLE QPoint getWeatherOverlaySpeed();
    /**
     * @brief getWeatherOverlayId
     * @return
     */
    Q_INVOKABLE QString getWeatherOverlayId();
    /**
     * @brief getFixedSprite only avaible for ingame editor
     * @return
     */
    Q_INVOKABLE bool getFixedSprite() const;
    /**
     * @brief getTerrainSpriteName only avaible for ingame editor
     * @return
     */
    Q_INVOKABLE QString getTerrainSpriteName() const;
    /**
     * @brief createTerrainFindingSystem Note: the path finding system needs to be deleted by the caller using killTerrainFindingSystem()
     * @return a path finding system that contains all connect terrains of the same type
     */
    Q_INVOKABLE TerrainFindingSystem* createTerrainFindingSystem();
    /**
     * @brief addTerrainOverlay
     */
    Q_INVOKABLE void addTerrainOverlay(const QString & id, qint32 x, qint32 y, QColor color = Qt::white, qint32 duration = -1, float scale = 1.0f);
    /**
     * @brief removeTerrainOverlay
     */
    Q_INVOKABLE void removeTerrainOverlay(const QString & id);
    /**
     * @brief getVariables
     * @return
     */
    Q_INVOKABLE inline ScriptVariables* getVariables()
    {
        return &m_Variables;
    }
    /**
     * @brief getAnimationVariables
     * @return
     */
    Q_INVOKABLE ScriptVariables* getAnimationVariables();
    /**
     * @brief getOffensiveBonus
     * @param pAttacker
     * @param atkPosition
     * @param pDefender
     * @param defPosition
     * @param isDefender
     * @return
     */
    Q_INVOKABLE qint32 getOffensiveFieldBonus(GameAction* pAction, Unit* pAttacker, QPoint atkPosition,Unit* pDefender,  QPoint defPosition, bool isDefender, GameEnums::LuckDamageMode luckMode);
    /**
     * @brief getDeffensiveBonus
     * @param pAttacker
     * @param atkPosition
     * @param pDefender
     * @param defPosition
     * @param isDefender
     * @return
     */
    Q_INVOKABLE qint32 getDeffensiveFieldBonus(GameAction* pAction, Unit* pAttacker, QPoint atkPosition, Unit* pDefender, QPoint defPosition, bool isAttacker, GameEnums::LuckDamageMode luckMode);
    /**
     * @brief startOfTurn
     */
    Q_INVOKABLE void startOfTurn();
    /**
     * @brief getHasStartOfTurn
     * @return
     */
    Q_INVOKABLE bool getHasStartOfTurn() const;
    /**
     * @brief setHasStartOfTurn
     * @param hasStartOfTurn
     */
    Q_INVOKABLE void setHasStartOfTurn(bool hasStartOfTurn);
    /**
     * @brief getTerrainDescription
     * @return
     */
    Q_INVOKABLE QString getTerrainDescription() const;
    /**
     * @brief setTerrainDescription
     * @param terrainDescription
     */
    Q_INVOKABLE void setTerrainDescription(const QString & terrainDescription);
    /**
     * @brief getVisionHigh
     * @return
     */
    Q_INVOKABLE qint32 getVisionHigh() const;
    /**
     * @brief setVisionHigh
     * @param VisionHigh
     */
    Q_INVOKABLE void setVisionHigh(const qint32 VisionHigh);
    /**
     * @brief getTotalVisionHigh
     * @return
     */
    Q_INVOKABLE qint32 getTotalVisionHigh();
    /**
     * @brief getTerrainGroup
     * @return
     */
    Q_INVOKABLE qint32 getTerrainGroup();
    /**
     * @brief getDescription
     * @return
     */
    Q_INVOKABLE QString getDescription();
    /**
     * @brief loadSprites loads all sprites we won't to show. Updates all sprite data
     */
    Q_INVOKABLE void loadSprites(bool reloadBase = true);
    /**
     * @brief unloadSprites
     */
    Q_INVOKABLE void unloadSprites();
    /**
     * @brief loadBaseTerrainSprites
     */
    Q_INVOKABLE void loadBaseTerrainSprites();
    /**
     * @brief getMinimapIcon
     * @return
     */
    Q_INVOKABLE QString getMinimapIcon();
    /**
     * @brief getTerrainID
     * @return
     */
    Q_INVOKABLE const QString getTerrainID() const;
    Q_INVOKABLE QString getTerrainName() const;
    Q_INVOKABLE void setTerrainName(const QString & value, bool customName = false);
    Q_INVOKABLE qint32 getX() const
    {
        return m_x;
    }
    Q_INVOKABLE void setX(const qint32 value);
    Q_INVOKABLE qint32 getHp() const;
    Q_INVOKABLE void setHp(const qint32 value);
    Q_INVOKABLE qint32 getY() const
    {
        return m_y;
    }
    Q_INVOKABLE void setY(const qint32 value);
    /**
     * @brief Terrain::getVision
     * @return
     */
    Q_INVOKABLE qint32 getVision(Player* pPlayer);
    /**
     * @brief getBonusVision
     * @param pUnit
     * @return
     */
    Q_INVOKABLE qint32 getBonusVision(Unit* pUnit);
    /**
     * @brief isLoadingTile
     * @return
     */
    Q_INVOKABLE bool isLoadingTile();
    /**
     * @brief getUnit the unit on this terrain
     * @return
     */
    Q_INVOKABLE Unit* getUnit() const;
    /**
     * @brief getBuilding the building on this terrain
     * @return
     */
    Q_INVOKABLE Building* getBuilding() const;
    /**
     * @brief createBaseTerrain creates the base terrain for this terrain if it's a nullptr
     */
    Q_INVOKABLE void createBaseTerrain(const QString & currentTerrainID, const QString & currentTerrainPalette = "");
    /**
     * @brief loadBaseTerrain loads a base terrain with the given id
     * @param terrainID
     */
    Q_INVOKABLE void loadBaseTerrain(const QString & terrainID, const QString & currentTerrainPalette = "");
    /**
     * @brief loadBaseSprite loads the sprite for this terrain
     * @param spriteID
     */
    Q_INVOKABLE void loadBaseSprite(const QString & spriteID, qint32 frameTime = 100, qint32 startFrame = -1, qint32 endFrame = -1);
    /**
     * @brief getSurroundings returns a string containing the directions which fulfill the given rule
     * @param list the list as string split with ,
     * @param blacklist use black or whitelist for given arguments
     * @param searchType use GameMap::Directions here
     * @param useMapBorder if true the terrain is treated as searched terrain
     * @param useBuildingID if building ID's are used or only terrain id's
     * @param recursionCount how deep the game searches for base terrain id's -1 for infinite
     * @param inverted for flow direction
     * @return
     */
    Q_INVOKABLE QString getSurroundings(const QString & list, bool useBaseTerrainID, bool blacklist, qint32 searchType, bool useMapBorder = true, bool useBuildingID = false, qint32 recursionCount = -1, bool inverted = false);
    /**
     * @brief loadOverlaySprite loads overlay sprites of this terrain
     * @param spriteID
     */
    Q_INVOKABLE void loadOverlaySprite(const QString & spriteID, qint32 startFrame = -1, qint32 endFrame = -1, const QString & palette = "", bool customOverlay = false);
    /**
     * @brief getBaseTerrainID finds the base terrain id of the real base terrain recursivly
     * @return the base terrainID
     */
    Q_INVOKABLE inline QString getBaseTerrainID()
    {
        if (m_pBaseTerrain.get() != nullptr)
        {
            return m_pBaseTerrain->getBaseTerrainID();
        }
        else
        {
            return m_terrainID;
        }
    }
    /**
     * @brief getBaseTerrainID recursivly
     * @param count maximum recursion
     * @return
     */
    Q_INVOKABLE inline QString getBaseTerrainIDOfLevel(qint32 count)
    {
        if (m_pBaseTerrain.get() != nullptr &&
            ((count > 0) || (count < 0)))
        {
            return m_pBaseTerrain->getBaseTerrainIDOfLevel(count - 1);
        }
        else
        {
            return m_terrainID;
        }
    }
    Q_INVOKABLE Terrain* getBaseTerrain(QString terrainId, bool returnNullIfNotFound = false)
    {
        if (m_pBaseTerrain.get() != nullptr)
        {
            if (m_pBaseTerrain->m_terrainID == terrainId || terrainId.isEmpty())
            {
                return m_pBaseTerrain.get();
            }
            else
            {
                return m_pBaseTerrain->getBaseTerrain(terrainId, returnNullIfNotFound);
            }
        }
        else if (!returnNullIfNotFound)
        {
            return this;
        }
        else
        {
            return nullptr;
        }
    }
    Q_INVOKABLE Terrain* getNextBaseTerrain()
    {
        return m_pBaseTerrain.get();
    }

    /**
     * @brief getID the overall id of this terrain either terrain id or building id
     * @return
     */
    Q_INVOKABLE QString getID();
    /**
     * @brief getFirerangeModifier
     * @param pUnit
     * @return
     */
    Q_INVOKABLE qint32 getFirerangeModifier(Unit* pUnit);
    /**
     * @brief getFirerangeModifier
     * @param pUnit
     * @return
     */
    Q_INVOKABLE qint32 getMinFirerangeModifier(Unit* pUnit);
    /**
     * @brief removeBuilding
     */
    Q_INVOKABLE void removeBuilding();
    /**
     * @brief loadBuilding deletes the current building by loading the given building ID.
     * @param buildingID
     */
    Q_INVOKABLE void loadBuilding(const QString & buildingID);
    /**
     * @brief getDefense
     */
    Q_INVOKABLE qint32 getDefense(Unit* pUnit);
    /**
     * @brief getDefense
     */
    Q_INVOKABLE qint32 getBaseDefense();
    /**
     * @brief getVisionHide
     * @return
     */
    Q_INVOKABLE bool getVisionHide(Player* pPlayer);
    /**
     * @brief getTerrainAnimationBase
     * @return
     */
    Q_INVOKABLE QString getTerrainAnimationBase();
    /**
     * @brief getTerrainAnimationForeground
     * @return
     */
    Q_INVOKABLE QString getTerrainAnimationForeground();
    /**
     * @brief getTerrainAnimationBackground
     * @return
     */
    Q_INVOKABLE QString getTerrainAnimationBackground();
    /**
     * @brief getTerrainAnimationMoveSpeed
     * @return
     */
    Q_INVOKABLE float getTerrainAnimationMoveSpeed();
    /**
     * @brief getTerrainSprites
     * @return
     */
    Q_INVOKABLE QStringList getTerrainSprites();
    /**
     * @brief getOverlayTerrainSprites
     * @return
     */
    Q_INVOKABLE QStringList getOverlayTerrainSprites();
    /**
     * @brief existsResAnim
     * @param spriteId
     * @return
     */
    Q_INVOKABLE bool existsResAnim(const QString & spriteId) const;
    /**
     * @brief getFittingResAnim
     * @param spriteIdStart
     * @param spriteIdEnd
     * @return
     */
    Q_INVOKABLE QString getFittingResAnim(const QString & spriteIdStart, const QString & spriteIdEnd) const;
    /**
     * @brief updateFlowSprites
     * @param pPfs
     */
    Q_INVOKABLE void updateFlowSprites(TerrainFindingSystem* pPfs, bool applyRulesPalette = false);
    /**
     * @brief getFlowTiles
     * @return
     */
    Q_INVOKABLE QStringList getFlowTiles();
    /**
     * @brief getMovementcostModifier
     * @param x
     * @param y
     * @param curX
     * @param curY
     * @return
     */
    Q_INVOKABLE qint32 getMovementcostModifier(Unit* pUnit, qint32 x, qint32 y, qint32 curX, qint32 curY);
    /**
     * @brief getPaletteNames
     * @return
     */
    Q_INVOKABLE static QStringList getPaletteNames();
    /**
     * @brief getPaletteId
     * @param index
     * @return
     */
    Q_INVOKABLE static QString getPaletteId(qint32 index, const QString & terrainId);
    /**
     * @brief getPaletteName
     * @param id
     * @return
     */
    Q_INVOKABLE static QString getPaletteName(const QString & id);
    /**
     * @brief getPaletteNameFromIndex
     * @param id
     * @return
     */
    Q_INVOKABLE static QString getPaletteNameFromIndex(qint32 id);
    /**
     * @brief getNeighbourPalette
     * @param direction
     * @return
     */
    Q_INVOKABLE QString getNeighbourPalette(GameEnums::Directions direction, const QString & baseTerrainId = "");
    /**
     * @brief getNeighbourPalette
     * @param direction
     * @param baseTerrainId
     * @return
     */
    Q_INVOKABLE QString getNeighbourDirectionsPalette(const QString & direction, const QString & baseTerrainId = "");
    /**
     * @brief getSupportPalette
     * @return
     */
    Q_INVOKABLE bool getSupportPalette() const;
    /**
     * @brief setSupportPalette
     * @param newSupportPalette
     */
    Q_INVOKABLE void setSupportPalette(bool newSupportPalette);
    /**
     * @brief onWeatherChanged
     * @param pWeather
     */
    Q_INVOKABLE void onWeatherChanged(Weather* pWeather);

    Q_INVOKABLE bool getShowInWiki();
protected:
    /**
     * @brief createBuildingDownStream
     */
    void createBuildingDownStream();
    /**
     * @brief removeDownstream
     */
    void removeDownstream();
    /**
     * @brief addBuildingSprite
     * @param pBuilding
     */
    void addBuildingSprite(spBuilding pBuilding);
private:
    /**
     * @brief customSpriteExists
     * @return
     */
    bool customSpriteExists() const;

    friend class MemoryManagement;
    explicit Terrain(QString terrainID, qint32 x, qint32 y, GameMap* pMap);
    void initTerrain();
    static qint32 getTerrainGroup(const QString & terrainId, GameMap* pMap);
private:
    QString m_palette;
    /**
     * @brief terrainName terrain name shown in the game
     */
    QString m_terrainName;
    /**
     * @brief m_customName
     */
    bool m_customName{false};
    /**
     * @brief m_terrainDescription
     */
    QString m_terrainDescription;
    /**
     * @brief terrainID our terrain id
     */
    QString m_terrainID;
    /**
     * @brief m_pTerrainSprite actor holding our sprite data
     */
    oxygine::spSprite m_pTerrainSprite;
    /**
     * @brief m_pOverlaySprites
     */
    QVector<oxygine::spSprite> m_pOverlaySprites;    
    /**
     * @brief m_customOverlays
     */
    QStringList m_customOverlays;
    /**
     * @brief m_overlayPalettes
     */
    QStringList m_customOverlayPalettes;
    /**
     * @brief m_fixedOverlaySprites
     */
    bool m_fixedOverlaySprites{false};
    /**
     * @brief fixedSprite stores if the selected sprite is fixed or will be loaded based on the surrounding and random factors
     */
    bool m_FixedSprite{false};
    /**
     * @brief terrainSprite which terrain sprite we loaded
     */
    QString m_terrainSpriteName;
    /**
     * @brief m_SpriteAnim
     */
    oxygine::spResAnim m_SpriteAnim;
    /**
     * @brief m_pBaseTerrain base terrain of this terrain
     */
    spTerrain m_pBaseTerrain{nullptr};
    /**
     * @brief x coordinates in x direction in the map array
     */
    qint32 m_x{-1};
    /**
     * @brief y coordinates in y direction in the map array
     */
    qint32 m_y{-1};
    /**
      * the building at this position
      */
    spBuilding m_Building{nullptr};
    /**
      * the unit at this position
      */
    spUnit m_Unit{nullptr};
    /**
      * hp of this unit
      */
    qint32 m_hp{-1};
    qint32 m_VisionHigh{0};
    ScriptVariables m_Variables;
    ScriptVariables m_AnimationVariables;
    bool m_hasStartOfTurn{false};
    bool m_hasFlowDirection{false};
    bool m_supportPalette{true};

    std::shared_ptr<JsCallback<Terrain>> m_pStartDayCallback;
    QVector<TerrainOverlay> m_terrainOverlay;

    QPoint m_test;
    GameMap* m_pMap{nullptr};
};

Q_DECLARE_INTERFACE(Terrain, "Terrain");

#endif // TERRAIN_H
