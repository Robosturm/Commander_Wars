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
    explicit BattleAnimationSprite(Unit* pUnit, Terrain* pTerrain, QString animationType, qint32 hp = -1);
    /**
     * @brief loadAnimation
     * @param animationType
     */
    void loadAnimation(QString animationType);
    /**
     * @brief loadAnimation
     * @param animationType
     * @param pUnit
     */
    void loadAnimation(QString animationType, Unit* pUnit);
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
    void loadSprite(QString spriteID, bool addPlayerColor, qint32 maxUnitCount, QPoint offset);
private:
    Unit* m_pUnit;
    Terrain* m_pTerrain;
    oxygine::spClipRectActor m_Actor;
    qint32 hpRounded{0};
};

#endif // BATTLEANIMATIONSPRITE_H
