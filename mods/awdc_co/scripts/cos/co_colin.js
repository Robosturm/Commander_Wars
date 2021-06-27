CO_COLIN.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_COLIN.activateSuperpower = function(co, powerMode)
{
	CO_COLIN.activatePower(co, powerMode);
};
CO_COLIN.getSuperPowerDescription = function()
{
    return CO_COLIN.getPowerDescription();
};
CO_COLIN.getSuperPowerName = function()
{
    return CO_COLIN.getPowerName();
};
CO_COLIN.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return -5;
        default:
            break;
    }
    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
    {
        return -CO_COLIN.coZoneBoost;
    }
    return -CO_COLIN.globalBoost;
};
