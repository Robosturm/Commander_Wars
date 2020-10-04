#ifndef DIALOGPASSWORD_H
#define DIALOGPASSWORD_H

#include <QObject>

#include "oxygine-framework.h"

#include "objects/passwordbox.h"

class DialogPassword;
typedef oxygine::intrusive_ptr<DialogPassword> spDialogPassword;

class DialogPassword : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogPassword(QString text, bool showCancel, QString startInput);
    virtual ~DialogPassword() = default;

signals:
    void sigTextChanged(QString text);
    void sigCancel();
public slots:

private:
    oxygine::spButton m_OkButton;
    oxygine::spButton m_CancelButton;
    spPasswordbox m_pTextbox;

};

#endif // DIALOGPASSWORD_H
