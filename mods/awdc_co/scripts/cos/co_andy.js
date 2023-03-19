CO_ANDY.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_ANDY.activateSuperpower = function(co, powerMode, map)
{
	CO_ANDY.activatePower(co, map);
};
CO_ANDY.getSuperPowerDescription = function()
{
    return CO_ANDY.getPowerDescription();
};
CO_ANDY.getSuperPowerName = function()
{
    return CO_ANDY.getPowerName();
};
CO_ANDY.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action, luckmode, map)
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
                return CO_ANDY.coZoneBonus;
            }
            break;
    }
    return 0;
};
CO_ANDY.getMovementpointModifier = function(co, unit, posX, posY, map)
{
    return 0;
};
