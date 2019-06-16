CO_ZANDRA.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM")
            {
                // apply sandstorm buff :)
                return 55;
            }
            else
            {
                return 10;
            }
        case GameEnums.PowerMode_Power:
            if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM")
            {
                // apply sandstorm buff :)
                return 55;
            }
            else
            {
                return 10;
            }
        default:
            if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM")
            {
                // apply sandstorm buff :)
                return 35;
            }
            return 0;
        }
    }
    return 0;
};
CO_ZANDRA.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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
CO_ZANDRA.getFirerangeModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM")
        {
            if (unit.getBaseMaxRange() > 1)
            {
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    return 2;
                case GameEnums.PowerMode_Power:
                    return 2;
                default:
                    return 1;
                }
            }
        }
    }
    return 0;
};
