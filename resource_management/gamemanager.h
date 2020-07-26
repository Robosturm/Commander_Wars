#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "resource_management/RessourceManagement.h"

class GameManager : public QObject, public RessourceManagement<GameManager>
{
    Q_OBJECT
public:
    QString getActionIcon(QString actionID);
    oxygine::spSprite getIcon(QString icon);
    /**
     * @brief getDescription
     * @param position
     * @return
     */
    QString getDescription(qint32 position);
protected:
    friend RessourceManagement<GameManager>;
    GameManager();
private:
    virtual ~GameManager() = default;
    oxygine::spSprite getIconSprite(QString icon);
};

#endif // GAMEMANAGER_H
