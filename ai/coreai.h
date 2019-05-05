#ifndef COREAI_H
#define COREAI_H

#include <qvector.h>
#include <qvector3d.h>
#include <QRectF>

#include "gameinput/basegameinputif.h"

#include "ai/decisionquestion.h"

class GameAction;
class Unit;
class UnitPathFindingSystem;
class QmlVectorUnit;
class QmlVectorBuilding;
class QmlVectorPoint;
class Building;

class CoreAI : public BaseGameInputIF
{
    Q_OBJECT
public:
    // static string list of actions so we only define them once
    static const QString ACTION_WAIT;
    static const QString ACTION_REPAIR;
    static const QString ACTION_RATION;
    static const QString ACTION_UNSTEALTH;
    static const QString ACTION_STEALTH;
    static const QString ACTION_BUILD_UNITS;
    static const QString ACTION_CAPTURE;
    static const QString ACTION_MISSILE;
    static const QString ACTION_FIRE;
    static const QString ACTION_UNLOAD;
    static const QString ACTION_LOAD;
    static const QString ACTION_NEXT_PLAYER;
    static const QString ACTION_ACTIVATE_POWER_CO_0;
    static const QString ACTION_ACTIVATE_POWER_CO_1;
    static const QString ACTION_ACTIVATE_SUPERPOWER_CO_0;
    static const QString ACTION_ACTIVATE_SUPERPOWER_CO_1;
    static const QString ACTION_CO_UNIT_0;
    static const QString ACTION_CO_UNIT_1;

    explicit CoreAI(BaseGameInputIF::AiTypes aiType);
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
    /**
     * @brief getEnableBuildingAttack
     * @return
     */
    bool getEnableBuildingAttack() const;
    /**
     * @brief setEnableBuildingAttack
     * @param value
     */
    void setEnableBuildingAttack(bool value);
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
    /**
     * @brief appendAttackTargets
     * @param pUnit
     * @param pEnemyUnits
     * @param targets
     */
    void appendAttackTargets(Unit* pUnit, QmlVectorUnit* pEnemyUnits, QVector<QPoint>& targets);
    /**
     * @brief getTrainingData reads the training data from a training file for a pipeline either decision tree or neural network
     * @param file
     * @param trainingData
     * @param questions
     */
    static void getTrainingData(QString file, QVector<QVector<float>>& trainingData, QVector<QVector<spDecisionQuestion>>& questions);
protected:
    void addMenuItemData(GameAction* pGameAction, QString itemID, qint32 cost);
    void addSelectedFieldData(GameAction* pGameAction, QPoint point);

    /**
     * @brief processPredefinedAi
     * @return
     */
    bool processPredefinedAi();
    void processPredefinedAiHold(Unit* pUnit);
    void processPredefinedAiDefensive(Unit* pUnit);
    void processPredefinedAiOffensive(Unit* pUnit, QmlVectorUnit* pEnemyUnits);
private:
    bool finish{false};
    bool enableBuildingAttack{true};
};

#endif // COREAI_H
