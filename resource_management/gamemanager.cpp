#include "gamemanager.h"

GameManager::GameManager()
    : RessourceManagement<GameManager>("/images/game/res.xml",
                                       "scripts/actions")
{
}
