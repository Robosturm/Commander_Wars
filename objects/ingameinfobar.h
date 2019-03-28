#ifndef INGAMEINFOBAR_H
#define INGAMEINFOBAR_H

#include <QObject>

#include "oxygine-framework.h"
#include "objects/minimap.h"

class IngameInfoBar;
typedef oxygine::intrusive_ptr<IngameInfoBar> spIngameInfoBar;

class IngameInfoBar : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit IngameInfoBar();

    Minimap* getMinimap()
    {
        return m_pMinimap.get();
    }
signals:

public slots:
    void updateMinimap();
private:
    spMinimap m_pMinimap;
    oxygine::spSlidingActor m_pMinimapSlider;
};

#endif // INGAMEINFOBAR_H
