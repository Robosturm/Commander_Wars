#ifndef NETWORKGAME_H
#define NETWORKGAME_H

#include <QObject>

#include "network/tcpclient.h"

class NetworkGame : public QObject
{
    Q_OBJECT
public:
    explicit NetworkGame();

signals:

public slots:
    void disconnected(quint64 socketID);
    void recieveData(quint64 socketID, QByteArray data, NetworkInterface::NetworkSerives service, quint64 targetSocket);
private:
    TCPClient* m_pHost;
    quint64 m_HostId{0};
    QVector<TCPClient*> m_pClients;
    QVector<quint64> m_pClientIDs;
};

#endif // NETWORKGAME_H
