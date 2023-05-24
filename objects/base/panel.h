#ifndef PANEL_H
#define PANEL_H

#include <QObject>
#include <QSize>

#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"
#include "3rd_party/oxygine-framework/oxygine/actor/ClipRectActor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/SlidingActor.h"

#include "objects/base/h_scrollbar.h"
#include "objects/base/v_scrollbar.h"

class Panel;
using spPanel = oxygine::intrusive_ptr<Panel>;

class Panel final : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    static constexpr qint32 sliderSize = 33;
    explicit Panel(bool useBox, QSize size, QSize contentSize, QString resAnim = "panel");
    ~Panel();
    void addItem(oxygine::spActor pActor);
    void removeItem(oxygine::spActor pActor);
    void setContentHeigth(qint32 heigth);
    void setContentWidth(qint32 width);

    qint32 getContentHeigth() const;
    qint32 getContentWidth() const;

    qint32 getContentX() const;
    qint32 getContentY() const;

    void clearContent();

    inline H_Scrollbar* getH_Scrollbar()
    {
        return m_HScrollbar.get();
    }
    inline V_Scrollbar* getV_Scrollbar()
    {
        return m_VScrollbar.get();
    }
    bool getSubComponent() const;
    void setSubComponent(bool subComponent);
    bool getStopMouseWheel() const;
    void setStopMouseWheel(bool newStopMouseWheel);

public slots:
    void scrolledY(float value);
    void scrolledX(float value);
protected:
    virtual void doUpdate(const oxygine::UpdateState& us) override;
private slots:
    void hideItems();
private:
    void hideChildItems(oxygine::spActor parent);
private:
    oxygine::spBox9Sprite m_Panelbox;
    oxygine::spClipRectActor m_ClipRect;
    oxygine::spSlidingActor m_SlidingActor;
    oxygine::spActor m_ContentRect;
    spH_Scrollbar m_HScrollbar;
    spV_Scrollbar m_VScrollbar;
    QTimer m_hideTimer;
    bool m_stopMouseWheel{true};
};

#endif // PANEL_H
