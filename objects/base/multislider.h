#ifndef MULTISLIDER_H
#define MULTISLIDER_H

#include <QObject>
#include <QString>
#include <QVector>

#include "3rd_party/oxygine-framework/oxygine/actor/TextField.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Button.h"

#include "objects/base/slider.h"
#include "objects/base/tooltip.h"

class Multislider;
using spMultislider = oxygine::intrusive_ptr<Multislider>;

class Multislider : public Tooltip
{
    Q_OBJECT
public:
    explicit Multislider(QStringList texts, qint32 width, QVector<qint32> values = {});
    virtual ~Multislider() = default;
    qint32 getSliderValue(qint32 slider);
    virtual void setEnabled(bool value) override;
signals:
    void signalSliderChanged();
    void signalSliderValueChanged(qint32 slider);
public slots:
    void sliderValueChanged(qint32 slider);
private:
    QVector<oxygine::spTextField> m_Textfields;
    QVector<spSlider> m_Slider;
    QVector<oxygine::spButton> m_lockButtons;
    QVector<bool> m_locked;
    qint32 m_currentSliderChange{0};
};

#endif // MULTISLIDER_H
