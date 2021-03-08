#ifndef SLIDER_H
#define SLIDER_H

#include <QObject>

#include "objects/base/v_scrollbar.h"
#include "objects/base/spinbox.h"

class Slider;
typedef oxygine::intrusive_ptr<Slider> spSlider;

class Slider : public V_Scrollbar
{
    Q_OBJECT
public:
    explicit Slider(qint32 width, qint32 minValue, qint32 maxValue, QString unit = "%");

    qint32 getCurrentValue() const;
    void setCurrentValue(const qint32 &CurrentValue);

    virtual void setEnabled(bool value) override;
signals:
    void sliderValueChanged(qint32 value);
public slots:
    void slotSliderValueChanged(float value);
private slots:
    void slotSpinBoxValueChanged(qreal currentValuevalue);
private:
    spSpinBox m_spinBox;
    qint32 m_minValue{0};
    qint32 m_maxValue{0};
    QString m_Unit;
};

#endif // SLIDER_H
