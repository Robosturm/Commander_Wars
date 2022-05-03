#ifndef INGAMEINFOBAR_H
#define INGAMEINFOBAR_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine-framework.h"
#include "objects/minimap.h"

class GameMap;
class IngameInfoBar;
using spIngameInfoBar = oxygine::intrusive_ptr<IngameInfoBar>;

class IngameInfoBar : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    static constexpr qint32 spriteWidth = 127;
    static constexpr qint32 spriteHeigth = 192;

    explicit IngameInfoBar(GameMap* pMap);
    virtual ~IngameInfoBar() = default;
    Minimap* getMinimap()
    {
        return m_pMinimap.get();
    }

    void updateTerrainInfo(qint32 x, qint32 y, bool update);

    oxygine::spBox9Sprite getDetailedViewBox() const;
    void setMap(GameMap *newMap);

public slots:
    GameMap *getMap() const;
    void updateMinimap();
    void updatePlayerInfo();
    void updateCursorInfo(qint32 x, qint32 y);
    void updateDetailedView(qint32 x, qint32 y);
    bool createUnitInfo(qint32 x, qint32 y);
    void createTerrainInfo(qint32 x, qint32 y);
    void createMovementInfo(qint32 x, qint32 y);
    void addColorbar(float divider, qint32 posX, qint32 posY, QColor color);
    void syncMinimapPosition();
private:
    spMinimap m_pMinimap;
    oxygine::spSlidingActor m_pMinimapSlider;
    oxygine::spBox9Sprite m_pGameInfoBox;
    oxygine::spBox9Sprite m_pCursorInfoBox;
    oxygine::spBox9Sprite m_pDetailedViewBox;
    qint32 m_LastX{-1};
    qint32 m_LastY{-1};
    GameMap* m_pMap{nullptr};
};

#endif // INGAMEINFOBAR_H
