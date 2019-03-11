#ifndef MINIMAP_H
#define MINIMAP_H

#include <QObject>
#include "oxygine-framework.h"

class GameMap;

class Minimap;
typedef oxygine::intrusive_ptr<Minimap> spMinimap;

class Minimap : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit Minimap();
    virtual ~Minimap();


signals:

public slots:
    void updateMinimap(GameMap* pMap);
private:
    static const qint32 ImageSize{4};
};

#endif // MINIMAP_H
