#ifndef PLAYER_H
#define PLAYER_H

#include <tuple>
#include <QColor>
#include <QObject>
#include <QVector>
#include <QVector3D>
#include "oxygine-flow.h"
#include "game/smartpointers.h"
#include "game/smartCO.h"

#include "coreengine/mainapp.h"

#include "coreengine/fileserializable.h"

#include "game/GameEnums.h"

#include "gameinput/basegameinputif.h"

class CO;

class Player : public QObject, public oxygine::Actor, public FileSerializable
{
    Q_OBJECT

public:

    explicit Player();
    void init();
    virtual ~Player() override;
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
     * @brief getVersion version of the file
     * @return
     */
    inline virtual qint32 getVersion() override
    {
        return 6;
    }
    /**
     * @brief getBaseGameInput pointer to the ai or human player interface
     * @return
     */
    inline BaseGameInputIF* getBaseGameInput()
    {
        return m_pBaseGameInput.get();
    }
    /**
     * @brief setBaseGameInput sets the player input
     * @param pBaseGameInput
     */
    void setBaseGameInput(BaseGameInputIF *pBaseGameInput);

signals:

public slots:
    /**
     * @brief getIsDefeated
     * @return
     */
    bool getIsDefeated() const;
    /**
     * @brief swapCOs swaps co 0 and 1
     */
    void swapCOs();
    /**
     * @brief getColor the color of this player
     * @return
     */
    QColor getColor() const;
    /**
     * @brief setColor sets the color of this player. Note this want update existing sprites
     * @param Color
     */
    void setColor(QColor Color);
    /**
     * @brief getPlayerID player id of this player from 0 to n
     * @return
     */
    qint32 getPlayerID() const;
    /**
     * @brief getArmy the army string id of this player.
     * @return
     */
    QString getArmy();
    /**
     * @brief isEnemy checks the alliance with this player
     * @param pPlayer the player we want to check if he's an enemy
     * @return the alliance of the player with us
     */
    GameEnums::Alliance checkAlliance(Player* pPlayer);
    /**
     * @brief isEnemyUnit checks if the given unit is an enemy
     * @param pUnit
     * @return
     */
    bool isEnemyUnit(Unit* pUnit);
    /**
     * @brief isEnemy
     * @param pOwner
     * @return
     */
    bool isEnemy(Player* pOwner);
    qint32 getFonds() const;
    /**
     * @brief addFonds increases the money of this player by the given value
     * @param value
     */
    void addFonds(const qint32 &value);
    /**
     * @brief setFonds
     * @param value
     */
    void setFonds(const qint32 &value);
    /**
     * @brief earnMoney earns money based on the buildings the player has
     * @param modifier multiplier additionaly modifying the income
     */
    void earnMoney(float modifier = 1.0f);
    /**
     * @brief getCO
     * @param id index of the co 0 or 1
     * @return pointer to the co
     */
    CO* getCO(quint8 id);
    /**
     * @brief setCO
     * @param coId the co we want to load
     * @param id the index at which we want this co
     */
    void setCO(QString coId, quint8 idx);
    /**
     * @brief getCostModifier
     * @param baseCost
     * @return
     */
    qint32 getCostModifier(QString id, qint32 baseCost);
    /**
     * @brief getCosts
     * @param id
     * @return
     */
    qint32 getCosts(QString id);
    /**
     * @brief gainPowerstar
     * @param fondsDamage
     */
    void gainPowerstar(qint32 fondsDamage, QPoint position);
    /**
     * @brief startOfTurn
     */
    void startOfTurn();
    /**
     * @brief getUnits
     * @return
     */
    QmlVectorUnit* getUnits();
    /**
     * @brief getBuildings
     * @return
     */
    QmlVectorBuilding* getBuildings();
    /**
     * @brief updateCORange
     */
    void updateVisualCORange();
    /**
     * @brief getMovementpointModifier
     * @param pUnit
     * @param position
     * @return
     */
    qint32 getMovementpointModifier(Unit* pUnit, QPoint position);
    /**
     * @brief getRockettarget finds the most valuable target for damaging units in a diamond.
     * If multiple targets with the same value are found.
     * One is selected at random
     * @param radius
     * @param damage
     * @param ownUnitValue value of own or allied units compared to enemy ones.
     * @return -1, -1 for no target found
     */
    QPoint getRockettarget(qint32 radius, qint32 damage, float ownUnitValue = 1.0f, GameEnums::RocketTarget targetType = GameEnums::RocketTarget_Money);
    /**
     * @brief defineArmy defines our army sprites based on the current co at position 0
     */
    void defineArmy();
    /**
     * @brief getFondsModifier
     * @return
     */
    float getFondsModifier() const;
    /**
     * @brief setFondsModifier
     * @param value
     */
    void setFondsModifier(float value);
    /**
     * @brief getBuildingCount
     * @return
     */
    qint32 getBuildingCount(QString buildingID = "");
    /**
     * @brief getUnitCount
     * @return
     */
    qint32 getUnitCount(QString unitID = "");
    /**
     * @brief getTeam
     * @return
     */
    qint32 getTeam() const;
    /**
     * @brief setTeam
     * @param value
     */
    void setTeam(const qint32 &value);
    /**
     * @brief defeatPlayer defeats the player and removes him from further playing
     * @param pPLayer none nullptr will give all buildings to the selected player
     * @param units if units is true the player will also get the controll of the given player
     */
    void defeatPlayer(Player* pPLayer, bool units = false);
    /**
     * @brief postBattleActions
     * @param pAttacker
     * @param pDefender
     */
    void postBattleActions(Unit* pAttacker, float atkDamage, Unit* pDefender);
    /**
     * @brief updatePlayerVision updates the current vision of this player.
     * @brief reduces the timer for vision created for several turns
     */
    void updatePlayerVision(bool reduceTimer = false);
    /**
     * @brief addVisionField
     * @param x
     * @param y
     * @param duration
     */
    void addVisionField(qint32 x, qint32 y, qint32 duration = 1);
    /**
     * @brief getFieldVisible
     * @param x
     * @param y
     */
    bool getFieldVisible(qint32 x, qint32 y);
    /**
     * @brief loadVisionFields
     */
    void loadVisionFields();
private:
    qint32 fonds{0};
    float fondsModifier{1.0f};

    QColor m_Color;
    QString playerArmy{""};
    qint32 team{0};
    spCO playerCOs[2]{nullptr, nullptr};
    /**
     * @brief m_pBaseGameInput pointer to the ai or human player
     */
    spBaseGameInputIF m_pBaseGameInput{nullptr};
    bool isDefeated{false};
    QVector<QVector<QPoint>> m_FogVisionFields;
};

#endif // PLAYER_H
