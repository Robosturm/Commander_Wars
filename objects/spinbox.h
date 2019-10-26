#ifndef SPINBOX_H
#define SPINBOX_H

#include <QObject>
#include <QTime>
#include "oxygine-framework.h"

#include "oxygine/KeyEvent.h"

class SpinBox;
typedef oxygine::intrusive_ptr<SpinBox> spSpinBox;

class SpinBox : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    enum
    {
        BLINKFREQG = 250
    };
    enum Mode
    {
        Int,
        Float,
        Real,
    };

    explicit SpinBox(qint32 width, qint32 min = 0, qint32 max = 100, Mode mode = Mode::Int);
    /**
     * @brief getCurrentValue
     * @return the current value of the spin box
     */
    qreal getCurrentValue();
    virtual void update(const oxygine::UpdateState& us) override;
    /**
     * @brief setCurrentValue changes the value of this spin box
     * @param text
     */
    void setCurrentValue(qreal value);
    qreal getSpinSpeed() const;
    void setSpinSpeed(qreal SpinSpeed);

    qreal getInfinityValue() const;
    void setInfinityValue(qreal InfinityValue);

    virtual void setEnabled(bool value) override;
signals:
    void sigValueChanged(qreal value);
public slots:
    void TextInput(oxygine::KeyEvent event);
    void KeyInput(oxygine::KeyEvent event);
protected:
    /**
     * @brief checkInput checks if the input is correct and updates it if needed and returns the new value
     * @return current value
     */
    qreal checkInput();
    /**
     * @brief setValue
     * @param value
     */
    void setValue(qreal value);
private:
    bool m_focused{false};
    oxygine::spBox9Sprite m_pSpinBox;
    oxygine::spBox9Sprite m_Textbox;
    oxygine::spTextField m_Textfield;
    oxygine::spButton m_pArrowDown;
    oxygine::spButton m_pArrowUp;
    QString m_Text;
    QTime toggle;
    qint32 curmsgpos{0};
    qreal m_InfinityValue{-1.0};
    qreal m_MinValue{0.0};
    qreal m_MaxValue{100.0};
    qreal m_spinDirection{0.0};
    qreal m_SpinSpeed{1.0};
    Mode m_Mode{Mode::Int};
};

#endif // SPINBOX_H
