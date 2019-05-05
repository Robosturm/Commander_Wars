#ifndef SLIDER_H
#define SLIDER_H

#include <QObject>

#include "objects/v_scrollbar.h"

class Slider;
typedef oxygine::intrusive_ptr<Slider> spSlider;

class Slider : public V_Scrollbar
{
    Q_OBJECT
public:
    explicit Slider(qint32 width, qint32 minValue, qint32 maxValue, QString unit = "%");

    qint32 getCurrentValue() const;
    void setCurrentValue(const qint32 &CurrentValue);

signals:
    void sliderValueChanged(qint32 value);
public slots:
    void slotSliderValueChanged(float value);
private:
    oxygine::spTextField m_Textfield;
    qint32 m_minValue{0};
    qint32 m_maxValue{0};
    qint32 m_CurrentValue{0};
    QString m_Unit;
};

#endif // SLIDER_H
