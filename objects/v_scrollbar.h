#ifndef V_SCROLLBAR_H
#define V_SCROLLBAR_H

#include <QObject>
#include <QTime>
#include "oxygine-framework.h"

class V_Scrollbar;
typedef oxygine::intrusive_ptr<V_Scrollbar> spV_Scrollbar;

class V_Scrollbar : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    /**
     * @brief V_Scrollbar
     * @param width width of the complete scrollbar. Slider + Both Arrows -> Note: No Checks for a valid value is done here.
     * @param contentWidth width of the area you can scroll with this scrollbar. Needed for the arrows to calculate the add value. :)
     */
    explicit V_Scrollbar(qint32 width, qint32 contentWidth);
    /**
     * @brief setContentWidth
     * @param heigth
     */
    void setContentWidth(qint32 width);

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
    qint32 m_Width;
    qint32 m_ContentWidth;
    qint32 m_scroll{0};
    bool m_sliding{false};
    oxygine::spBox9Sprite  m_slider;
    QTime m_ScrollTimer;
};

#endif // V_SCROLLBAR_H
