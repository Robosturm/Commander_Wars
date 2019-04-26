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

    void updateTerrainInfo(qint32 x, qint32 y, bool update);
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
    qint32 m_LastX{-1};
    qint32 m_LastY{-1};
};

#endif // INGAMEINFOBAR_H
