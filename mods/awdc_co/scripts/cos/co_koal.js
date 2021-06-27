CO_KOAL.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_KOAL.activateSuperpower = function(co, powerMode)
{
	CO_KOAL.activatePower(co, powerMode);
};
CO_KOAL.getSuperPowerDescription = function()
{
    return CO_KOAL.getPowerDescription();
};
CO_KOAL.getSuperPowerName = function()
{
    return CO_KOAL.getPowerName();
};
CO_KOAL.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action)
{
    if (typeof map !== 'undefined')
    {
        if (map.onMap(atkPosX, atkPosY))
        {
            var terrainID = map.getTerrain(atkPosX, atkPosY).getID();
            var isStreet = (terrainID === "STREET") ||
                           (terrainID === "BRIDGE") ||
                           (terrainID === "DESERT_PATH");
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                if (isStreet)
                {
                    return 50;
                }
                return 10;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (isStreet)
                    {
                        return 50;
                    }
                    return 10;
                }
                break;
            }
        }
    }
    else if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
    {
        return 10;
    }
    return 0;
};

CO_KOAL.getMovementpointModifier = function(co, unit, posX, posY)
{
    if (co.getPowerMode() > GameEnums.PowerMode_Off)
    {
        return 1;
    }
    return 0;
};
