#ifndef DIALOGCONNECTING_H
#define DIALOGCONNECTING_H

#include <QObject>
#include <QTimer>

#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"
#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"

class DialogConnecting;
using spDialogConnecting = std::shared_ptr<DialogConnecting>;

class DialogConnecting final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogConnecting(QString text, qint32 timeoutMs, bool showCancel = true);
   virtual ~DialogConnecting() = default;
signals:
    void sigCancel();
    void sigConnected();
public slots:
    void cancel();
    void connected();
    void timeout();
    void connectionTimeout();
private:
     oxygine::spButton m_CancelButton;
     oxygine::spTextField m_Text;
     QString m_Message;
     QTimer m_Timer;
     QTimer m_TimerConnectionTimeout;
     quint8 m_counter;
};

#endif // DIALOGCONNECTING_H
