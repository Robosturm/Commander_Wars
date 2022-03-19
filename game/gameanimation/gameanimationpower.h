#ifndef GAMEANIMATIONPOWER_H
#define GAMEANIMATIONPOWER_H
#include <QTimer>

#include "game/gameanimation/gameanimation.h"
#include "game/GameEnums.h"
#include "game/co.h"

class GameAnimationPower;
using spGameAnimationPower = oxygine::intrusive_ptr<GameAnimationPower>;

class GameAnimationPower : public GameAnimation
{
    Q_OBJECT
public:
    static spGameAnimationPower createGameAnimationPower(quint32 frameTime, QColor color, GameEnums::PowerMode powerMode, CO* pCO, GameMap * pMap);

    virtual ~GameAnimationPower();

    virtual void restart() override;
    virtual void stop() override;
signals:
    void sigRightClick();
public slots:
    void rightClick();
protected slots:
    virtual void start() override;
private:
    friend class oxygine::intrusive_ptr<GameAnimationPower>;
    GameAnimationPower(quint32 frameTime, QColor color, GameEnums::PowerMode powerMode, CO* pCO, GameMap * pMap);
    void createPowerDescription(CO* pCo, GameEnums::PowerMode powerMode, bool onTop);
private:
    QTimer m_endTimer;
    CO* m_pCO{nullptr};
    static GameAnimationPower* m_pGameAnimationPower;
};

Q_DECLARE_INTERFACE(GameAnimationPower, "GameAnimationPower");

#endif // GAMEANIMATIONPOWER_H
