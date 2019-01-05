#ifndef GAMEANIMATIONMANAGER_H
#define GAMEANIMATIONMANAGER_H

#include "oxygine-framework.h"

class GameAnimationManager : public oxygine::Resources
{

public:
    static GameAnimationManager* getInstance();
private:
    explicit GameAnimationManager();
    virtual ~GameAnimationManager() = default;
    static GameAnimationManager* m_pInstance;
};

#endif // GAMEANIMATIONMANAGER_H
