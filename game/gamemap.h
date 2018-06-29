#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <QObject>
#include <QVector>

#include "oxygine-framework.h"

#include "memory"

class Terrain;

class GameMap : public QObject, public oxygine::Actor
{
    Q_OBJECT

    Q_PROPERTY(QVector<QVector<Terrain*>*> fields READ getFields)
    Q_PROPERTY(qint32 width READ getWidth WRITE setWidth)
public:
    static const qint32 frameTime;
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

    QVector<QVector<Terrain *> *> getFields() const;


    qint32 getWidth() const;
    void setWidth(const qint32 &value);

    qint32 getHeigth() const;
    void setHeigth(const qint32 &value);

    inline static GameMap* getInstance()
    {
        return m_pInstance;
    }
    inline Terrain* getTerrain(qint32 x, qint32 y)
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
private:
    static GameMap* m_pInstance;

    qint32 width{0};
    qint32 heigth{0};
    QVector<QVector<Terrain*>*> fields;

    static const QString m_JavascriptName;

};

#endif // GAMEMAP_H
