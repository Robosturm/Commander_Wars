CO_KINDLE.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_KINDLE.activateSuperpower = function(co, powerMode, map)
{
	CO_KINDLE.activatePower(co, map);
};
CO_KINDLE.getSuperPowerDescription = function()
{
    return CO_KINDLE.getPowerDescription();
};
CO_KINDLE.getSuperPowerName = function()
{
    return CO_KINDLE.getPowerName();
};
CO_KINDLE.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    if (map !== null)
    {
        if (map.onMap(atkPosX, atkPosY))
        {
            var building = map.getTerrain(atkPosX, atkPosY).getBuilding();
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                if (building !== null)
                {
                    return 80;
                }
                return 10;
            default:
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    if (building !== null)
                    {
                        return 70;
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
