#ifndef GAMEANIMATIONMANAGER_H
#define GAMEANIMATIONMANAGER_H

#include "resource_management/ressourcemanagement.h"

class GameAnimationManager final : public QObject, public RessourceManagement<GameAnimationManager>
{
    Q_OBJECT
protected:
    friend MemoryManagement;
    GameAnimationManager();
private:
    virtual ~GameAnimationManager() = default;
};

#endif // GAMEANIMATIONMANAGER_H
