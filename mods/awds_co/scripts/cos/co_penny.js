CO_PENNY.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        if (map !== null)
        {
            switch (co.getPowerMode())
            {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    return 30;
                case GameEnums.PowerMode_Power:
                    return 10;
                default:
                    break;
            }
        }
    }
    return 0;
};
CO_PENNY.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isAttacker, action, luckmode, map)
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
CO_PENNY.getFirerangeModifier = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        if (map.getGameRules().getCurrentWeather() !== null &&
            map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM" &&
            unit.getBaseMaxRange() > 1)
        {
            if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                co.getPowerMode() === GameEnums.PowerMode_Tagpower)
            {
                return 2;
            }
        }
    }
    return 0;
};

CO_PENNY.getWeatherImmune = function(co, map)
{
    if (co.getIsCO0() === true)
    {
        return true;
    }
    return false;
};

CO_PENNY.getMovementpointModifier = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
            co.getPowerMode() === GameEnums.PowerMode_Tagpower)
        {
            if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SNOW")
            {
                return 2;
            }
        }
    }
    return 0;
};

CO_PENNY.getVisionrangeModifier = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        if (map.getGameRules().getCurrentWeather() !== null &&
            map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_RAIN")
        {
            if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                co.getPowerMode() === GameEnums.PowerMode_Tagpower)
            {
                return 3;
            }
        }
    }
    return 0;
};

CO_PENNY.getPerfectVision = function(co, map)
{
    if (co.getIsCO0() === true)
    {
        if (map.getGameRules().getCurrentWeather() !== null &&
            map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_RAIN")
        {
            if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                co.getPowerMode() === GameEnums.PowerMode_Tagpower)
            {
                return true;
            }
        }
    }
    return false;
};
