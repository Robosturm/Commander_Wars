CO_GRIMM.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_GRIMM.activateSuperpower = function(co, powerMode, map)
{
	CO_GRIMM.activatePower(co, powerMode, map);
};
CO_GRIMM.getSuperPowerDescription = function()
{
    return CO_GRIMM.getPowerDescription();
};
CO_GRIMM.getSuperPowerName = function()
{
    return CO_GRIMM.getPowerName();
};
CO_GRIMM.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 60;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return 50;
            }
            break;
    }
    return 30;
};
