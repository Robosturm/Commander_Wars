#ifndef MAINSERVER_H
#define MAINSERVER_H

#include <QObject>
#include "qprocess.h"

#include "network/tcpserver.h"
#include "network/tcpclient.h"

#include "network/networkgamedata.h"
#include "network/networkgame.h"

/**
 * @brief The MainServer class handling the server and it's data.
 */
class MainServer : public QObject
{
    Q_OBJECT
public:
    static MainServer* getInstance();

    virtual ~MainServer();

    inline TCPServer* getGameServer()
    {
        return m_pGameServer;
    }
    inline void stopGameServer()
    {
        m_pGameServer->deleteLater();
        m_pGameServer = nullptr;
    }    
signals:
    void sigRemoveGame(NetworkGame* pGame);
public slots:
    void recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service);
    /**
     * @brief updateGameData
     */
    void updateGameData();
    /**
     * @brief sendGameDataUpdate
     */
    void sendGameDataUpdate();
    /**
     * @brief playerJoined
     * @param socketId
     */
    void playerJoined(qint64 socketId);
private slots:
    void removeGame(NetworkGame* pGame);
private:
    void spawnSlaveGame(QDataStream & stream, quint64 socketID, QByteArray& data);
    bool validHostRequest(QStringList mods);
    void sendGameDataToClient(qint64 socketId);
    void joinSlaveGame(quint64 socketID, QDataStream & stream);
    void closeGame(NetworkGame* pGame);
private:
    class InternNetworkGame;
    typedef oxygine::intrusive_ptr<InternNetworkGame> spInternNetworkGame;
    class InternNetworkGame : public oxygine::ref_counter
    {
    public:
        QProcess* process{nullptr};
        NetworkGame game;
        QThread m_runner;
    };
    explicit MainServer();

private:
    static MainServer* m_pInstance;    
    TCPServer* m_pGameServer{nullptr};
    quint64 m_slaveGameIterator{0};
    // data for games currently run on the server
    QVector<spInternNetworkGame> m_games;
    QTimer m_updateTimer;
    bool m_updateGameData{false};
};

#endif // MAINSERVER_H
