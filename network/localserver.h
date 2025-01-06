#ifndef LOCALSERVER_H
#define LOCALSERVER_H

#include <QLocalServer>
#include <QVector>

#include "network/rxtask.h"
#include "network/txtask.h"
#include "network/networkInterface.h"

class LocalServer;
using spLocalServer = std::shared_ptr<LocalServer>;
using spQLocalServer = std::shared_ptr<QLocalServer>;

/**
 * @brief The LocalServer class for connecting a local pipe to the hosted game
 */
class LocalServer final : public NetworkInterface
{
    Q_OBJECT
public:
    explicit LocalServer(QObject* pParent);
    virtual ~LocalServer();
public slots:
    virtual void connectTCP(QString primaryAdress, quint16 port, QString secondaryAdress, bool sendAll = false) override;
    virtual void disconnectTCP() override;
    virtual void forwardData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service) override;
    virtual QVector<quint64> getConnectedSockets() override;
    virtual void changeThread(quint64 socketID, QThread* pThread) override;
    void onConnect();
    void disconnectClient(quint64 socketID);
    void pauseListening();
    void continueListening();
    /**
     * @brief addSocket
     * @param socket
     */
    void addSocket(quint64 socket);
    /**
     * @brief removeSocket
     * @param socket
     */
    void removeSocket(quint64 socket);
protected:
    void displayDetailedError() override;
private:
    QVector<spRxTask> m_pRXTasks;
    QVector<spTxTask> m_pTXTasks;
    QVector<QLocalSocket*> m_pTCPSockets;
    QVector<quint64> m_SocketIDs;
    quint64 m_idCounter = 0;
    spQLocalServer m_pTCPServer{nullptr};
    bool m_gameServer{false};
};

#endif // LOCALSERVER_H
