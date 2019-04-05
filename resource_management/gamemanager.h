#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QStringList>

#include "oxygine-framework.h"

class GameManager : public oxygine::Resources
{
public:
    static GameManager* getInstance();

    /**
     * @brief loadAll loads all movement table data
     */
    void loadAll();

    bool getLoaded()
    {
        return loaded;
    }
private:
    explicit GameManager();
    virtual ~GameManager() = default;
    static GameManager* m_pInstance;
    QStringList m_loadedActions;
    bool loaded{false};
};

#endif // GAMEMANAGER_H
