#pragma once

#include "gameinput/humanplayerinput.h"

class MoveplannerInput;
using spMoveplannerInput = std::shared_ptr<MoveplannerInput>;

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
    Q_INVOKABLE virtual QStringList getEmptyActionList() override;
public slots:
    virtual void leftClick(qint32 x, qint32 y) override;

private:
    bool m_focus{true};
};

