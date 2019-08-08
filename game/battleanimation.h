#ifndef BATTLEANIMATION_H
#define BATTLEANIMATION_H

#include <QObject>

#include "game/gameanimation.h"

#include "game/GameEnums.h"

#include <QTimer>

class BattleAnimation : public GameAnimation
{
    Q_OBJECT
public:
    BattleAnimation(Terrain* pAtkTerrain, Unit* pAtkUnit, float atkStartHp, float atkEndHp,
                    Terrain* pDefTerrain, Unit* pDefUnit, float defStartHp, float defEndHp);

    virtual void restart() override;
    virtual void stop() override;
public slots:

private:
    /**
     * @brief getIsRight
     * @param pUnit1
     * @param pUnit2
     * @return
     */
    bool getIsRight(Unit* pUnit1, Unit* pUnit2);
    /**
     * @brief setSpritePosition
     * @param pSprite
     * @param pAtkUnit
     * @param pDefUnit
     */
    void setSpritePosition(oxygine::spSprite pSprite, Unit* pUnit1, Unit* pUnit2);
    /**
     * @brief setCOMood
     * @param hp1
     * @param hp2
     */
    void setCOMood(oxygine::spSprite pSprite, float hp1, float hp2);
    /**
     * @brief loadTerrainSprite
     * @param pUnit
     * @return
     */
    oxygine::spSprite loadTerrainSprite(Unit* pUnit);
    /**
     * @brief setHealthBarColor
     * @param pColorBar
     * @param hp
     */
    void setHealthBarColor(oxygine::spColorRectSprite pColorBar, float hp);
    // dummy
    QTimer endTimer;

    oxygine::spSprite m_AtkCO0;
    oxygine::spSprite m_AtkCO1;
    oxygine::spSprite m_DefCO0;
    oxygine::spSprite m_DefCO1;

    oxygine::spColorRectSprite m_HealthBar0;
    oxygine::spColorRectSprite m_HealthBar1;

    static const short priorityBack = -1;
    static const short priorityFront = 1;
    static const short priorityCOBack = 2;
    static const short priorityCO = 3;
    static const short priorityCOFront = 4;
};

#endif // BATTLEANIMATION_H
