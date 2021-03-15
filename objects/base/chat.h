#ifndef CHAT_H
#define CHAT_H

#include <QObject>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "network/NetworkInterface.h"

#include "objects/base/panel.h"
#include "objects/base/textbox.h"

class Chat;
typedef oxygine::intrusive_ptr<Chat> spChat;

class Chat : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit Chat(spNetworkInterface pInterface, QSize size, NetworkInterface::NetworkSerives serviceMode);

    virtual void update(const oxygine::UpdateState& us) override;
    /**
     * @brief setVisible
     * @param vis
     */
    virtual void setVisible(bool vis) override;
signals:
    void sigSendText(QString message);
public slots:
    void dataRecieved(quint64, QByteArray data, NetworkInterface::NetworkSerives service);
    void sendData(QString message);
private:
    QVector<QString> messages;
    spNetworkInterface m_pInterface{nullptr};
    spPanel m_Panel;
    oxygine::spTextField m_Chat;
    spTextbox m_ChatInput;
    oxygine::spButton m_Send;

    void addMessage(QString message, bool local = false);

    NetworkInterface::NetworkSerives m_serviceMode{NetworkInterface::NetworkSerives::GameChat};

    static const qint32 bufferSize = 500;
};

#endif // CHAT_H
