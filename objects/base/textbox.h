#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <QObject>
#include <QElapsedTimer>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "objects/base/tooltip.h"

#include "3rd_party/oxygine-framework/oxygine/KeyEvent.h"

class Textbox;
typedef oxygine::intrusive_ptr<Textbox> spTextbox;

class Textbox : public Tooltip
{
    Q_OBJECT
public:
    enum
    {
        BLINKFREQG = 250
    };
    explicit Textbox(qint32 width, qint32 heigth = -1);
    virtual ~Textbox() = default;
    /**
     * @brief getCurrentText
     * @return the current text of the textbox
     */
    inline QString getCurrentText()
    {
        return m_Text.trimmed();
    }
    virtual void update(const oxygine::UpdateState& us) override;
    /**
     * @brief setCurrentText changes the text of this textbox
     * @param text
     */
    void setCurrentText(QString text);
    /**
     * @brief setEnabled
     * @param value
     */
    virtual void setEnabled(bool value) override;
    virtual bool keyInputMethodQueryEvent(QInputMethodQueryEvent *event) override;
signals:
    void sigTextChanged(QString text);
    void sigEnterPressed(QString text);
public slots:
    void KeyInput(oxygine::KeyEvent event);
    virtual void focusedLost() override;
protected:
    virtual void focused() override;
    virtual void looseFocusInternal() override;
    void handleTouchInput(oxygine::KeyEvent event);
protected:

    oxygine::spBox9Sprite m_Textbox;
    oxygine::spTextField m_Textfield;
    QString m_Text;
    QElapsedTimer m_toggle;
    qint32 m_curmsgpos{0};
    qint32 m_preeditSize{0};
    qint32 m_editPos{0};
};

#endif // TEXTBOX_H
