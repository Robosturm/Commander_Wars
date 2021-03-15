#ifndef DIALOGMESSAGEBOX_H
#define DIALOGMESSAGEBOX_H

#include <QObject>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

class DialogMessageBox;
typedef oxygine::intrusive_ptr<DialogMessageBox> spDialogMessageBox;

class DialogMessageBox : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogMessageBox(QString text, bool withCancel = false);

signals:
    void sigOk();
    void sigCancel();
public slots:
    void remove();
private:
     oxygine::spButton m_OkButton;
     oxygine::spButton m_CancelButton;
     oxygine::spTextField m_Text;
     QString m_Message;
};

#endif // DIALOGCONNECTING_H
