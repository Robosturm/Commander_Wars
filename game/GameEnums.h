#ifndef GAMEENUMS_H
#define GAMEENUMS_H

#include <QObject>

class GameEnums : public QObject
{
    Q_OBJECT
public:
    enum Alliance
    {
        Alliance_Friend,
        Alliance_Enemy
    };
    Q_ENUM(Alliance)

    enum PowerMode
    {
        PowerMode_Off,
        PowerMode_Power,
        PowerMode_Superpower
    };
    Q_ENUM(PowerMode)

    enum RocketTarget
    {
        RocketTarget_Money, /**< targets the most money damage witht the rocket */
        RocketTarget_HpLowMoney, /**< targets most hp damage where units which costs less than half of the average fonds count four time */
        RocketTarget_HpHighMoney /**< targets most hp damage where units which costs more than half of the average fonds count four time */
    };
    Q_ENUM(RocketTarget)

    enum UnitRanks
    {
        UnitRank_None,
        UnitRank_Lieutenant,
        UnitRank_General,
        UnitRank_Veteran,
        UnitRank_CO0,
        UnitRank_CO1
    };
    Q_ENUM(UnitRanks)

    enum Directions
    {
        Directions_None = -1,
        Directions_North,
        Directions_NorthEast,
        Directions_East,
        Directions_SouthEast,
        Directions_South,
        Directions_SouthWest,
        Directions_West,
        Directions_NorthWest,
        Directions_All,
        Directions_Direct,
        Directions_Diagnonal
    };
    Q_ENUM(Directions)

    enum AIQuestionType
    {
        AIQuestionType_Equal,
        AIQuestionType_Smaler,
        AIQuestionType_Greater,
        AIQuestionType_Between
    };
    Q_ENUM(AIQuestionType)

    enum DefeatType
    {
        DefeatType_Alive, /**< the player will not loose the game cause of this rule for now */
        DefeatType_Defeated, /**< all units will get destroyed and the player looses the game */
        DefeatType_ByCurrentPlayer, /**< all buildings will get controlled by the current player and all units will get destroyed */
        DefeatType_Domination /**< all units and buildings will get controlled by the current player */
    };
    Q_ENUM(DefeatType)

    enum Fog
    {
        Fog_Off,
        Fog_OfWar
    };
    Q_ENUM(Fog)

    enum COMood
    {
        COMood_Normal,
        COMood_Happy,
        COMood_Sad
    };
    Q_ENUM(COMood)

    static void registerEnums();
private:
    explicit GameEnums() = default;
    virtual ~GameEnums() = default;
};

#endif
