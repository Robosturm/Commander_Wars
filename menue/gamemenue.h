#ifndef GAMEMENUE_H
#define GAMEMENUE_H

#include <QObject>
#include <menue/ingamemenue.h>
#include "game/gameaction.h"
#include "game/gamemap.h"

/**
 * @brief The GameMenue class handles the game :)
 */
class GameMenue : public InGameMenue
{
    Q_OBJECT
public:
    GameMenue(QString map);

    void startGame(qint32 startPlayer);
signals:
    void sigRightClick(qint32 x, qint32 y);
    void sigLeftClick(qint32 x, qint32 y);
public slots:
    /**
     * @brief performAction performs the given action and deletes it afterwards.
     * @param pGameAction
     */
    void performAction(GameAction* pGameAction);
private:
};

#endif // GAMEMENUE_H
