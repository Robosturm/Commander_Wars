#ifndef TERRAIN_H
#define TERRAIN_H

#include <QObject>
#include <QVector>

#include "oxygine-framework.h"

#include "game/smartpointers.h"

#include "coreengine/fileserializable.h"

class Terrain : public QObject, public oxygine::Actor, public FileSerializable
{
    Q_OBJECT
public:
    /**
     * @brief The DrawPriority enum z-priority for sprites
     */
    enum class DrawPriority
    {
        Terrain = 0,
        Building = 1,
        MarkedFieldLow = 2,
        Unit = 3,
        MarkedFieldMid = 4,
        Arrow,
        MarkedFieldHigh
    };
    static spTerrain createTerrain(const QString& terrainID, qint32 x, qint32 y);

    virtual ~Terrain();


    /**
     * @brief loadSprites loads all sprites we won't to show. Updates all sprite data
     */
    void loadSprites();
    QString getTerrainID() const;



    /**
     * @brief syncAnimation call this on all terrains to make their animation synchronized.
     */
    void syncAnimation();
    /**
     * @brief setBaseTerrain replace base terrain
     * @param terrain
     */
    void setBaseTerrain(spTerrain terrain);
    /**
     * @brief setUnit will place the units on this terrain ignoring all rules nullptr to delete it.
     * @param pUnit
     */
    void setUnit(spUnit pUnit);
    /**
     * @brief getUnit the unit on this terrain
     * @return
     */
    spUnit getSpUnit();

    spBuilding getSpBuilding();
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
        return 1;
    }
public slots:
    QString getTerrainName() const;
    void setTerrainName(const QString &value);
    qint32 getX() const;
    void setX(const qint32 &value);

    qint32 getY() const;
    void setY(const qint32 &value);
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
    void createBaseTerrain();
    /**
     * @brief loadBaseTerrain loads a base terrain with the given id
     * @param terrainID
     */
    void loadBaseTerrain(QString terrainID);
    /**
     * @brief loadBaseSprite loads the sprite for this terrain
     * @param spriteID
     */
    void loadBaseSprite(QString spriteID);
    /**
     * @brief getSurroundings returns a string containing the directions which fulfill the given rule
     * @param list the list as string split with ,
     * @param blacklist use black or whitelist for given arguments
     * @param searchType use GameMap::Directions here
     * @return
     */
    QString getSurroundings(QString list, bool useBaseTerrainID, bool blacklist, qint32 searchType, bool useMapBorder = true, bool useBuildingID = false);
    /**
     * @brief loadOverlaySprite loads overlay sprites of this terrain
     * @param spriteID
     */
    void loadOverlaySprite(QString spriteID);
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
            return terrainID;
        }
    }
    /**
     * @brief getID the overall id of this terrain either terrain id or building id
     * @return
     */
    QString getID();
    /**
     * @brief setBuilding will place the building on this terrain ignoring all rules nullptr to delete it.
     * @param pBuilding
     */
    void setBuilding(Building* pBuilding);
    /**
     * @brief loadBuilding deletes the current building by loading the given building ID.
     * @param buildingID
     */
    void loadBuilding(QString buildingID);
    /**
     * @brief getDefense
     */
    qint32 getDefense();
    /**
     * @brief getDefense
     */
    qint32 getBaseDefense();
private:
    explicit Terrain(const QString& terrainID, qint32 x, qint32 y);
    /**
     * @brief terrainName terrain name shown in the game
     */
    QString terrainName;
    /**
     * @brief terrainID our terrain id
     */
    QString terrainID;
    /**
     * @brief terrainSprite which terrain sprite we loaded
     */
    QString m_terrainSpriteName;
    /**
     * @brief m_pTerrainSprite actor holding our sprite data
     */
    oxygine::spSprite m_pTerrainSprite{nullptr};
    QVector<oxygine::spSprite> m_pOverlaySprites;
    /**
     * @brief fixedSprite stores if the selected sprite is fixed or will be loaded based on the surrounding and random factors
     */
    bool m_FixedSprite{false};
    /**
     * @brief m_pBaseTerrain base terrain of this terrain
     */
    spTerrain m_pBaseTerrain{nullptr};
    /**
     * @brief x coordinates in x direction in the map array
     */
    qint32 x{-1};
    /**
     * @brief y coordinates in y direction in the map array
     */
    qint32 y{-1};
    /**
      * the building at this position
      */
    spBuilding m_Building;
    /**
      * the unit at this position
      */
    spUnit m_Unit;

};

#endif // TERRAIN_H
