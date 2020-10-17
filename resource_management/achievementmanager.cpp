#include "achievementmanager.h"

AchievementManager::AchievementManager()
    : RessourceManagement<AchievementManager>("/images/achievements/res.xml",
                                              "/scripts/achievements")
{
}
