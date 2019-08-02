#ifndef BATTLEANIMATIONSPRITE_H
#define BATTLEANIMATIONSPRITE_H

#include <QObject>

#include "oxygine-framework.h"

#include "game/unit.h"

class BattleAnimationSprite;
typedef oxygine::intrusive_ptr<BattleAnimationSprite> spBattleAnimationSprite;

class BattleAnimationSprite : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    static const QString standingAnimation;
    explicit BattleAnimationSprite(Unit* pUnit, Terrain* pTerrain, QString animationType);


signals:

public slots:
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
};

#endif // BATTLEANIMATIONSPRITE_H
