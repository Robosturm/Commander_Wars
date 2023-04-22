#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <QObject>
#include <QElapsedTimer>

#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"

#include "objects/base/textinput.h"

class Textbox;
using spTextbox = oxygine::intrusive_ptr<Textbox>;

class Textbox : public TextInput
{
    Q_OBJECT
public:
    explicit Textbox(qint32 width, qint32 heigth = -1);
    virtual ~Textbox() = default;
    virtual void update(const oxygine::UpdateState& us) override;
    /**
     * @brief setEnabled
     * @param value
     */
    virtual void setEnabled(bool value) override;
signals:
    void sigTextChanged(QString text);
    void sigEnterPressed(QString text);
public slots:
    virtual void focusedLost() override;

protected:
    virtual bool onEditFinished() override;
protected:
    oxygine::spBox9Sprite m_Textbox;
    oxygine::spTextField m_Textfield;
private:
    QString m_lastDrawText;
};

Q_DECLARE_INTERFACE(Textbox, "Textbox");

#endif // TEXTBOX_H
