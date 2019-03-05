#ifndef KO_H
#define KO_H

#include <QObject>
#include <QPoint>

#include "game/GameEnums.h"

#include "oxygine-framework.h"

#include "game/smartUnit.h"

#include "coreengine/fileserializable.h"

class Unit;
class Terrain;
class Player;

class CO : public QObject, public oxygine::Actor, public FileSerializable
{
    Q_OBJECT
public:
    explicit CO(QString coID, Player* owner);
    /**
     * @brief setCOUnit
     * @param pUnit
     * @return
     */
    void setCOUnit(spUnit pUnit);
    /**
     * @brief serialize stores the object
     * @param pStream
     */
    virtual void serialize(QDataStream& pStream) override;
    /**
     * @brief deserialize restores the object
     * @param pStream
     */
    virtual void deserialize(QDataStream& pStream) override;
    /**
     * @brief getVersion
     * @return
     */
    virtual qint32 getVersion()
    {
        return 1;
    }
    void init();
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
     * @brief getMovementpointModifier
     * @param pUnit
     * @param position
     * @return
     */
    qint32 getMovementpointModifier(Unit* pUnit, QPoint position);
    /**
     * @brief getAttackHpBonus
     * @param pUnit
     * @param position
     * @return
     */
    qint32 getAttackHpBonus(Unit* pUnit, QPoint position);
    /**
     * @brief getBonusLuck
     * @param pUnit
     * @param position
     * @return
     */
    qint32 getBonusLuck(Unit* pUnit, QPoint position);
    /**
     * @brief getBonusMisfortune
     * @param pUnit
     * @param position
     * @return
     */
    qint32 getBonusMisfortune(Unit* pUnit, QPoint position);
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
    /**
     * @brief canUseSuperpower
     * @return
     */
    bool canUseSuperpower() const;
    /**
     * @brief canUsePower
     * @return
     */
    bool canUsePower() const;
    /**
     * @brief getPowerFilled
     * @return
     */
    float getPowerFilled() const;
    /**
     * @brief setPowerFilled
     * @param value
     */
    void setPowerFilled(const float &value);
    /**
     * @brief getCanMoveAndFire returns
     * @return
     */
    bool getCanMoveAndFire(Unit* pUnit, QPoint position);
    /**
     * @brief getRepairBonus
     * @param pUnit
     * @param position
     * @return
     */
    qint32 getRepairBonus(Unit* pUnit, QPoint position);
    /**
     * @brief getCaptureBonus
     * @param pUnit
     * @param position
     * @return
     */
    qint32 getCaptureBonus(Unit* pUnit, QPoint position);
    /**
     * @brief getCostModifier
     * @param baseCost
     * @return
     */
    qint32 getCostModifier(QString id, qint32 baseCost);
    /**
     * @brief getCOArmy
     */
    QString getCOArmy();
    /**
     * @brief getPowerMode
     * @return
     */
    GameEnums::PowerMode getPowerMode() const;
    /**
     * @brief setPowerMode
     * @param PowerMode
     */
    void setPowerMode(const GameEnums::PowerMode &PowerMode);
    /**
     * @brief gainPower
     * @param fondsDamage
     * @param position
     */
    void gainPowerstar(qint32 fondsDamage, QPoint position);
    /**
     * @brief startOfTurn called at the start of our turn
     */
    void startOfTurn();
    /**
     * @brief getPlayer
     * @return
     */
    inline Player* getPlayer()
    {
        return m_Owner;
    }
    /**
     * @brief getCOUnit
     * @return
     */
    Unit* getCOUnit();
    /**
     * @brief getCORange
     * @return
     */
    qint32 getCORange();
    /**
     * @brief inCORange
     * @param position
     * @return
     */
    bool inCORange(QPoint position);
    /**
     * @brief getFuelCostModifier
     * @param pUnit
     * @param position
     * @param costs
     * @return
     */
    qint32 getFuelCostModifier(Unit* pUnit, QPoint position, qint32 costs);
private:
    Player* m_Owner;
    QString coID;
    qint32 powerStars{0};
    qint32 superpowerStars{0};
    float powerFilled{0.0f};
    spUnit m_COUnit;
    GameEnums::PowerMode m_PowerMode{GameEnums::PowerMode_Off};
};

#endif // KO_H
