#ifndef PANEL_H
#define PANEL_H

#include <QObject>
#include <QSize>
#include "oxygine-framework.h"
#include "objects/h_scrollbar.h"
#include "objects/v_scrollbar.h"

class Panel;
typedef oxygine::intrusive_ptr<Panel> spPanel;

class Panel : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit Panel(bool useBox, QSize size, QSize contentSize);

    void addItem(oxygine::spActor pActor);
    void removeItem(oxygine::spActor pActor);
    void setContentHeigth(qint32 heigth);
    void setContentWidth(qint32 width);

    qint32 getContentHeigth();
    qint32 getContentWidth();

    void clearContent();

    inline H_Scrollbar* getH_Scrollbar()
    {
        return m_HScrollbar.get();
    }
    inline V_Scrollbar* getV_Scrollbar()
    {
        return m_VScrollbar.get();
    }
signals:

public slots:
    void scrolledY(float value);
    void scrolledX(float value);
private:
    void hideItems();
    void hideItems(oxygine::spActor parent);
private:
    oxygine::spBox9Sprite m_Panelbox;
    oxygine::spClipRectActor m_ClipRect;
    oxygine::spActor m_ContentRect;
    spH_Scrollbar m_HScrollbar;
    spV_Scrollbar m_VScrollbar;
};

#endif // PANEL_H
