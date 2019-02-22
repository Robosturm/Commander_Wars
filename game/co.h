#ifndef KO_H
#define KO_H

#include <QObject>
#include <QPoint>

#include "oxygine-framework.h"

class Unit;
class Terrain;


class CO : public QObject, public oxygine::Actor
{
    Q_OBJECT
public:
    explicit CO(QString coID);

signals:

public slots:
    /**
     * @brief getMovementPointModifier the bonus movementpoints of this co
     * @param pUnit the unit we want to get the bonus points from
     * @return
     */
    qint32 getMovementPointModifier(Unit* pUnit);
    /**
     * @brief getFirerangeModifier the bonus firerange of this co
     * @param pUnit the unit we want to get the bonus points from
     * @return
     */
    qint32 getFirerangeModifier(Unit* pUnit, QPoint position);
    /**
     * @brief getTerrainDefenseModifier the bonus defense of this co for a terrain
     * @param pUnit the unit we want to get the bonus points from
     * @return
     */
    qint32 getTerrainDefenseModifier(Unit* pUnit, QPoint position);
    /**
     * @brief activatePower called when the power is activated
     */
    void activatePower();
    /**
     * @brief activateSuperpower called when the super power is activated
     */
    void activateSuperpower();
    /**
     * @brief getOffensiveBonus
     * @param pAttacker the attacking unit our unit
     * @param pDefender the defending unit the unit we attack
     * @return
     */
    qint32 getOffensiveBonus(Unit* pAttacker, QPoint atkPosition,Unit* pDefender,  QPoint defPosition);
    /**
     * @brief getDeffensiveBonus the deffensive bonus of our unit
     * @param pAttacker the unit attacking us
     * @param pDefender our unit that gets attacked
     * @return bonus
     */
    qint32 getDeffensiveBonus(Unit* pAttacker, QPoint atkPosition, Unit* pDefender, QPoint defPosition);
    /**
     * @brief getCoID our co id
     * @return
     */
    QString getCoID() const;
    /**
     * @brief getPowerStars stars needed to activate our power
     * @return
     */
    qint32 getPowerStars() const;
    /**
     * @brief setPowerStars stars needed to activate our power
     * @param value
     */
    void setPowerStars(const qint32 &value);
    /**
     * @brief getSuperpowerStars stars needed to activate our super power
     * @return
     */
    qint32 getSuperpowerStars() const;
    /**
     * @brief setSuperpowerStars stars needed to activate our super power
     * @param value
     */
    void setSuperpowerStars(const qint32 &value);

    float getPowerFilled() const;
    void setPowerFilled(const float &value);
    /**
     * @brief getCanMoveAndFire returns
     * @return
     */
    bool getCanMoveAndFire(Unit* pUnit, QPoint position);
private:
    QString coID;
    qint32 powerStars;
    qint32 superpowerStars;
    float powerFilled;
};

#endif // KO_H
