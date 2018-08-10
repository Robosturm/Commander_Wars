#ifndef H_SCROLLBAR_H
#define H_SCROLLBAR_H

#include <QObject>
#include <QTime>
#include "oxygine-framework.h"

class H_Scrollbar : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    /**
     * @brief H_Scrollbar
     * @param heigth heigth of the complete scrollbar. Slider + Both Arrows -> Note: No Checks for a valid value is done here.
     * @param contentHeigth heigth of the area you can scroll with this scrollbar. Needed for the arrows to calculate the add value. :)
     */
    explicit H_Scrollbar(qint32 heigth, qint32 contentHeigth);

    virtual void update(const oxygine::UpdateState& us) override;
signals:
    /**
     * @brief sigScrollValueChanged emitted when the scroll value changes between 0.0f and 1.0f
     * @param value
     */
    void sigScrollValueChanged(float value);
public slots:

private:
    float m_Scrollvalue{0.0f};
    qint32 m_Heigth;
    qint32 m_ContentHeigth;
    qint32 m_scroll{0};
    bool m_sliding{false};
    oxygine::spBox9Sprite  m_slider;
    QTime m_ScrollTimer;
};

#endif // H_SCROLLBAR_H
