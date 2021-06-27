CO_HACHI.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_HACHI.activateSuperpower = function(co, powerMode)
{
	CO_HACHI.activatePower(co, powerMode);
};
CO_HACHI.getSuperPowerDescription = function()
{
    return CO_HACHI.getPowerDescription();
};
CO_HACHI.getSuperPowerName = function()
{
    return CO_HACHI.getPowerName();
};
CO_HACHI.getAdditionalBuildingActions = function(co, building)
{
    return "";
};

CO_HACHI.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 10;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return 10;
            }
            break;
    }
    return 0;
};
