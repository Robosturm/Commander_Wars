#ifndef BATTLEANIMATIONMANAGER_H
#define BATTLEANIMATIONMANAGER_H

#include "resource_management/ressourcemanagement.h"

class BattleAnimationManager final : public QObject, public RessourceManagement<BattleAnimationManager>
{
    Q_OBJECT
protected:
    friend RessourceManagement<BattleAnimationManager>;
    BattleAnimationManager();
private:
    ~BattleAnimationManager() = default;
};

#endif // BATTLEANIMATIONMANAGER_H
