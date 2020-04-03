#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "resource_management/RessourceManagement.h"

class GameManager : public QObject, public RessourceManagement<GameManager>
{
    Q_OBJECT
protected:
    friend RessourceManagement<GameManager>;
    GameManager();
private:
    virtual ~GameManager() = default;
};

#endif // GAMEMANAGER_H
