#ifndef H_SCROLLBAR_H
#define H_SCROLLBAR_H

#include "objects/base/tooltip.h"
#include <QElapsedTimer>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

class H_Scrollbar;
typedef oxygine::intrusive_ptr<H_Scrollbar> spH_Scrollbar;

class H_Scrollbar : public Tooltip
{
    Q_OBJECT
public:
    /**
     * @brief H_Scrollbar
     * @param heigth heigth of the complete scrollbar. Slider + Both Arrows -> Note: No Checks for a valid value is done here.
     * @param contentHeigth heigth of the area you can scroll with this scrollbar. Needed for the arrows to calculate the add value. :)
     */
    explicit H_Scrollbar(qint32 heigth, qint32 contentHeigth);
    /**
     * @brief setContentHeigth
     * @param heigth
     */
    void setContentHeigth(qint32 heigth);

    virtual void update(const oxygine::UpdateState&) override;

    virtual void setHeight(float h) override;
    bool getSliding() const;
    void setSliding(bool sliding);

signals:
    /**
     * @brief sigScrollValueChanged emitted when the scroll value changes between 0.0f and 1.0f
     * @param value
     */
    void sigScrollValueChanged(float value);
    /**
     * @brief sigChangeScrollValue
     * @param value
     */
    void sigChangeScrollValue(float value);
public slots:
    void changeScrollValue(float value);
    void setScrollvalue(float Scrollvalue);
private:
    void scroll(oxygine::Event* pEvent);
private:
    float m_Scrollvalue{0.0f};
    float m_Scrollspeed{1.0f};
    qint32 m_speedCounter{0};
    qint32 m_Heigth;
    qint32 m_ContentHeigth;
    qint32 m_scroll{0};
    bool m_sliding{false};
    oxygine::spBox9Sprite  m_slider;
    QElapsedTimer m_ScrollTimer;

    oxygine::spBox9Sprite m_pBox;
    oxygine::spButton m_pArrowDown;
};

#endif // H_SCROLLBAR_H
