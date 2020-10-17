#ifndef ACHIEVEMENTMANAGER_H
#define ACHIEVEMENTMANAGER_H

#include "resource_management/RessourceManagement.h"

class AchievementManager : public QObject, public RessourceManagement<AchievementManager>
{
    Q_OBJECT
protected:
    friend RessourceManagement<AchievementManager>;
    AchievementManager();
private:
    virtual ~AchievementManager() = default;
};
#endif // ACHIEVEMENTMANAGER_H
