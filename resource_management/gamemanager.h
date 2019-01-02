#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H


#include "oxygine-framework.h"

class GameManager : public oxygine::Resources
{
public:
    static GameManager* getInstance();

    /**
     * @brief loadAll loads all movement table data
     */
    void loadAll();
private:
    explicit GameManager();
    virtual ~GameManager() = default;
    static GameManager* m_pInstance;
};

#endif // GAMEMANAGER_H
