#ifndef SPINBOX_H
#define SPINBOX_H

#include <QObject>
#include <QTime>
#include "oxygine-framework.h"

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
    };

    explicit SpinBox(qint32 width, qint32 min = 0, qint32 max = 100, Mode mode = Mode::Int);
    /**
     * @brief getCurrentValue
     * @return the current value of the spin box
     */
    float getCurrentValue();
    virtual void update(const oxygine::UpdateState& us) override;
    /**
     * @brief setCurrentValue changes the value of this spin box
     * @param text
     */
    void setCurrentValue(float value);
    float getSpinSpeed() const;
    void setSpinSpeed(float SpinSpeed);

    float getInfinityValue() const;
    void setInfinityValue(float InfinityValue);

    virtual void setEnabled(bool value) override;
signals:
    void sigValueChanged(float value);
public slots:
    void TextInput(SDL_Event event);
    void KeyInput(SDL_Event event);
protected:
    /**
     * @brief checkInput checks if the input is correct and updates it if needed and returns the new value
     * @return current value
     */
    float checkInput();
    /**
     * @brief setValue
     * @param value
     */
    void setValue(float value);
private:
    bool m_focused{false};
    oxygine::spBox9Sprite m_Textbox;
    oxygine::spTextField m_Textfield;
    oxygine::spButton m_pArrowDown;
    oxygine::spButton m_pArrowUp;
    QString m_Text;
    QTime toggle;
    qint32 curmsgpos{0};
    float m_InfinityValue{-1.0f};
    float m_MinValue{0.0f};
    float m_MaxValue{100.0f};
    float m_spinDirection{0.0f};
    float m_SpinSpeed{1.0f};
    Mode m_Mode{Mode::Int};
};

#endif // SPINBOX_H
