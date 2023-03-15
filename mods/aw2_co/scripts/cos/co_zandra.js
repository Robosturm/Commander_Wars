CO_ZANDRA.sandstormBonus = 30;
CO_ZANDRA.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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
            case GameEnums.PowerMode_Power:
                if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM")
                {
                    // apply sandstorm buff :)
                    return CO_ZANDRA.sandstormBonus;
                }
                else
                {
                    return 0;
                }
            default:
                if (map.getGameRules().getCurrentWeather() !== null &&
                        map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SANDSTORM")
                {
                    // apply sandstorm buff :)
                    return CO_ZANDRA.sandstormBonus;
                }
                return 0;
            }
        }
    }
    return 0;
};
CO_ZANDRA.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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
CO_ZANDRA.getFirerangeModifier = function(co, unit, posX, posY, map)
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
                    return CO_ZANDRA.sandstormRangeBonus;
                default:
                    return 0;
                }
            }
        }
    }
    return 0;
};
CO_ZANDRA.getWeatherImmune = function(co, map)
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
