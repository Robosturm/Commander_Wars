#include "coperkmanager.h"

COPerkManager::COPerkManager()
    : RessourceManagement<COPerkManager>("/images/co_perks/res.xml",
                                         "/scripts/co_perks")
{
}
