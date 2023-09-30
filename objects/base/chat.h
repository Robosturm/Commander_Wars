#ifndef CHAT_H
#define CHAT_H

#include <QObject>
#include <QJsonObject>

#include "3rd_party/oxygine-framework/oxygine/actor/Actor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"

#include "network/networkInterface.h"

#include "objects/base/panel.h"
#include "objects/base/textbox.h"

class BaseGamemenu;
class Chat;
using spChat = std::shared_ptr<Chat>;

class Chat final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit Chat(spNetworkInterface pInterface, QSize size, NetworkInterface::NetworkSerives serviceMode, BaseGamemenu* pMenu);
    virtual ~Chat() = default;
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
    void addMessage(QJsonObject data, bool local = false);

private:
    QStringList m_messages;
    spNetworkInterface m_pInterface{nullptr};
    spPanel m_Panel;
    oxygine::spTextField m_Chat;
    spTextbox m_ChatInput;
    oxygine::spButton m_Send;
    NetworkInterface::NetworkSerives m_serviceMode{NetworkInterface::NetworkSerives::GameChat};
    BaseGamemenu* m_pMenu{nullptr};
    static const qint32 m_bufferSize = 500;
};

#endif // CHAT_H
