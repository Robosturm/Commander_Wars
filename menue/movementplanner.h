#pragma once

#include "menue/gamemenue.h"

#include "game/player.h"
#include "game/gameanimation/animationskipper.h"

#include "objects/base/topbar.h"

class MovementPlanner;
using spMovementPlanner = oxygine::intrusive_ptr<MovementPlanner>;

class MovementPlanner final : public GameMenue
{
    Q_OBJECT
public:
    MovementPlanner(GameMenue* pOwner, Player* pViewPlayer);
    ~MovementPlanner() = default;

    Player *getViewPlayer() const;

signals:
    void sigExit();
    void sigHide();

public slots:
    void reloadMap();
    void onShowPlanner();
    void onExitPlanner();
    virtual void victory(qint32 team) override;
    void clickedTopbar(QString itemID);

    void hide();
    void exit();
private:
    GameMenue* m_pOwner{nullptr};
    Player* m_pViewPlayer{nullptr};
    AnimationSkipper m_animationSkipper;
    spTopbar m_Topbar;
};

