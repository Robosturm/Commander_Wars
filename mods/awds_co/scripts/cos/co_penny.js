CO_PENNY.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        if (typeof map !== 'undefined')
        {
            switch (co.getPowerMode())
            {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM")
                    {
                        return 45
                    }
                    return 30;
                case GameEnums.PowerMode_Power:
                    if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM")
                    {
                        // apply sandstorm buff :)
                        return 25;
                    }
                    return 10;
                default:
                    break;
            }
            if (map.getGameRules().getCurrentWeather() !== null &&
                map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM")
            {
                // apply sandstorm buff :)
                return 15;
            }
        }
    }
    return 0;
};
CO_PENNY.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender)
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
CO_PENNY.getFirerangeModifier = function(co, unit, posX, posY)
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
                return 3;
            }
            return 1;
        }
    }
    return 0;
};
CO_PENNY.getMovementcostModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        if (unit.getOwner() === co.getOwner())
        {
            if (map.getGameRules().getCurrentWeather() !== null &&
                map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SNOW")
            {
                // apply snow buff :)
                return -1;
            }
        }
    }
    return 0;
};

CO_PENNY.getMovementpointModifier = function(co, unit, posX, posY)
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

CO_PENNY.getVisionrangeModifier = function(co, unit, posX, posY)
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
            return 1;
        }
    }
    return 0;
};

CO_PENNY.getPerfectVision = function(co)
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
