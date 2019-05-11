#ifndef CHAT_H
#define CHAT_H

#include <QObject>

#include "oxygine-framework.h"

#include "network/NetworkInterface.h"



class Chat : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit Chat(NetworkInterface* pInterface);

signals:

public slots:
    void dataRecieved(QByteArray data, Mainapp::NetworkSerives service);
private:
    QVector<QString> messages;
    NetworkInterface* m_pInterface{nullptr};
    oxygine::spTextField m_Chat;
    oxygine::TextField m_ChatInput;

    static const qint32 bufferSize = 500;
};

#endif // CHAT_H
