CO_STURM.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(5);
};
CO_STURM.activateSuperpower = function(co, powerMode, map)
{
    CO_STURM.activatePower(co, powerMode);
};
CO_STURM.getSuperPowerDescription = function()
{
    return CO_STURM.getPowerDescription();
};
CO_STURM.getSuperPowerName = function()
{
    return CO_STURM.getPowerName();
};
CO_STURM.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, map)
{
    if (co.getIsCO0() === true)
    {
        return -20;
    }
    return 0;
};
CO_STURM.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 30;
        default:
            return 20;
        }
    }
    return 0;
};
CO_STURM.getMovementcostModifier = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        if (unit.getOwner() === co.getOwner())
        {
            if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SNOW")
            {
                return 0;
            }
            else
            {
                return -999;
            }
        }
    }
    return 0;
};
