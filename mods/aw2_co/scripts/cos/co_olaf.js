
this.superPowerDamage = 2;
this.superPowerOffBonus = 60;
this.powerOffBonus = 10;
this.powerDefBonus = 10;
this.d2dOffBonus = 60;
this.d2dRainMalus = true;
this.d2dCoZoneOffBonus = 60;
this.d2dCoZoneBaseOffBonus = 10;
this.d2dCoZoneDefBonus = 10;

CO_OLAF.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                     defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    return 0;
};

CO_OLAF.getWeatherImmune = function(co, map)
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
