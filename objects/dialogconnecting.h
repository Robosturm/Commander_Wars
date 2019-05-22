#ifndef DIALOGCONNECTING_H
#define DIALOGCONNECTING_H

#include <QObject>
#include <qtimer.h>

#include "oxygine-framework.h"

class DialogConnecting;
typedef oxygine::intrusive_ptr<DialogConnecting> spDialogConnecting;

class DialogConnecting : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogConnecting(QString text);

signals:
    void sigCancel();
public slots:
    void cancel();
    void connected();
    void timeout();
private:
     oxygine::spButton m_CancelButton;
     oxygine::spTextField m_Text;
     QString m_Message;
     QTimer m_Timer;
     qint8 counter;
};

#endif // DIALOGCONNECTING_H
