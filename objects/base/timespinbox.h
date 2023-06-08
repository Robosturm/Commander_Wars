#ifndef TIMESPINBOX_H
#define TIMESPINBOX_H

#include <QObject>
#include <QElapsedTimer>

#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"

#include "objects/base/textinput.h"

class TimeSpinBox;
using spTimeSpinBox = oxygine::intrusive_ptr<TimeSpinBox>;

class TimeSpinBox final : public TextInput
{
    Q_OBJECT
public:
    explicit TimeSpinBox(qint32 width);
   virtual ~TimeSpinBox() = default;
    virtual void update(const oxygine::UpdateState& us) override;

    virtual void setEnabled(bool value) override;
    /**
     * @brief getCurrentValue
     * @return the current value of the spin box
     */
    Q_INVOKABLE qint32 getCurrentValue();
    /**
     * @brief setCurrentValue changes the value of this spin box
     * @param text
     */
    Q_INVOKABLE void setCurrentValue(qint32 value);
    Q_INVOKABLE qint32 getSpinSpeed() const;
    Q_INVOKABLE void setSpinSpeed(qint32 SpinSpeed);
signals:
    void sigValueChanged(qint32 value);
public slots:
    virtual void focusedLost() override;
protected:
    /**
     * @brief checkInput checks if the input is correct and updates it if needed and returns the value is valid
     * @return current value
     */
    qint32 checkInput();
    /**
     * @brief setValue
     * @param value
     */
    void setValue(qint32 value);
    virtual bool onEditFinished() override;
private:
    oxygine::spBox9Sprite m_pSpinBox;
    oxygine::spBox9Sprite m_Textbox;
    oxygine::spTextField m_Textfield;
    oxygine::spButton m_pArrowDown;
    oxygine::spButton m_pArrowUp;
    QElapsedTimer m_spinTimer;
    qint32 m_spinDirection{0};
    qint32 m_SpinSpeed{1000 * 60};
};

Q_DECLARE_INTERFACE(TimeSpinBox, "TimeSpinBox");

#endif // SPINBOX_H
