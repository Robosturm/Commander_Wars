#ifndef INGAMEINFOBAR_H
#define INGAMEINFOBAR_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine-framework.h"
#include "objects/minimap.h"

class IngameInfoBar;
typedef oxygine::intrusive_ptr<IngameInfoBar> spIngameInfoBar;

class IngameInfoBar : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    static constexpr qint32 spriteWidth = 127;
    static constexpr qint32 spriteHeigth = 192;

    explicit IngameInfoBar();

    Minimap* getMinimap()
    {
        return m_pMinimap.get();
    }

    void updateTerrainInfo(qint32 x, qint32 y, bool update);

public slots:
    void updateMinimap();
    void updatePlayerInfo();
    void updateCursorInfo(qint32 x, qint32 y);
    void updateDetailedView(qint32 x, qint32 y);
    void createUnitInfo(qint32 x, qint32 y);
    void createTerrainInfo(qint32 x, qint32 y);
    void addColorbar(float divider, qint32 posX, qint32 posY, QColor color);
    void syncMinimapPosition();
private:
    spMinimap m_pMinimap;
    oxygine::spSlidingActor m_pMinimapSlider;
    oxygine::spBox9Sprite m_pGameInfoBox;
    oxygine::spBox9Sprite m_pCursorInfoBox;
    qint32 m_LastX{-1};
    qint32 m_LastY{-1};
};

#endif // INGAMEINFOBAR_H
