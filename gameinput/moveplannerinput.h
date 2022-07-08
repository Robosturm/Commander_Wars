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
    bool getFocus() const;
    void setFocus(bool newFocus);

public slots:
    virtual QStringList getEmptyActionList() override;
    virtual void leftClick(qint32 x, qint32 y) override;

private:
    bool m_focus{true};
};

