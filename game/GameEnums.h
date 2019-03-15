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
        RocketTarget_Money,
        RocketTarget_HpLowMoney,
        RocketTarget_HpHighMoney
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

    static void registerEnums();
private:
    explicit GameEnums() = default;
    virtual ~GameEnums() = default;
};

#endif
