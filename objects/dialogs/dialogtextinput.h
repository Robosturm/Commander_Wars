#ifndef DIALOGTEXTINPUT_H
#define DIALOGTEXTINPUT_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"

#include "objects/base/textbox.h"

class DialogTextInput;
using spDialogTextInput = std::shared_ptr<DialogTextInput>;

class DialogTextInput final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit DialogTextInput(QString text, bool showCancel, QString startInput);
    ~DialogTextInput() = default;

signals:
    void sigTextChanged(QString text);
    void sigCancel();
    void sigFinished();
private slots:
    void remove();
private:
    oxygine::spButton m_OkButton;
    oxygine::spButton m_CancelButton;
    spTextbox m_pTextbox;
};

#endif // DIALOGTEXTINPUT_H
