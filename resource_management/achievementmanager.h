#ifndef ACHIEVEMENTMANAGER_H
#define ACHIEVEMENTMANAGER_H

#include "resource_management/ressourcemanagement.h"

class AchievementManager : public QObject, public RessourceManagement<AchievementManager>
{
    Q_OBJECT
public:
    /**
     * @brief loadAll loads all achievement data
     */
    virtual void loadAll() override;
    /**
     * @brief victory
     */
    void onVictory(qint32 team, bool humanWin);
protected:
    friend RessourceManagement<AchievementManager>;
    AchievementManager();

private:
    virtual ~AchievementManager() = default;
};
#endif // ACHIEVEMENTMANAGER_H
