#ifndef CHAT_H
#define CHAT_H

#include <QObject>

#include <QMutex>

#include "oxygine-framework.h"

#include "network/NetworkInterface.h"

#include "objects/panel.h"
#include "objects/textbox.h"

class Chat;
typedef oxygine::intrusive_ptr<Chat> spChat;

class Chat : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit Chat(NetworkInterface* pInterface, QSize size);

    virtual void update(const oxygine::UpdateState& us) override;
signals:
    void sigSendText(QString message);
public slots:
    void dataRecieved(QByteArray data, Mainapp::NetworkSerives service);
    void sendData(QString message);
private:
    QVector<QString> messages;
    NetworkInterface* m_pInterface{nullptr};
    spPanel m_Panel;
    oxygine::spTextField m_Chat;
    spTextbox m_ChatInput;
    oxygine::spButton m_Send;
    QMutex datalocker;

    void addMessage(QString message);

    static const qint32 bufferSize = 500;
};

#endif // CHAT_H
