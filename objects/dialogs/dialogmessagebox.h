#ifndef DIALOGMESSAGEBOX_H
#define DIALOGMESSAGEBOX_H

#include <QObject>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

class DialogMessageBox;
using spDialogMessageBox = oxygine::intrusive_ptr<DialogMessageBox>;

class DialogMessageBox : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogMessageBox(QString text, bool withCancel = false, QString confirmText = tr("Ok"), QString cancelText = tr("Cancel"));
    virtual ~DialogMessageBox() = default;
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
