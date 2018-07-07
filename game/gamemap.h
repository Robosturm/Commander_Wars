#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <QObject>
#include <QVector>

#include "oxygine/core/ref_counter.h"
#include "oxygine-framework.h"


#include "memory"

#include "game/terrain.h"

typedef oxygine::intrusive_ptr<Terrain> spTerrain;


class GameMap : public QObject, public oxygine::Actor
{
    Q_OBJECT

    Q_PROPERTY(qint32 width READ getWidth WRITE setWidth)
public:
    static const qint32 frameTime;
    static const qint32 Imagesize = 24;
    enum Directions
    {
        None = -1,
        North,
        NorthEast,
        East,
        SouthEast,
        South,
        SouthWest,
        West,
        NorthWest,
        All,
        Direct,
        Diagnonal
    };

    explicit GameMap(qint32 width, qint32 heigth);
    explicit GameMap(QString map);
    virtual ~GameMap();

    qint32 getWidth() const;
    void setWidth(const qint32 &value);

    qint32 getHeigth() const;
    void setHeigth(const qint32 &value);

    inline static GameMap* getInstance()
    {
        return m_pInstance;
    }
    /**
     * @brief getTerrain
     * @param x
     * @param y
     * @return normal pointer only use this for jsengine access
     */
    inline Terrain* getTerrain(qint32 x, qint32 y)
    {
        return fields.at(y)->at(x).get();
    }
    /**
     * @brief getTerrain
     * @param x
     * @param y
     * @return smart pointer to the terrain use this when ever you can
     */
    inline spTerrain getSpTerrain(qint32 x, qint32 y)
    {
        return fields.at(y)->at(x);
    }

signals:

public slots:
    void updateTerrainSprites();
    /**
     * @brief getField changes the coordinates into the given direction
     * @param x x-coordinates
     * @param y y-coordinates
     * @param direction the suitable direction
     */
    static void getField(qint32& x, qint32& y, Directions direction);
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
    void moveMap(qint32 x, qint32 y);
private:
    static GameMap* m_pInstance;
    float m_zoom{1.0f};
    qint32 width{0};
    qint32 heigth{0};
    QVector<QVector<spTerrain>*> fields;

    static const QString m_JavascriptName;

};

#endif // GAMEMAP_H
