#pragma once

#include "gameinput/humanplayerinput.h"

class MoveplannerInput;
using spMoveplannerInput = oxygine::intrusive_ptr<MoveplannerInput>;

class MoveplannerInput final : public HumanPlayerInput
{
public:
    explicit MoveplannerInput(GameMap* pMap);
    ~MoveplannerInput() = default;
    /**
     * @brief getPerformingPlayer
     * @return
     */
    virtual Player* getPerformingPlayer(Player* pPlayer) const override;
public slots:
    virtual QStringList getEmptyActionList() override;
};

