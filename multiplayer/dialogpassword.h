#ifndef DIALOGPASSWORD_H
#define DIALOGPASSWORD_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"

#include "objects/base/passwordbox.h"

class DialogPassword;
using spDialogPassword = oxygine::intrusive_ptr<DialogPassword>;

class DialogPassword final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogPassword(QString text, bool showCancel, QString startInput);
   virtual ~DialogPassword() = default;

signals:
    void sigTextChanged(QString text);
    void sigCancel();
    void sigFinished();
private slots:
    void remove();
private:
    oxygine::spButton m_OkButton;
    oxygine::spButton m_CancelButton;
    spPasswordbox m_pTextbox;

};

#endif // DIALOGPASSWORD_H
