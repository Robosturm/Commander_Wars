#ifndef TERRAIN_H
#define TERRAIN_H

#include <QObject>

#include "oxygine-framework.h"

class Terrain : public QObject, public oxygine::Actor
{
    Q_OBJECT
    Q_PROPERTY(bool Animated READ getAnimated WRITE setAnimated)
public:
    explicit Terrain(const QString& terrainID, qint32 x, qint32 y);
    virtual ~Terrain() = default;

    bool getAnimated() const;
    void setAnimated(bool value);

    qint32 getX() const;
    void setX(const qint32 &value);

    qint32 getY() const;
    void setY(const qint32 &value);

public slots:
    void loadBaseTerrainSprite(QString spriteID);
private:
    QString terrainID;



    bool Animated{false};
    qint32 x;
    qint32 y;

};

#endif // TERRAIN_H
