#ifndef GAMEANIMATIONPOWER_H
#define GAMEANIMATIONPOWER_H
#include <QTimer>

#include "game/gameanimation/gameanimation.h"
#include "game/GameEnums.h"
#include "game/co.h"

class GameAnimationPower;
using spGameAnimationPower = oxygine::intrusive_ptr<GameAnimationPower>;

class GameAnimationPower final : public GameAnimation
{
    Q_OBJECT
public:
    static spGameAnimationPower createGameAnimationPower(quint32 frameTime, QColor color, GameEnums::PowerMode powerMode, CO* pCO, GameMap * pMap);
    ~GameAnimationPower();

    virtual void restart() override;
    virtual void stop() override;
signals:
    void sigRightClick();
public slots:
    void rightClick();
    void createPowerDescription(CO* pCo, GameEnums::PowerMode powerMode, bool onTop);
    void createRotatingBackground(const QString & resAnim, const QColor & color);
    void setDuration(qint32 timeMs);
    void createMovingText(const QString & font, const QString & text, qint32 delay, QPoint startPos, QPoint endPos, qint32 duration, QEasingCurve::Type easeType = QEasingCurve::Linear);
    void addMovingCoSprite(const QString & sprite, float scale, QPoint startPos, QPoint endPos, qint32 duration, qint32 delay = 0, QEasingCurve::Type easeType = QEasingCurve::Linear);
    QPoint getCoSpriteSize(const QString & sprite) const;
protected slots:
    virtual void start() override;
private:
    friend class oxygine::intrusive_ptr<GameAnimationPower>;
    GameAnimationPower(quint32 frameTime, CO* pCO, GameMap * pMap);

private:
    QTimer m_endTimer;
    CO* m_pCO{nullptr};
    static GameAnimationPower* m_pGameAnimationPower;
};

Q_DECLARE_INTERFACE(GameAnimationPower, "GameAnimationPower");

#endif // GAMEANIMATIONPOWER_H
