#ifndef COREAI_H
#define COREAI_H

#include <qvector3d.h>
#include <qvector2d.h>

#include "gameinput/basegameinputif.h"

class GameAction;
class Unit;
class UnitPathFindingSystem;

class CoreAI : public BaseGameInputIF
{
    Q_OBJECT
public:
    explicit CoreAI();
    virtual ~CoreAI();
    /**
     * @brief init
     */
    virtual void init() override;
    /**
     * @brief process
     */
    virtual void process() = 0;
signals:
    /**
     * @brief performAction signal with an action to be performed the action has to be deleted by the reciever of this slot. Only one slot can be connected to this signal
     * @param pAction
     */
    void performAction(GameAction* pAction);
public slots:
    void nextAction();
    /**
     * @brief getBestTarget
     * @param pUnit
     * @param pAction
     * @param pPfs
     * @return target unit x, y and z = fonddamage
     */
    QVector3D getBestTarget(Unit* pUnit, GameAction* pAction, UnitPathFindingSystem* pPfs);
    /**
     * @brief calcUnitDamage
     * @param pUnit
     * @param position
     * @param target
     * @return x = unit damage and y = counter damage
     */
    QVector2D calcUnitDamage(Unit* pUnit, QPoint position, QPoint target);
protected:

    static const QString ACTION_CAPTURE;
    static const QString ACTION_FIRE;
    static const QString ACTION_NEXT_PLAYER;
private:
    bool finish{false};
};

#endif // COREAI_H
