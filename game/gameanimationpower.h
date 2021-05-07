#ifndef GAMEANIMATIONPOWER_H
#define GAMEANIMATIONPOWER_H
#include <QTimer>

#include "game/gameanimation.h"
#include "game/GameEnums.h"
#include "game/co.h"

class GameAnimationPower;
using spGameAnimationPower = oxygine::intrusive_ptr<GameAnimationPower>;

class GameAnimationPower : public GameAnimation
{
    Q_OBJECT
public:
    static spGameAnimationPower createGameAnimationPower(quint32 frameTime, QColor color, GameEnums::PowerMode powerMode, CO* pCO);

    virtual ~GameAnimationPower();

    virtual void restart() override;
    virtual void stop() override;

    virtual void update(const oxygine::UpdateState& us) override;
signals:
    void sigRightClick();
public slots:
    void rightClick();
private:
    friend class oxygine::intrusive_ptr<GameAnimationPower>;
    GameAnimationPower(quint32 frameTime, QColor color, GameEnums::PowerMode powerMode, CO* pCO);

private:
    QTimer m_endTimer;
    CO* m_pCO{nullptr};
    static GameAnimationPower* m_pGameAnimationPower;
};

#endif // GAMEANIMATIONPOWER_H
