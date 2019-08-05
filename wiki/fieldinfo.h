#ifndef FIELDINFO_H
#define FIELDINFO_H

#include <QObject>

#include "wiki/wikipage.h"

#include "game/building.h"
#include "game/unit.h"
#include "game/terrain.h"

class FieldInfo;
typedef oxygine::intrusive_ptr<FieldInfo> spFieldInfo;

class FieldInfo : public Wikipage
{
    Q_OBJECT
public:
    explicit FieldInfo(Terrain* pTerrain, Unit* pUnit);

signals:
private:
};

#endif // FIELDINFO_H
