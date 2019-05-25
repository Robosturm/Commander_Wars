#ifndef DIALOGMESSAGEBOX_H
#define DIALOGMESSAGEBOX_H

#include <QObject>

#include "oxygine-framework.h"

class DialogMessageBox;
typedef oxygine::intrusive_ptr<DialogMessageBox> spDialogMessageBox;

class DialogMessageBox : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogMessageBox(QString text);

signals:
    void sigOk();
public slots:
    void ok();
private:
     oxygine::spButton m_OkButton;
     oxygine::spTextField m_Text;
     QString m_Message;
};

#endif // DIALOGCONNECTING_H
