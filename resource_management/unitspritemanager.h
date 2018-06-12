#ifndef UNITSPRITEMANAGER_H
#define UNITSPRITEMANAGER_H

#include "oxygine-framework.h"

class UnitSpriteManager : public oxygine::Resources
{
public:
    static UnitSpriteManager* getInstance();

private:
    explicit UnitSpriteManager();
    virtual ~UnitSpriteManager() = default;

    static UnitSpriteManager* m_pInstance;
};

#endif // UNITSPRITEMANAGER_H
