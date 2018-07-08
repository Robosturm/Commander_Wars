#ifndef TERRAIN_H
#define TERRAIN_H

#include <QObject>
#include <QVector>

#include "oxygine-framework.h"

// make us smart
class Terrain;
typedef oxygine::intrusive_ptr<Terrain> spTerrain;

class Terrain : public QObject, public oxygine::Actor
{
    Q_OBJECT
    Q_PROPERTY(QString terrainID READ getTerrainID )
    Q_PROPERTY(bool y READ getY WRITE setY)
    Q_PROPERTY(bool x READ getX WRITE setX)
    Q_PROPERTY(QString terrainName READ getTerrainName WRITE setTerrainName)
public:

    static Terrain* createTerrain(const QString& terrainID, qint32 x, qint32 y);

    virtual ~Terrain();

    qint32 getX() const;
    void setX(const qint32 &value);

    qint32 getY() const;
    void setY(const qint32 &value);
    /**
     * @brief loadSprites loads all sprites we won't to show. Updates all sprite data
     */
    void loadSprites();
    QString getTerrainID() const;


    QString getTerrainName() const;
    void setTerrainName(const QString &value);

public slots:
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
    QString getSurroundings(QString list, bool useBaseTerrainID, bool blacklist, qint32 searchType);
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
            return "";
        }
    }
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

};

#endif // TERRAIN_H
