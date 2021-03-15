#ifndef DIALOGTEXTINPUT_H
#define DIALOGTEXTINPUT_H

#include <QObject>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "objects/base/textbox.h"

class DialogTextInput;
typedef oxygine::intrusive_ptr<DialogTextInput> spDialogTextInput;

class DialogTextInput : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogTextInput(QString text, bool showCancel, QString startInput);
    virtual ~DialogTextInput() = default;

signals:
    void sigTextChanged(QString text);
    void sigCancel();
public slots:

private:
    oxygine::spButton m_OkButton;
    oxygine::spButton m_CancelButton;
    spTextbox m_pTextbox;
};

#endif // DIALOGTEXTINPUT_H
