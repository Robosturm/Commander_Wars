#ifndef MAINSERVER_H
#define MAINSERVER_H

#include <QObject>
#include "qprocess.h"

#include "network/tcpserver.h"
#include "network/tcpclient.h"

#include "network/networkgamedata.h"

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

public slots:
    void recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service);
private:
    void spawnSlaveGame(QDataStream & stream, quint64 socketID);
    quint16 getNextPort();
    bool validHostRequest(QStringList mods);
private:
    explicit MainServer();
    static MainServer* m_pInstance;
    TCPServer* m_pGameServer{nullptr};

    QVector<spNetworkGameData> m_networkGames;
    QVector<TCPClient*> m_Client;
    QVector<QProcess*> m_Processes;
};

#endif // MAINSERVER_H
