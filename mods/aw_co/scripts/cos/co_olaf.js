CO_OLAF.init = function(co)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_OLAF.activateSuperpower = function(co, powerMode)
{
    CO_OLAF.activatePower(co, powerMode);
};
CO_OLAF.getSuperPowerDescription = function()
{
    return CO_OLAF.getPowerDescription();
};
CO_OLAF.getSuperPowerName = function()
{
    return CO_OLAF.getPowerName();
};
CO_OLAF.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                     defender, defPosX, defPosY, isDefender, action)
{
    return 0;
};

CO_OLAF.getWeatherImmune = function(co)
{
    if (co.getIsCO0() === true)
    {
        if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SNOW")
        {
            return true;
        }
    }
    return false;
};

CO_OLAF.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
    }
    return 0;
};
