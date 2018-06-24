#ifndef GAMEMAP_H
#define GAMEMAP_H

#include <QObject>
#include <QVector>

#include "oxygine-framework.h"

#include "game/terrain.h"

class GameMap : public QObject, public oxygine::Actor
{
    Q_OBJECT

    Q_PROPERTY(QVector<QVector<Terrain*>*> fields READ getFields)
    Q_PROPERTY(qint32 width READ getWidth WRITE setWidth)
public:
    static const qint32 frameTime;
    enum Directions
    {
        North = 0,
        NorthEast,
        East,
        SouthEast,
        South,
        SouthWest,
        West,
        NorthWest,
    }

    explicit GameMap(qint32 width, qint32 heigth);
    explicit GameMap(QString map);
    virtual ~GameMap();

    QVector<QVector<Terrain *> *> getFields() const;


    qint32 getWidth() const;
    void setWidth(const qint32 &value);

    qint32 getHeigth() const;
    void setHeigth(const qint32 &value);

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
private:
    qint32 width{0};
    qint32 heigth{0};
    QVector<QVector<Terrain*>*> fields;

    static const QString m_JavascriptName;

};

#endif // GAMEMAP_H
