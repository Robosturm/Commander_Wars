CO_OLAF.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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
                    if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SNOW")
                    {
                        // apply snow buff :)
                        return 50;
                    }
                    else
                    {
                        return 10;
                    }
                case GameEnums.PowerMode_Power:
                    if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SNOW")
                    {
                        // apply snow buff :)
                        return 50;
                    }
                    else
                    {
                        return 10;
                    }
                default:
                    if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SNOW")
                    {
                        // apply snow buff :)
                        return 20;
                    }
                    return 0;
            }
        }
    }
    return 0;
};
CO_OLAF.getMovementcostModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        if (unit.getOwner() === co.getOwner())
        {
            if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_SNOW")
            {
                // apply snow buff :)
                return -1;
            }
        }
    }
    return 0;
};

CO_OLAF.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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
