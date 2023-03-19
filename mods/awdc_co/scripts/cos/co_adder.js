CO_ADDER.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_ADDER.activateSuperpower = function(co, powerMode, map)
{
	CO_ADDER.activatePower(co, map);
};
CO_ADDER.getSuperPowerDescription = function()
{
    return CO_ADDER.getPowerDescription();
};
CO_ADDER.getSuperPowerName = function()
{
    return CO_ADDER.getPowerName();
};
CO_ADDER.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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
            return CO_ADDER.coZoneBonus;
        }
        break;
    }
    return 0;
};
CO_ADDER.getMovementpointModifier = function(co, unit, posX, posY, map)
{
    if (co.getPowerMode() > GameEnums.PowerMode_Off)
    {
        return 1;
    }
    return 0;
};
