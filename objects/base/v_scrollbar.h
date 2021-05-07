#ifndef V_SCROLLBAR_H
#define V_SCROLLBAR_H

#include <QObject>
#include <QElapsedTimer>

#include "3rd_party/oxygine-framework/oxygine-framework.h"

#include "objects/base/tooltip.h"

class V_Scrollbar;
typedef oxygine::intrusive_ptr<V_Scrollbar> spV_Scrollbar;

class V_Scrollbar : public Tooltip
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

    virtual void setWidth(float w) override;
    float getScrollspeed() const;
    void setScrollspeed(float Scrollspeed);

    float getScrollvalue() const;
    void setScrollvalue(float Scrollvalue);
    bool getSliding() const;
    void setSliding(bool sliding);
    /**
     * @brief setEnabled
     * @param value
     */
    virtual void setEnabled(bool value) override;
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
    /**
     * @brief sigStartEditValue
     */
    void sigStartEditValue();
    /**
     * @brief sigEndEditValue
     * @param value
     */
    void sigEndEditValue(float value);
public slots:
    void changeScrollValue(float value);
    virtual void focusedLost() override;
private:
    void scroll(oxygine::Event* pEvent);
private:
    float m_Scrollvalue{0.0f};
    qint32 m_speedCounter{0};
    qint32 m_Width;
    qint32 m_ContentWidth;
    qint32 m_scroll{0};
    float m_currentScrollspeed{0.0f};
    float m_Scrollspeed{1.0f};
    bool m_sliding{false};
    oxygine::spBox9Sprite  m_slider;
    QElapsedTimer m_ScrollTimer;
    oxygine::spBox9Sprite m_pBox;
    oxygine::spButton m_pArrowRigth;
    oxygine::spButton m_pArrowLeft;
};

#endif // V_SCROLLBAR_H
