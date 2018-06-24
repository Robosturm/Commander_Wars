#ifndef TERRAIN_H
#define TERRAIN_H

#include <QObject>

#include "oxygine-framework.h"

class Terrain : public QObject, public oxygine::Actor
{
    Q_OBJECT
    Q_PROPERTY(bool Animated READ getAnimated WRITE setAnimated)
    Q_PROPERTY(bool x READ getX WRITE setX)
    Q_PROPERTY(bool y READ getY WRITE setY)
public:
    enum SearchTypes
    {
        All = 0,
        Direct,
        Diagnonal
    };

    explicit Terrain(const QString& terrainID, qint32 x, qint32 y);
    virtual ~Terrain() = default;

    bool getAnimated() const;
    void setAnimated(bool value);

    qint32 getX() const;
    void setX(const qint32 &value);

    qint32 getY() const;
    void setY(const qint32 &value);
    /**
     * @brief loadSprites loads all sprites we won't to show. Updates all sprite data
     */
    void loadSprites();
public slots:
    void loadBaseTerrain(QString terrainID);
    void loadBaseSprite(QString spriteID);
    QString getSurroundings(QString list, bool blacklist = false, qint32 searchType = SearchTypes::Direct);
    void loadOverlaySprite(spriteID);
private:
    /**
     * @brief terrainID our terrain id
     */
    QString terrainID;
    /**
     * @brief terrainSprite which terrain sprite we loaded
     */
    QString terrainSprite;
    /**
     * @brief m_pTerrainSprite actor holding our sprite data
     */
    oxygine::spSprite m_pTerrainSprite{nullptr};
    QVector<oxygine::spSprite> m_pOverlaySprites;
    /**
     * @brief fixedSprite stores if the selected sprite is fixed or will be loaded based on the surrounding and random factors
     */
    bool fixedSprite;
    /**
      * @brief defines if this terrain is animated or not
      */
    bool Animated{false};
    /**
     * @brief m_pBaseTerrain base terrain of this terrain
     */
    Terrain* m_pBaseTerrain{nullptr};
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
