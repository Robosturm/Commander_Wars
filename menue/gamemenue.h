#ifndef GAMEMENUE_H
#define GAMEMENUE_H

#include <QObject>
#include <menue/ingamemenue.h>
#include "game/gameaction.h"
#include "game/gamemap.h"
#include "game/playerinfo.h"
#include "objects/ingameinfobar.h"

#include "network/NetworkInterface.h"

/**
 * @brief The GameMenue class handles the game :)
 */
class GameMenue : public InGameMenue
{
    Q_OBJECT
public:
    explicit GameMenue(spNetworkInterface pNetworkInterface = nullptr);
    explicit GameMenue(QString map);
    virtual ~GameMenue();

    static GameMenue* getInstance()
    {
        return m_pInstance;
    }
    /**
     * @brief attachInterface
     * @param pNetworkInterface
     */
    void attachInterface(spNetworkInterface pNetworkInterface);
    /**
     * @brief startGame
     * @param startPlayer
     */
    void startGame();
    /**
     * @brief getGameStarted
     * @return always true for singleplayer games turns true on multiplayer games once all clients have started the game
     */
    bool getGameStarted() const;

signals:
    void sigActionPerformed();
    void sigGameStarted();
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
     * @brief victoryInfo
     */
    void victoryInfo();
    /**
     * @brief showCOInfo
     */
    void showCOInfo();
    /**
     * @brief saveMap
     * @param file
     */
    void saveMap(QString filename);
    /**
     * @brief exitGame
     */
    void exitGame();
    /**
     * @brief keyInput
     * @param event
     */
    virtual void keyInput(SDL_Event event) override;
    /**
     * @brief editFinishedCanceled
     */
    void editFinishedCanceled();
    /**
     * @brief recieveData
     * @param socketID
     * @param data
     * @param service
     */
    void recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service);
    /**
     * @brief disconnected
     * @param socketID
     */
    void disconnected(quint64 socketID);
    /**
     * @brief isNetworkGame
     * @return
     */
    bool isNetworkGame();
    /**
     * @brief playerJoined
     * @param socketID
     */
    void playerJoined(quint64 socketID);
protected:
    void loadGameMenue();
private:
    spPlayerInfo m_pPlayerinfo;
    spIngameInfoBar m_IngameInfoBar;
    static GameMenue* m_pInstance;
    spNetworkInterface m_pNetworkInterface;
    bool gameStarted{false};
    QVector<quint64> m_PlayerSockets;
    QVector<quint64> m_ReadySockets;
};

#endif // GAMEMENUE_H
