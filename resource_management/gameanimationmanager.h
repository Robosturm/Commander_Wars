#ifndef GAMEANIMATIONMANAGER_H
#define GAMEANIMATIONMANAGER_H

#include "resource_management/ressourcemanagement.h"

class GameAnimationManager : public QObject, public RessourceManagement<GameAnimationManager>
{
    Q_OBJECT
protected:
    friend RessourceManagement<GameAnimationManager>;
    GameAnimationManager();
private:
    virtual ~GameAnimationManager() = default;
};

#endif // GAMEANIMATIONMANAGER_H
