#ifndef TERRAIN_H
#define TERRAIN_H

#include <QObject>
#include <QVector>

#include "coreengine/fileserializable.h"
#include "coreengine/JsCallback.h"
#include "coreengine/LUPDATE_MACROS.h"
#include "coreengine/scriptvariables.h"

#include "game/unit.h"
#include "game/building.h"

#include "objects/base/tooltip.h"

class Player;
class TerrainFindingSystem;
class GameMap;
using spTerrainFindingSystem = oxygine::intrusive_ptr<TerrainFindingSystem>;

class Terrain;
using spTerrain = oxygine::intrusive_ptr<Terrain>;

class Terrain : public Tooltip, public FileSerializable
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
    ENUM_CLASS DrawPriority
    {
        Terrain = 0,
        TerrainOverlay,
        Shroud,
        Building,
        TerrainMarker,                

    };
    ENUM_CLASS ExtraDrawPriority
    {
        BuildingLayer = 2,
    };
    static spTerrain createTerrain(const QString & terrainID, qint32 x, qint32 y, const QString & currentTerrainID, GameMap* pMap);

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
        return 9;
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
public slots:
    /**
     * @brief setSpriteVisibility
     * @param value
     */
    void setSpriteVisibility(bool value);
    /**
     * @brief setFixedSprite only avaible for ingame editor
     * @return
     */
    void setFixedSprite(bool FixedSprite);
    /**
     * @brief setTerrainSpriteName only avaible for ingame editor
     * @return
     */
    void setTerrainSpriteName(const QString &terrainSpriteName);
    /**
     * @brief getShowInEditor
     * @param unitId
     * @return if the given terrain should be shown in the editor
     */
    static bool getShowInEditor(QString terrainId);
    /**
     * @brief getMap
     * @return
     */
    GameMap *getMap() const;
    /**
     * @brief getHasFlowDirection
     * @return
     */
    bool getHasFlowDirection() const;
    /**
     * @brief setHasFlowDirection
     * @param newHasFlowDirection
     */
    void setHasFlowDirection(bool newHasFlowDirection);
    /**
     * @brief getWeatherOverlaySpeed
     * @return
     */
    QPoint getWeatherOverlaySpeed();
    /**
     * @brief getWeatherOverlayId
     * @return
     */
    QString getWeatherOverlayId();
    /**
     * @brief getFixedSprite only avaible for ingame editor
     * @return
     */
    bool getFixedSprite() const;
    /**
     * @brief getTerrainSpriteName only avaible for ingame editor
     * @return
     */
    QString getTerrainSpriteName() const;
    /**
     * @brief createTerrainFindingSystem Note: the path finding system needs to be deleted by the caller using killTerrainFindingSystem()
     * @return a path finding system that contains all connect terrains of the same type
     */
    TerrainFindingSystem* createTerrainFindingSystem();
    /**
     * @brief addTerrainOverlay
     */
    void addTerrainOverlay(QString id, qint32 x, qint32 y, QColor color = Qt::white, qint32 duration = -1, float scale = 1.0f);
    /**
     * @brief removeTerrainOverlay
     */
    void removeTerrainOverlay(const QString & id);
    /**
     * @brief getVariables
     * @return
     */
    inline ScriptVariables* getVariables()
    {
        return &m_Variables;
    }
    /**
     * @brief getOffensiveBonus
     * @param pAttacker
     * @param atkPosition
     * @param pDefender
     * @param defPosition
     * @param isDefender
     * @return
     */
    qint32 getOffensiveFieldBonus(GameAction* pAction, Unit* pAttacker, QPoint atkPosition,Unit* pDefender,  QPoint defPosition, bool isDefender, GameEnums::LuckDamageMode luckMode);
    /**
     * @brief getDeffensiveBonus
     * @param pAttacker
     * @param atkPosition
     * @param pDefender
     * @param defPosition
     * @param isDefender
     * @return
     */
    qint32 getDeffensiveFieldBonus(GameAction* pAction, Unit* pAttacker, QPoint atkPosition, Unit* pDefender, QPoint defPosition, bool isAttacker, GameEnums::LuckDamageMode luckMode);
    /**
     * @brief startOfTurn
     */
    void startOfTurn();
    /**
     * @brief getHasStartOfTurn
     * @return
     */
    bool getHasStartOfTurn() const;
    /**
     * @brief setHasStartOfTurn
     * @param hasStartOfTurn
     */
    void setHasStartOfTurn(bool hasStartOfTurn);
    /**
     * @brief getTerrainDescription
     * @return
     */
    QString getTerrainDescription() const;
    /**
     * @brief setTerrainDescription
     * @param terrainDescription
     */
    void setTerrainDescription(const QString &terrainDescription);
    /**
     * @brief getVisionHigh
     * @return
     */
    qint32 getVisionHigh() const;
    /**
     * @brief setVisionHigh
     * @param VisionHigh
     */
    void setVisionHigh(const qint32 &VisionHigh);
    /**
     * @brief getTotalVisionHigh
     * @return
     */
    qint32 getTotalVisionHigh();
    /**
     * @brief getTerrainGroup
     * @return
     */
    qint32 getTerrainGroup();
    /**
     * @brief getDescription
     * @return
     */
    QString getDescription();
    /**
     * @brief loadSprites loads all sprites we won't to show. Updates all sprite data
     */
    void loadSprites();
    /**
     * @brief unloadSprites
     */
    void unloadSprites();
    /**
     * @brief loadBaseTerrainSprites
     */
    void loadBaseTerrainSprites();
    /**
     * @brief getMinimapIcon
     * @return
     */
    QString getMinimapIcon();
    /**
     * @brief getTerrainID
     * @return
     */
    QString getTerrainID() const;

    QString getTerrainName() const;
    void setTerrainName(const QString &value);

    qint32 getX() const;
    void setX(const qint32 &value);

    qint32 getHp() const;
    void setHp(const qint32 &value);

    qint32 getY() const;
    void setY(const qint32 &value);
    /**
     * @brief Terrain::getVision
     * @return
     */
    qint32 getVision(Player* pPlayer);
    /**
     * @brief getBonusVision
     * @param pUnit
     * @return
     */
    qint32 getBonusVision(Unit* pUnit);
    /**
     * @brief isLoadingTile
     * @return
     */
    bool isLoadingTile();
    /**
     * @brief getUnit the unit on this terrain
     * @return
     */
    Unit* getUnit();
    /**
     * @brief getBuilding the building on this terrain
     * @return
     */
    Building* getBuilding();
    /**
     * @brief createBaseTerrain creates the base terrain for this terrain if it's a nullptr
     */
    void createBaseTerrain(const QString & currentTerrainID);
    /**
     * @brief loadBaseTerrain loads a base terrain with the given id
     * @param terrainID
     */
    void loadBaseTerrain(const QString & terrainID);
    /**
     * @brief loadBaseSprite loads the sprite for this terrain
     * @param spriteID
     */
    void loadBaseSprite(const QString & spriteID, qint32 frameTime = 100, qint32 startFrame = -1, qint32 endFrame = -1);
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
    QString getSurroundings(const QString & list, bool useBaseTerrainID, bool blacklist, qint32 searchType, bool useMapBorder = true, bool useBuildingID = false, qint32 recursionCount = -1, bool inverted = false);
    /**
     * @brief loadOverlaySprite loads overlay sprites of this terrain
     * @param spriteID
     */
    void loadOverlaySprite(const QString & spriteID, qint32 startFrame = -1, qint32 endFrame = -1);
    /**
     * @brief getBaseTerrainID finds the base terrain id of the real base terrain recursivly
     * @return the base terrainID
     */
    inline QString getBaseTerrainID()
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
    inline QString getBaseTerrainIDOfLevel(qint32 count)
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
    Terrain* getBaseTerrain(QString terrainId)
    {
        if (m_pBaseTerrain.get() != nullptr)
        {

            if (m_pBaseTerrain->m_terrainID == terrainId)
            {
                return m_pBaseTerrain.get();
            }
            else
            {
                return m_pBaseTerrain->getBaseTerrain(terrainId);
            }
        }
        else
        {
            return nullptr;
        }
    }
    /**
     * @brief getID the overall id of this terrain either terrain id or building id
     * @return
     */
    QString getID();
    /**
     * @brief getFirerangeModifier
     * @param pUnit
     * @return
     */
    qint32 getFirerangeModifier(Unit* pUnit);    
    /**
     * @brief getFirerangeModifier
     * @param pUnit
     * @return
     */
    qint32 getMinFirerangeModifier(Unit* pUnit);
    /**
     * @brief removeBuilding
     */
    void removeBuilding();
    /**
     * @brief loadBuilding deletes the current building by loading the given building ID.
     * @param buildingID
     */
    void loadBuilding(const QString & buildingID);
    /**
     * @brief getDefense
     */
    qint32 getDefense(Unit* pUnit);
    /**
     * @brief getDefense
     */
    qint32 getBaseDefense();
    /**
     * @brief getVisionHide
     * @return
     */
    bool getVisionHide(Player* pPlayer);
    /**
     * @brief getTerrainAnimationBase
     * @return
     */
    QString getTerrainAnimationBase();
    /**
     * @brief getTerrainAnimationForeground
     * @return
     */
    QString getTerrainAnimationForeground();
    /**
     * @brief getTerrainAnimationBackground
     * @return
     */
    QString getTerrainAnimationBackground();
    /**
     * @brief getTerrainAnimationMoveSpeed
     * @return
     */
    float getTerrainAnimationMoveSpeed();
    /**
     * @brief getTerrainSprites
     * @return
     */
    QStringList getTerrainSprites();
    /**
     * @brief existsResAnim
     * @param spriteId
     * @return
     */
    bool existsResAnim(const QString & spriteId) const;
    /**
     * @brief getFittingResAnim
     * @param spriteIdStart
     * @param spriteIdEnd
     * @return
     */
    QString getFittingResAnim(const QString & spriteIdStart, const QString & spriteIdEnd) const;
    /**
     * @brief updateFlowSprites
     * @param pPfs
     */
    void updateFlowSprites(TerrainFindingSystem* pPfs);
    /**
     * @brief getFlowTiles
     * @return
     */
    QStringList getFlowTiles();
    /**
     * @brief getMovementcostModifier
     * @param x
     * @param y
     * @param curX
     * @param curY
     * @return
     */
    qint32 getMovementcostModifier(Unit* pUnit, qint32 x, qint32 y, qint32 curX, qint32 curY);    
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

    friend class oxygine::intrusive_ptr<Terrain>;
    explicit Terrain(QString terrainID, qint32 x, qint32 y, GameMap* pMap);

private:
    /**
     * @brief terrainName terrain name shown in the game
     */
    QString m_terrainName;
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
    oxygine::spSprite m_pTerrainSprite{nullptr};
    /**
     * @brief m_pOverlaySprites
     */
    QVector<oxygine::spSprite> m_pOverlaySprites;
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
    bool m_hasStartOfTurn{false};
    bool m_hasFlowDirection{false};

    oxygine::intrusive_ptr<JsCallback<Terrain>> m_pStartDayCallback;
    QVector<TerrainOverlay> m_terrainOverlay;

    QPoint m_test;
    GameMap* m_pMap{nullptr};
};

Q_DECLARE_INTERFACE(Terrain, "Terrain");

#endif // TERRAIN_H
