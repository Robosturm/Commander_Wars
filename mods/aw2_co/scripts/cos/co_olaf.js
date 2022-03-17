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
CO_OLAF.getMovementcostModifier = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        if (map !== null)
        {
            if (map.getGameRules().getCurrentWeather().getWeatherId() === "WEATHER_RAIN")
            {
                var terrain = map.getTerrain(posX, posY);
                var id = terrain.getID();
                if ((unit.getUnitType() === GameEnums.UnitType_Air) ||
                    (id !== "STREET" &&
                     id !== "STREET1" &&
                     id !== "SNOW_STREET" &&
                     id !== "BRIDGE" &&
                     id !== "BRIDGE1" &&
                     id !== "BRIDGE2" &&
                     id !== "WASTE_PATH" &&
                     id !== "DESERT_PATH" &&
                     id !== "DESERT_PATH1" &&
                     id !== "TELEPORTTILE" &&
                    terrain.getBuilding() === null))
                {
                    return 1;
                }
            }
        }
    }
    return 0;
};
