#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include "network/NetworkInterface.h"

class RxTask;
class TxTask;
class QTcpSocket;

class TCPClient : public NetworkInterface
{
    Q_OBJECT
public:
    TCPClient();
    virtual ~TCPClient();


public slots:
    virtual void connectTCP(const QString& adress, quint16 port) override;
    virtual void disconnectTCP() override;
    /**
     * @brief sendData send Data with this Connection
     * @param data
     */
    virtual void sendData(QByteArray data, Mainapp::NetworkSerives service, bool blocking) override;

    virtual void sessionOpened(quint16 port) override;
signals:
    void sig_sendData(QByteArray data, Mainapp::NetworkSerives service, bool blocking);

private:
    RxTask* pRXTask;
    TxTask* pTXTask;
    QTcpSocket* pSocket;
};

#endif // TCPCLIENT_H
