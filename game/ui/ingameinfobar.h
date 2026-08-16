#ifndef INGAMEINFOBAR_H
#define INGAMEINFOBAR_H

#include <QObject>

#include "3rd_party/oxygine-framework/oxygine/actor/SlidingActor.h"
#include "3rd_party/oxygine-framework/oxygine/actor/Box9Sprite.h"

#include "objects/minimap.h"

class GameMap;
class GameMenue;
class Player;
class IngameInfoBar;
using spIngameInfoBar = std::shared_ptr<IngameInfoBar>;

class IngameInfoBar final : public CreatedGui
{
    Q_OBJECT
public:
    static constexpr qint32 spriteWidth = 127;
    static constexpr qint32 spriteHeigth = 192;

    explicit IngameInfoBar(GameMenue* pMenu, GameMap* pMap);
    virtual ~IngameInfoBar() = default;
    Minimap* getMinimap()
    {
        return m_pMinimap;
    }
    void updateTerrainInfo(qint32 x, qint32 y, bool update);
    oxygine::spBox9Sprite getDetailedViewBox() const;
    Q_INVOKABLE GameMap* getMap() const;
    Q_INVOKABLE GameMenue* getGameMenu();
    Q_INVOKABLE void updateMinimap();
    Q_INVOKABLE void updatePlayerInfo();

public slots:
    void updateCursorInfo(qint32 x, qint32 y);
    void syncMinimapPosition();

private slots:
    void updateTerrainInfoInternal();

private:
    void updateDetailedView(qint32 x, qint32 y);

private:
    Minimap* m_pMinimap{nullptr};
    oxygine::SlidingActor* m_pMinimapSlider{nullptr};
    oxygine::spBox9Sprite m_pDetailedViewBox;
    qint32 m_LastX{-1};
    qint32 m_LastY{-1};
    qint32 m_newX{-1};
    qint32 m_newY{-1};
    GameMap* m_pMap{nullptr};
    GameMenue* m_pMenu{nullptr};
    QTimer m_debounceTimer;
};

Q_DECLARE_INTERFACE(IngameInfoBar, "IngameInfoBar");

#endif // INGAMEINFOBAR_H
