CO_ALEXANDER.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_ALEXANDER.activateSuperpower = function(co, powerMode, map)
{
	CO_ALEXANDER.activatePower(co, powerMode, map);
};
CO_ALEXANDER.getSuperPowerDescription = function()
{
    return CO_ALEXANDER.getPowerDescription();
};
CO_ALEXANDER.getSuperPowerName = function()
{
    return CO_ALEXANDER.getPowerName();
};
CO_ALEXANDER.getIncomeReduction = function(co, building, income, map)
{
    return 0;
};
CO_ALEXANDER.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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
            return CO_ALEXANDER.coZoneBonus;
        }
        break;
    }
    return 0;
};

CO_ALEXANDER.getCaptureBonus = function(co, unit, posX, posY, map)
{
    if (co.getPowerMode() > GameEnums.PowerMode_Off)
    {
        var hp = unit.getHpRounded();
        return hp / 2;
    }
    return 0;
};
