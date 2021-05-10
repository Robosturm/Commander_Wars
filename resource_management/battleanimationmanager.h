#ifndef BATTLEANIMATIONMANAGER_H
#define BATTLEANIMATIONMANAGER_H

#include "resource_management/ressourcemanagement.h"

class BattleAnimationManager : public QObject, public RessourceManagement<BattleAnimationManager>
{
    Q_OBJECT
protected:
    friend RessourceManagement<BattleAnimationManager>;
    BattleAnimationManager();
private:
    virtual ~BattleAnimationManager() = default;
};

#endif // BATTLEANIMATIONMANAGER_H
