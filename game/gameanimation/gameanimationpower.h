#ifndef GAMEANIMATIONPOWER_H
#define GAMEANIMATIONPOWER_H
#include <QTimer>

#include "game/gameanimation/gameanimation.h"
#include "game/GameEnums.h"
#include "game/co.h"

class GameAnimationPower;
using spGameAnimationPower = std::shared_ptr<GameAnimationPower>;

class GameAnimationPower final : public GameAnimation
{
    Q_OBJECT
public:
    static spGameAnimationPower createGameAnimationPower(quint32 frameTime, QColor color, GameEnums::PowerMode powerMode, CO* pCO, GameMap * pMap);
   virtual ~GameAnimationPower();
    virtual void restart() override;
    virtual void stop() override;
    Q_INVOKABLE void createPowerDescription(CO* pCo, GameEnums::PowerMode powerMode, bool onTop);
    Q_INVOKABLE void createRotatingBackground(const QString resAnim, const QColor color, qint32 speedX = 3);
    Q_INVOKABLE void setDuration(qint32 timeMs);
    Q_INVOKABLE void createMovingText(const QString font, const QString text, qint32 delay, QPoint startPos, QPoint endPos, qint32 duration, QEasingCurve::Type easeType = QEasingCurve::Linear);
    Q_INVOKABLE void addMovingCoSprite(const QString sprite, float scale, QPoint startPos, QPoint endPos, qint32 duration, qint32 delay = 0, QEasingCurve::Type easeType = QEasingCurve::Linear);
    Q_INVOKABLE QPoint getCoSpriteSize(const QString sprite) const;
signals:
    void sigRightClick();
public slots:
    void rightClick();
protected slots:
    virtual void start() override;
private:
    friend class MemoryManagement;
    GameAnimationPower(quint32 frameTime, CO* pCO, GameMap * pMap);

private:
    QTimer m_endTimer;
    CO* m_pCO{nullptr};
    static GameAnimationPower* m_pGameAnimationPower;
};

Q_DECLARE_INTERFACE(GameAnimationPower, "GameAnimationPower");

#endif // GAMEANIMATIONPOWER_H
