#ifndef SPINBOX_H
#define SPINBOX_H

#include <QObject>
#include <QElapsedTimer>

#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"

#include "objects/base/textinput.h"

class SpinBox;
using spSpinBox = oxygine::intrusive_ptr<SpinBox>;

class SpinBox final : public TextInput
{
    Q_OBJECT
public:
    enum Mode
    {
        Int,
        Float,
        Real,
    };

    explicit SpinBox(qint32 width, qreal min = 0, qreal max = 100, Mode mode = Mode::Int);
    ~SpinBox() = default;

    virtual void update(const oxygine::UpdateState& us) override;
    virtual void setEnabled(bool value) override;
    /**
     * @brief getCurrentValue
     * @return the current value of the spin box
     */
    Q_INVOKABLE qreal getCurrentValue();
    Q_INVOKABLE QString getUnit() const;
    Q_INVOKABLE void setUnit(const QString &unit);
    /**
     * @brief setCurrentValue changes the value of this spin box
     * @param text
     */
    Q_INVOKABLE void setCurrentValue(qreal value);
    Q_INVOKABLE qreal getSpinSpeed() const;
    Q_INVOKABLE void setSpinSpeed(qreal SpinSpeed);
    Q_INVOKABLE qreal getInfinityValue() const;
    Q_INVOKABLE void setInfinityValue(qreal InfinityValue);
signals:
    void sigValueChanged(qreal value);
public slots:
    virtual void focusedLost() override;
protected:
    /**
     * @brief checkInput checks if the input is correct and updates it if needed and returns the value is valid
     * @return current value
     */
    qreal checkInput();
    /**
     * @brief setValue
     * @param value
     */
    void setValue(qreal value);
    virtual bool onEditFinished() override;
private:
    oxygine::spBox9Sprite m_pSpinBox;
    oxygine::spBox9Sprite m_Textbox;
    oxygine::spTextField m_Textfield;
    oxygine::spButton m_pArrowDown;
    oxygine::spButton m_pArrowUp;
    QString m_unit;
    QElapsedTimer m_spinTimer;
    qreal m_InfinityValue{-1.0};
    qreal m_MinValue{0.0};
    qreal m_MaxValue{100.0};
    qreal m_spinDirection{0.0};
    qreal m_SpinSpeed{1.0};
    Mode m_Mode{Mode::Int};
};

Q_DECLARE_INTERFACE(SpinBox, "SpinBox");

#endif // SPINBOX_H
