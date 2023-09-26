#ifndef GAMEANIMATIONWALK_H
#define GAMEANIMATIONWALK_H

#include <QObject>
#include <QVector>
#include <QPoint>

#include "game/gameanimation/gameanimation.h"
#include "game/gamemap.h"
#include "game/GameEnums.h"

class Unit;
class GameAnimationWalk;
using spGameAnimationWalk = std::shared_ptr<GameAnimationWalk>;

class GameAnimationWalk final : public GameAnimation
{
    Q_OBJECT
public:
    explicit GameAnimationWalk(spUnit pUnit, const QVector<QPoint> & movePath, GameMap* pMap);
    virtual ~GameAnimationWalk() = default;
    /**
     * @brief loadSprite
     * @param spriteID the id of the sprite we want to create
     * @param addPlayerColor if the sprite should add some color
     * @param scaling scaling factor for the animation
     */
    Q_INVOKABLE void loadSprite(const QString spriteID, bool addPlayerColor, float scaling);
    /**
     * @brief loadSpriteV2
     * @param spriteID
     * @param mode
     * @param scaling
     */
    Q_INVOKABLE void loadSpriteV2(const QString spriteID, GameEnums::Recoloring mode, float scaling);
    /**
     * @brief onFinished called when the animation has finished
     */
    Q_INVOKABLE virtual bool onFinished(bool skipping) override;
protected slots:
    virtual void start() override;
protected:
    /**
     * @brief getMovementDirection returns the movement direction of this unit
     * @param x current position
     * @param y current position
     * @param x2 next position
     * @param y2 next position
     * @return
     */
    GameEnums::Directions getMovementDirection(qint32 x, qint32 y, qint32 x2, qint32 y2);

private:
    spUnit m_pUnit;
    QVector<QPoint> m_movePath;
};

Q_DECLARE_INTERFACE(GameAnimationWalk, "GameAnimationWalk");

#endif // GAMEANIMATIONWALK_H
