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
    void updatePlayerInfo();
    void updateCursorInfo(qint32 x, qint32 y);
private:
    spMinimap m_pMinimap;
    oxygine::spSlidingActor m_pMinimapSlider;
    oxygine::spBox9Sprite m_pGameInfoBox;
    oxygine::spBox9Sprite m_pCursorInfoBox;
};

#endif // INGAMEINFOBAR_H
