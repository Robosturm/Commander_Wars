#ifndef PANEL_H
#define PANEL_H

#include <QObject>
#include <QSize>
#include "oxygine-framework.h"
#include "objects/h_scrollbar.h"

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
signals:

public slots:
    void scrolled(float value);
private:
    oxygine::spBox9Sprite m_Panelbox;
    oxygine::spClipRectActor m_ClipRect;
    oxygine::spActor m_ContentRect;
    spH_Scrollbar m_Scrollbar;
};

#endif // PANEL_H
