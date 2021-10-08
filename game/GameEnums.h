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
        PowerMode_Unknown = -1,
        PowerMode_Off,
        PowerMode_Power,
        PowerMode_Superpower,
        PowerMode_Tagpower
    };
    Q_ENUM(PowerMode)

    enum RocketTarget
    {
        RocketTarget_Money, /**< targets the most money damage witht the rocket */
        RocketTarget_HpLowMoney, /**< targets most hp damage where units which costs less than half of the average funds count four time */
        RocketTarget_HpHighMoney /**< targets most hp damage where units which costs more than half of the average funds count four time */
    };
    Q_ENUM(RocketTarget)

    enum UnitRanks
    {
        UnitRank_CO0 = -1,
        UnitRank_CO1 = -2,
        UnitRank_None = 0,
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
        DefeatType_Defeated, /**< all units will get destroyed and the player loses the game */
        DefeatType_ByCurrentPlayer, /**< all buildings will get controlled by the current player and all units will get destroyed */
        DefeatType_Domination /**< all units and buildings will get controlled by the current player */
    };
    Q_ENUM(DefeatType)

    enum Fog
    {
        Fog_Off,
        Fog_OfWar,
        Fog_OfShroud,
        Fog_OfMist,
    };
    Q_ENUM(Fog)

    enum VisionType
    {
        VisionType_Shrouded,
        VisionType_Fogged,
        VisionType_Clear,
        VisionType_Mist,
    };
    Q_ENUM(VisionType)

    enum COMood
    {
        COMood_Normal,
        COMood_Happy,
        COMood_Sad
    };
    Q_ENUM(COMood)

    enum LuckDamageMode
    {
        LuckDamageMode_Off,
        LuckDamageMode_On,
        LuckDamageMode_Average,
        LuckDamageMode_Min,
        LuckDamageMode_Max,
    };
    Q_ENUM(LuckDamageMode)

    enum GameRecord_SpecialEvents
    {
        GameRecord_SpecialEvents_Power,
        GameRecord_SpecialEvents_SuperPower,
        GameRecord_SpecialEvents_HQCaptured,
        GameRecord_SpecialEvents_HQLost
    };
    Q_ENUM(GameRecord_SpecialEvents)

    enum GameAi
    {
        GameAi_Normal,
        GameAi_Offensive,
        GameAi_Defensive,
        GameAi_Hold,
        GameAi_Patrol,
        GameAi_PatrolLoop,
    };
    Q_ENUM(GameAi)

    enum BattleAnimationMode
    {
        BattleAnimationMode_None,
        BattleAnimationMode_All,
        BattleAnimationMode_Own,
        BattleAnimationMode_Ally,
        BattleAnimationMode_Enemy,
        BattleAnimationMode_OnlyBattleAll,
        BattleAnimationMode_OnlyBattleOwn,
        BattleAnimationMode_OnlyBattleAlly,
        BattleAnimationMode_OnlyBattleEnemy,
    };
    Q_ENUM(BattleAnimationMode)

    enum BattleAnimationType
    {
        BattleAnimationType_Detail,
        BattleAnimationType_Overworld,
        BattleAnimationType_Fullscreen,
        BattleAnimationType_DetailTransparent,
        BattleAnimationType_FullscreenTransparent,
    };
    Q_ENUM(BattleAnimationType)

    enum BuildingTarget
    {
        BuildingTarget_All,
        BuildingTarget_Own,
        BuildingTarget_Enemy,
    };
    Q_ENUM(BuildingTarget)

    enum UnitType
    {
        UnitType_Ground = 1,
        UnitType_Hovercraft = 2,
        UnitType_Infantry = 4,
        UnitType_Air = 8,
        UnitType_Naval = 16,
    };
    Q_ENUM(UnitType)

    enum AiTurnMode
    {
        AiTurnMode_StartOfDay,
        AiTurnMode_DuringDay,
        AiTurnMode_EndOfDay,
    };
    Q_ENUM(AiTurnMode)

    enum AiTypes
    {
        AiTypes_ProxyAi = -1,
        AiTypes_Human = 0,
        AiTypes_VeryEasy,
        AiTypes_Normal,
        AiTypes_NormalOffensive,
        AiTypes_NormalDefensive,
        AiTypes_Heavy,
        AiTypes_Closed = 199,
        AiTypes_Open,
    };
    Q_ENUM(AiTypes)

    enum RandomMapTerrainType
    {
        RandomMapTerrainType_Group,
        RandomMapTerrainType_Line,
    };
    Q_ENUM(RandomMapTerrainType)

    enum Recoloring
    {
        Recoloring_None,
        Recoloring_Mask,
        Recoloring_Table,
        Recoloring_Matrix
    };
    Q_ENUM(Recoloring)

    enum COInfoPosition
    {
        COInfoPosition_Flipping,
        COInfoPosition_Left,
        COInfoPosition_Right,
    };
    Q_ENUM(COInfoPosition)

    enum AutoFocusing
    {
        AutoFocusing_LastPos,
        AutoFocusing_Owned,
    };
    Q_ENUM(AutoFocusing)

    enum WeaponType
    {
        WeaponType_Both,
        WeaponType_Direct,
        WeaponType_Indirect,
    };
    Q_ENUM(WeaponType)

    enum ShopItemType
    {
        ShopItemType_All = -1,
        ShopItemType_CO,
        ShopItemType_CO_Skin,
        ShopItemType_Map,
        ShopItemType_Unit,
        ShopItemType_Perk,
        ShopItemType_Max,
    };
    Q_ENUM(ShopItemType)

    enum PowerGainZone
    {
        PowerGainZone_Global,
        PowerGainZone_GlobalCoZoneBonus,
        PowerGainZone_OnlyCoZone,
    };
    Q_ENUM(PowerGainZone)

    enum PowerGainMode
    {
        PowerGainMode_Money,
        PowerGainMode_Money_OnlyAttacker,
        PowerGainMode_Hp,
        PowerGainMode_Hp_OnlyAttacker,
    };
    Q_ENUM(PowerGainMode)

    enum CounterAttackMode
    {
        CounterAttackMode_Undefined,
        CounterAttackMode_Impossible,
        CounterAttackMode_Possible,
    };
    Q_ENUM(CounterAttackMode)

    static void registerEnums();
private:
    explicit GameEnums() = default;
    virtual ~GameEnums() = default;
};

#endif
