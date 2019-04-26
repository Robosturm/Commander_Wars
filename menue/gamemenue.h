#ifndef GAMEMENUE_H
#define GAMEMENUE_H

#include <QObject>
#include <menue/ingamemenue.h>
#include "game/gameaction.h"
#include "game/gamemap.h"
#include "game/playerinfo.h"
#include "objects/ingameinfobar.h"

/**
 * @brief The GameMenue class handles the game :)
 */
class GameMenue : public InGameMenue
{
    Q_OBJECT
public:
    explicit GameMenue(qint32 startPlayer = 0);
    explicit GameMenue(QString map);
    virtual ~GameMenue();

    static GameMenue* getInstance()
    {
        return m_pInstance;
    }

    /**
     * @brief startGame
     * @param startPlayer
     */
    void startGame(qint32 startPlayer);
signals:
    void sigRightClick(qint32 x, qint32 y);
    void sigLeftClick(qint32 x, qint32 y);    
    void sigActionPerformed();
public slots:
    /**
     * @brief actionPerformed
     */
    void actionPerformed();
    /**
     * @brief performAction performs the given action and deletes it afterwards.
     * @param pGameAction
     */
    void performAction(GameAction* pGameAction);
    /**
     * @brief updatePlayerinfo
     */
    void updatePlayerinfo();
    /**
     * @brief victory
     * @param team
     */
    void victory(qint32 team);
    /**
     * @brief saveGame
     */
    void saveGame();
    /**
     * @brief saveMap
     * @param file
     */
    void saveMap(QString filename);
    /**
     * @brief exitGame
     */
    void exitGame();
protected:
    void loadGameMenue();
private:
    spPlayerInfo m_pPlayerinfo;
    spIngameInfoBar m_IngameInfoBar;
    static GameMenue* m_pInstance;
};

#endif // GAMEMENUE_H
