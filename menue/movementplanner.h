#pragma once

#include "menue/gamemenue.h"

#include "game/player.h"
#include "game/gameanimation/animationskipper.h"

#include "game/ui/movementplanneraddin.h"

#include "gameinput/moveplannerinput.h"

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
    void startAddIn(QString addIn);
    void hide();
    void exit();
    /**
     * @brief keyInput
     * @param event
     */
    virtual void keyInput(oxygine::KeyEvent event) override;
    void leftClick(qint32 x, qint32 y);
private:
    void addAddIn(QStringList & loadedGroups, QString newAddInId);
    bool readyToExecute();
    void execute();

private:
    GameMenue* m_pOwner{nullptr};
    Player* m_pViewPlayer{nullptr};
    AnimationSkipper m_animationSkipper;
    spTopbar m_Topbar;
    spMoveplannerInput m_input;
    QVector<spMovementPlannerAddIn> m_addIns;
    spMovementPlannerAddIn m_activeAddIn;
};

