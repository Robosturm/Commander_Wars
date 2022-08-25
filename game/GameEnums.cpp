#include <QEasingCurve>

#include "game/GameEnums.h"

#include "coreengine/mainapp.h"
#include "coreengine/interpreter.h"

void GameEnums::registerEnums()
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue value = pInterpreter->newQObject(new GameEnums());
    value.setProperty("Alliance_Friend", Alliance_Friend);
    value.setProperty("Alliance_Enemy", Alliance_Enemy);

    value.setProperty("PowerMode_Unknown", PowerMode_Unknown);
    value.setProperty("PowerMode_Off", PowerMode_Off);
    value.setProperty("PowerMode_Power", PowerMode_Power);
    value.setProperty("PowerMode_Superpower", PowerMode_Superpower);
    value.setProperty("PowerMode_Tagpower", PowerMode_Tagpower);

    value.setProperty("RocketTarget_Money", RocketTarget_Money);
    value.setProperty("RocketTarget_HpHighMoney", RocketTarget_HpHighMoney);
    value.setProperty("RocketTarget_HpLowMoney", RocketTarget_HpLowMoney);

    value.setProperty("UnitRank_None", UnitRank_None);
    value.setProperty("UnitRank_CO0", UnitRank_CO0);
    value.setProperty("UnitRank_CO1", UnitRank_CO1);

    value.setProperty("Directions_None", Directions_None);
    value.setProperty("Directions_North", Directions_North);
    value.setProperty("Directions_NorthEast", Directions_NorthEast);
    value.setProperty("Directions_East", Directions_East);
    value.setProperty("Directions_SouthEast", Directions_SouthEast);
    value.setProperty("Directions_South", Directions_South);
    value.setProperty("Directions_SouthWest", Directions_SouthWest);
    value.setProperty("Directions_West", Directions_West);
    value.setProperty("Directions_NorthWest", Directions_NorthWest);
    value.setProperty("Directions_All", Directions_All);
    value.setProperty("Directions_Direct", Directions_Direct);
    value.setProperty("Directions_Diagnonal", Directions_Diagnonal);

    value.setProperty("AIQuestionType_Equal", AIQuestionType_Equal);
    value.setProperty("AIQuestionType_Smaler", AIQuestionType_Smaler);
    value.setProperty("AIQuestionType_Greater", AIQuestionType_Greater);
    value.setProperty("AIQuestionType_Between", AIQuestionType_Between);

    value.setProperty("DefeatType_Alive", DefeatType_Alive);
    value.setProperty("DefeatType_Defeated", DefeatType_Defeated);
    value.setProperty("DefeatType_ByCurrentPlayer", DefeatType_ByCurrentPlayer);
    value.setProperty("DefeatType_Domination", DefeatType_Domination);

    value.setProperty("Fog_Off", Fog_Off);
    value.setProperty("Fog_OfWar", Fog_OfWar);
    value.setProperty("Fog_OfShroud", Fog_OfShroud);
    value.setProperty("Fog_OfMist", Fog_OfMist);

    value.setProperty("VisionType_Shrouded", VisionType_Shrouded);
    value.setProperty("VisionType_Fogged", VisionType_Fogged);
    value.setProperty("VisionType_Clear", VisionType_Clear);

    value.setProperty("COMood_Normal", COMood_Normal);
    value.setProperty("COMood_Happy", COMood_Happy);
    value.setProperty("COMood_Sad", COMood_Sad);

    value.setProperty("LuckDamageMode_Off", LuckDamageMode_Off);
    value.setProperty("LuckDamageMode_On", LuckDamageMode_On);
    value.setProperty("LuckDamageMode_Average", LuckDamageMode_Average);
    value.setProperty("LuckDamageMode_Min", LuckDamageMode_Min);
    value.setProperty("LuckDamageMode_Max", LuckDamageMode_Max);

    value.setProperty("GameRecord_SpecialEvents_Power", GameRecord_SpecialEvents_Power);
    value.setProperty("GameRecord_SpecialEvents_SuperPower", GameRecord_SpecialEvents_SuperPower);
    value.setProperty("GameRecord_SpecialEvents_HQCaptured", GameRecord_SpecialEvents_HQCaptured);
    value.setProperty("GameRecord_SpecialEvents_HQLost", GameRecord_SpecialEvents_HQLost);

    value.setProperty("GameAi_Normal", GameAi_Normal);
    value.setProperty("GameAi_Offensive", GameAi_Offensive);
    value.setProperty("GameAi_Defensive", GameAi_Defensive);
    value.setProperty("GameAi_Hold", GameAi_Hold);    
    value.setProperty("GameAi_Patrol", GameAi_Patrol);
    value.setProperty("GameAi_PatrolLoop", GameAi_PatrolLoop);

    value.setProperty("BattleAnimationMode_None", BattleAnimationMode_None);
    value.setProperty("BattleAnimationMode_All", BattleAnimationMode_All);
    value.setProperty("BattleAnimationMode_Own", BattleAnimationMode_Own);
    value.setProperty("BattleAnimationMode_Ally", BattleAnimationMode_Ally);
    value.setProperty("BattleAnimationMode_Enemy", BattleAnimationMode_Enemy);

    value.setProperty("BuildingTarget_All", BuildingTarget_All);
    value.setProperty("BuildingTarget_Own", BuildingTarget_Own);
    value.setProperty("BuildingTarget_Enemy", BuildingTarget_Enemy);

    value.setProperty("UnitType_Ground", UnitType_Ground);
    value.setProperty("UnitType_Hovercraft", UnitType_Hovercraft);
    value.setProperty("UnitType_Infantry", UnitType_Infantry);
    value.setProperty("UnitType_Air", UnitType_Air);
    value.setProperty("UnitType_Naval", UnitType_Naval);

    value.setProperty("AiTurnMode_StartOfDay", AiTurnMode_StartOfDay);
    value.setProperty("AiTurnMode_DuringDay", AiTurnMode_DuringDay);
    value.setProperty("AiTurnMode_EndOfDay", AiTurnMode_EndOfDay);


    value.setProperty("AiTypes_MovePlanner", AiTypes_MovePlanner);
    value.setProperty("AiTypes_ProxyAi", AiTypes_ProxyAi);
    value.setProperty("AiTypes_Human", AiTypes_Human);
    value.setProperty("AiTypes_VeryEasy", AiTypes_VeryEasy);
    value.setProperty("AiTypes_Normal", AiTypes_Normal);
    value.setProperty("AiTypes_NormalOffensive", AiTypes_NormalOffensive);
    value.setProperty("AiTypes_NormalDefensive", AiTypes_NormalDefensive);
    value.setProperty("AiTypes_Heavy", AiTypes_Heavy);

    value.setProperty("RandomMapTerrainType_Group", RandomMapTerrainType_Group);
    value.setProperty("RandomMapTerrainType_Line", RandomMapTerrainType_Line);

    value.setProperty("Recoloring_None", Recoloring_None);
    value.setProperty("Recoloring_Mask", Recoloring_Mask);
    value.setProperty("Recoloring_Table", Recoloring_Table);
    value.setProperty("Recoloring_Matrix", Recoloring_Matrix);

    value.setProperty("AutoFocusing_LastPos", AutoFocusing_LastPos);
    value.setProperty("AutoFocusing_Owned", AutoFocusing_Owned);

    value.setProperty("WeaponType_Both", WeaponType_Both);
    value.setProperty("WeaponType_Direct", WeaponType_Direct);
    value.setProperty("WeaponType_Indirect", WeaponType_Indirect);

    value.setProperty("ShopItemType_CO", ShopItemType_CO);
    value.setProperty("ShopItemType_CO_Skin", ShopItemType_CO_Skin);
    value.setProperty("ShopItemType_Map", ShopItemType_Map);
    value.setProperty("ShopItemType_Unit", ShopItemType_Unit);
    value.setProperty("ShopItemType_Perk", ShopItemType_Perk);

    value.setProperty("PowerGainZone_Global", PowerGainZone_Global);
    value.setProperty("PowerGainZone_GlobalCoZoneBonus", PowerGainZone_GlobalCoZoneBonus);
    value.setProperty("PowerGainZone_OnlyCoZone", PowerGainZone_OnlyCoZone);

    value.setProperty("PowerGainMode_Money", PowerGainMode_Money);
    value.setProperty("PowerGainMode_Money_OnlyAttacker", PowerGainMode_Money_OnlyAttacker);
    value.setProperty("PowerGainMode_Hp", PowerGainMode_Hp);
    value.setProperty("PowerGainMode_Hp_OnlyAttacker", PowerGainMode_Hp_OnlyAttacker);

    value.setProperty("CounterAttackMode_Undefined", CounterAttackMode_Undefined);
    value.setProperty("CounterAttackMode_Impossible", CounterAttackMode_Impossible);
    value.setProperty("CounterAttackMode_Possible", CounterAttackMode_Possible);

    value.setProperty("MapFilterFlags_None", MapFilterFlags_None);
    value.setProperty("MapFilterFlags_Bases", MapFilterFlags_Bases);
    value.setProperty("MapFilterFlags_Airport", MapFilterFlags_Airport);
    value.setProperty("MapFilterFlags_Harbour", MapFilterFlags_Harbour);
    value.setProperty("MapFilterFlags_Tower", MapFilterFlags_Tower);
    value.setProperty("MapFilterFlags_Teleport", MapFilterFlags_Teleport);
    value.setProperty("MapFilterFlags_MixedBase", MapFilterFlags_MixedBase);
    value.setProperty("MapFilterFlags_Predeployed", MapFilterFlags_Predeployed);
    value.setProperty("MapFilterFlags_Symetric", MapFilterFlags_Symetric);
    value.setProperty("MapFilterFlags_PvP", MapFilterFlags_PvP);
    value.setProperty("MapFilterFlags_FTA", MapFilterFlags_FTA);
    value.setProperty("MapFilterFlags_VsAi", MapFilterFlags_VsAi);
    value.setProperty("MapFilterFlags_TeamPlay", MapFilterFlags_TeamPlay);
    value.setProperty("MapFilterFlags_Historical", MapFilterFlags_Historical);
    value.setProperty("MapFilterFlags_LowFunds", MapFilterFlags_LowFunds);
    value.setProperty("MapFilterFlags_HighFunds", MapFilterFlags_HighFunds);
    value.setProperty("MapFilterFlags_Naval", MapFilterFlags_Naval);
    value.setProperty("MapFilterFlags_Ground", MapFilterFlags_Ground);
    value.setProperty("MapFilterFlags_Air", MapFilterFlags_Air);
    value.setProperty("MapFilterFlags_Scripted", MapFilterFlags_Scripted);

    value.setProperty("FlowDirections_North", FlowDirections_North);
    value.setProperty("FlowDirections_South", FlowDirections_South);
    value.setProperty("FlowDirections_East",  FlowDirections_East);
    value.setProperty("FlowDirections_West",  FlowDirections_West);

    value.setProperty("AttackRangeCheck_All", AttackRangeCheck_All);
    value.setProperty("AttackRangeCheck_None", AttackRangeCheck_None);
    value.setProperty("AttackRangeCheck_OnlyMin", AttackRangeCheck_OnlyMin);
    value.setProperty("AttackRangeCheck_OnlyMax", AttackRangeCheck_OnlyMax);

    value.setProperty("AddinStepType_Field", AddinStepType_Field);
    value.setProperty("AddinStepType_Menu", AddinStepType_Menu);

    value.setProperty("LoginError_None", LoginError_None);
    value.setProperty("LoginError_WrongPassword", LoginError_WrongPassword);
    value.setProperty("LoginError_AccountDoesntExist", LoginError_AccountDoesntExist);
    value.setProperty("LoginError_AccountExists", LoginError_AccountExists);
    value.setProperty("LoginError_InvalidPasswordReset", LoginError_InvalidPasswordReset);
    value.setProperty("LoginError_PasswordOutdated", LoginError_PasswordOutdated);
    value.setProperty("LoginError_DatabaseNotAccesible", LoginError_DatabaseNotAccesible);
    value.setProperty("LoginError_SendingMailFailed", LoginError_SendingMailFailed);

    value.setProperty("QEasingCurve_Linear", QEasingCurve::Linear);
    value.setProperty("QEasingCurve_InQuad", QEasingCurve::InQuad);
    value.setProperty("QEasingCurve_OutQuad", QEasingCurve::OutQuad);
    value.setProperty("QEasingCurve_InOutQuad", QEasingCurve::InOutQuad);
    value.setProperty("QEasingCurve_OutInQuad", QEasingCurve::OutInQuad);
    value.setProperty("QEasingCurve_InCubic", QEasingCurve::InCubic);
    value.setProperty("QEasingCurve_OutCubic", QEasingCurve::OutCubic);
    value.setProperty("QEasingCurve_InOutCubic", QEasingCurve::InOutCubic);
    value.setProperty("QEasingCurve_OutInCubic", QEasingCurve::OutInCubic);
    value.setProperty("QEasingCurve_InQuart", QEasingCurve::InQuart);
    value.setProperty("QEasingCurve_OutQuart", QEasingCurve::OutQuart);
    value.setProperty("QEasingCurve_InOutQuart", QEasingCurve::InOutQuart);
    value.setProperty("QEasingCurve_OutInQuart", QEasingCurve::OutInQuart);
    value.setProperty("QEasingCurve_InQuint", QEasingCurve::InQuint);
    value.setProperty("QEasingCurve_OutQuint", QEasingCurve::OutQuint);
    value.setProperty("QEasingCurve_InOutQuint", QEasingCurve::InOutQuint);
    value.setProperty("QEasingCurve_OutInQuint", QEasingCurve::OutInQuint);
    value.setProperty("QEasingCurve_InSine", QEasingCurve::InSine);
    value.setProperty("QEasingCurve_OutSine", QEasingCurve::OutSine);
    value.setProperty("QEasingCurve_InOutSine", QEasingCurve::InOutSine);
    value.setProperty("QEasingCurve_OutInSine", QEasingCurve::OutInSine);
    value.setProperty("QEasingCurve_InExpo", QEasingCurve::InExpo);
    value.setProperty("QEasingCurve_OutExpo", QEasingCurve::OutExpo);
    value.setProperty("QEasingCurve_InOutExpo", QEasingCurve::InOutExpo);
    value.setProperty("QEasingCurve_OutInExpo", QEasingCurve::OutInExpo);
    value.setProperty("QEasingCurve_InCirc", QEasingCurve::InCirc);
    value.setProperty("QEasingCurve_OutCirc", QEasingCurve::OutCirc);
    value.setProperty("QEasingCurve_InOutCirc", QEasingCurve::InOutCirc);
    value.setProperty("QEasingCurve_OutInirc", QEasingCurve::OutInCirc);
    value.setProperty("QEasingCurve_InElastic", QEasingCurve::InElastic);
    value.setProperty("QEasingCurve_OutElastic", QEasingCurve::OutElastic);
    value.setProperty("QEasingCurve_InOutElastic", QEasingCurve::InOutElastic);
    value.setProperty("QEasingCurve_OutInElastic", QEasingCurve::OutInElastic);
    value.setProperty("QEasingCurve_InBack", QEasingCurve::InBack);
    value.setProperty("QEasingCurve_OutBack", QEasingCurve::OutBack);
    value.setProperty("QEasingCurve_InOutBack", QEasingCurve::InOutBack);
    value.setProperty("QEasingCurve_OutInBack", QEasingCurve::OutInBack);
    value.setProperty("QEasingCurve_InBounce", QEasingCurve::InBounce);
    value.setProperty("QEasingCurve_OutBounce", QEasingCurve::OutBounce);
    value.setProperty("QEasingCurve_InOutBounce", QEasingCurve::InOutBounce);
    value.setProperty("QEasingCurve_OutInBounce", QEasingCurve::OutInBounce);
    value.setProperty("QEasingCurve_InElastic", QEasingCurve::InCurve);
    value.setProperty("QEasingCurve_OutElastic", QEasingCurve::OutCurve);
    value.setProperty("QEasingCurve_InOutElastic", QEasingCurve::SineCurve);
    value.setProperty("QEasingCurve_OutInElastic", QEasingCurve::CosineCurve);

    pInterpreter->setGlobal("GameEnums", value);
}

