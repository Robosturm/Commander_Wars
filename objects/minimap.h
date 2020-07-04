#ifndef MINIMAP_H
#define MINIMAP_H

#include <QObject>
#include "oxygine-framework.h"

class GameMap;
typedef oxygine::intrusive_ptr<GameMap> spGameMap;

class Minimap;
typedef oxygine::intrusive_ptr<Minimap> spMinimap;

class Minimap : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit Minimap();
    virtual ~Minimap();


signals:
    void clicked(qint32 x, qint32 y);
public slots:
    void updateMinimap(spGameMap pMap, bool useVision = false);
private:
    static const qint32 ImageSize{4};
};

#endif // MINIMAP_H
