CO_ZANDRA.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        if (map !== null)
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM")
                {
                    // apply sandstorm buff :)
                    return 40;
                }
                else
                {
                    return 10;
                }
            default:
                if (map.getGameRules().getCurrentWeather() !== null &&
                        map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM")
                {
                    // apply sandstorm buff :)
                    return 20;
                }
                return 0;
            }
        }
    }
    return 0;
};
CO_ZANDRA.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                        defender, defPosX, defPosY, isAttacker, action)
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
        if (map.getGameRules().getCurrentWeather() !== null &&
                map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM")
        {
            if (unit.getBaseMaxRange() > 1)
            {
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                case GameEnums.PowerMode_Power:
                    return 1;
                default:
                    return 0;
                }
            }
        }
    }
    return 0;
};
CO_ZANDRA.getWeatherImmune = function(co)
{
    if (co.getIsCO0() === true)
    {
        if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM")
        {
            return true;
        }
    }
    return false;
};
