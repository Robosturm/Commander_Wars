#ifndef FIELDINFO_H
#define FIELDINFO_H

#include <QObject>

#include "oxygine-framework.h"

#include "game/building.h"
#include "game/unit.h"
#include "game/terrain.h"

class FieldInfo;
typedef oxygine::intrusive_ptr<FieldInfo> spFieldInfo;

class FieldInfo : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit FieldInfo(Terrain* pTerrain, Unit* pUnit);

signals:
    void sigFinished();
public slots:
    void keyInput(SDL_Event event);
private:
    oxygine::spButton m_OkButton;
};

#endif // FIELDINFO_H
