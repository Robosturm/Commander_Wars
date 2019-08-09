#ifndef BATTLEANIMATIONSPRITE_H
#define BATTLEANIMATIONSPRITE_H

#include <QObject>

#include "oxygine-framework.h"

#include "game/unit.h"

class BattleAnimationSprite;
typedef oxygine::intrusive_ptr<BattleAnimationSprite> spBattleAnimationSprite;

class BattleAnimationSprite : public QObject, public oxygine::Sprite
{
    Q_OBJECT
public:
    static const QString standingAnimation;
    static const QString impactAnimation;
    static const QString fireAnimation;
    static const QString moveInAnimation;
    explicit BattleAnimationSprite(Unit* pUnit, Terrain* pTerrain, QString animationType, qint32 hp = -1);
    /**
     * @brief loadAnimation
     * @param animationType
     */
    void loadAnimation(QString animationType);

    /**
     * @brief getClipActor
     * @return
     */
    oxygine::spClipRectActor getClipActor()
    {
        return m_Actor;
    }
signals:

public slots:
    /**
     * @brief loadAnimation
     * @param animationType
     * @param pUnit
     * @param clearSprite if true clears the battle animation sprite buffer so you can add stuff from scratch default
     */
    void loadAnimation(QString animationType, Unit* pUnit, bool clearSprite = true);
    /**
     * @brief getMaxUnitCount
     * @return
     */
    qint32 getMaxUnitCount();
    /**
     * @brief getHpRounded
     * @return
     */
    qint32 getHpRounded() const;
    /**
     * @brief setHpRounded
     * @param value
     */
    void setHpRounded(const qint32 &value);
    /**
     * @brief getUnitPositionOffset
     * @param unitIdx
     * @return
     */
    QPoint getUnitPositionOffset(qint32 unitIdx);
    /**
     * @brief getUnitPosition
     * @param unitCount
     * @return
     */
    QPoint getUnitPosition(qint32 unitCount, qint32 maxUnitCount);
    /**
     * @brief loadSprite
     * @param spriteID
     * @param addPlayerColor
     */
    void loadSprite(QString spriteID, bool addPlayerColor, qint32 maxUnitCount, QPoint offset,
                    qint32 loop = 1, float scale = 1.0f, short priority = 0, qint32 showDelay = 0);

    void loadMovingSprite(QString spriteID, bool addPlayerColor, qint32 maxUnitCount, QPoint offset,
                    QPoint startPoint, QPoint endPoint, qint32 moveTime,
                    qint32 loop = 1, float scale = 1.0f, short priority = 0, qint32 showDelay = 0);
    /**
     * @brief getImpactDurationMS
     * @return
     */
    qint32 getImpactDurationMS();
    /**
     * @brief getImpactDurationMS
     * @param pUnit
     * @return
     */
    qint32 getImpactDurationMS(Unit* pUnit);
    /**
     * @brief getFireDurationMS
     * @return
     */
    qint32 getFireDurationMS();
    /**
     * @brief getMoveInDurationMS
     * @return
     */
    qint32 getMoveInDurationMS();
    /**
     * @brief hasMoveInAnimation
     * @return
     */
    bool hasMoveInAnimation();
private:
    Unit* m_pUnit;
    Terrain* m_pTerrain;
    oxygine::spClipRectActor m_Actor;
    qint32 hpRounded{0};
};

#endif // BATTLEANIMATIONSPRITE_H
