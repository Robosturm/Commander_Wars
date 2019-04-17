#ifndef COREAI_H
#define COREAI_H

#include <qvector.h>
#include <qvector3d.h>
#include <QRectF>

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
    void getBestTarget(Unit* pUnit, GameAction* pAction, UnitPathFindingSystem* pPfs, QVector<QVector3D>& ret, QVector<QPoint>& moveTargetFields);
    /**
     * @brief calcUnitDamage
     * @param pUnit
     * @param position
     * @param target
     * @return x = unit damage and y = counter damage
     */
    QRectF calcUnitDamage(GameAction* pAction, QPoint target);
    /**
     * @brief getAttacksFromField
     * @param pUnit
     * @param pAction
     * @param ret
     */
    void getAttacksFromField(Unit* pUnit, GameAction* pAction, QVector<QVector3D>& ret, QVector<QPoint>& moveTargetFields);
protected:

    static const QString ACTION_CAPTURE;
    static const QString ACTION_FIRE;
    static const QString ACTION_NEXT_PLAYER;
private:
    bool finish{false};
};

#endif // COREAI_H
