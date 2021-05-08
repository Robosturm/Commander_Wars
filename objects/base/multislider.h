#ifndef MULTISLIDER_H
#define MULTISLIDER_H

#include <QObject>

#include <QString>

#include <QVector>


#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "objects/base/slider.h"
#include "objects/base/tooltip.h"

class Multislider;
typedef oxygine::intrusive_ptr<Multislider> spMultislider;

class Multislider : public Tooltip
{
    Q_OBJECT
public:
    explicit Multislider(QVector<QString> texts, qint32 width, QVector<qint32> values = {});

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
