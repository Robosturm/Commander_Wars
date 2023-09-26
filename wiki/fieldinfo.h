#ifndef FIELDINFO_H
#define FIELDINFO_H

#include <QObject>

#include "wiki/wikipage.h"

#include "game/unit.h"
#include "game/terrain.h"

class FieldInfo;
using spFieldInfo = std::shared_ptr<FieldInfo>;

class FieldInfo final : public Wikipage
{
    Q_OBJECT
public:
    explicit FieldInfo(spTerrain pTerrain, spUnit pUnit);
    virtual ~FieldInfo() = default;
};

#endif // FIELDINFO_H
