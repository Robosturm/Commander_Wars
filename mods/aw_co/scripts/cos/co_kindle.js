CO_KINDLE.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_KINDLE.activateSuperpower = function(co, powerMode, map)
{
    CO_KINDLE.activatePower(co, powerMode);
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
                                       defender, defPosX, defPosY, isDefender, action, map)
{
    if (co.getIsCO0() === true)
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
                        return 70;
                    }
                    return 0;
                default:
                    if (building !== null)
                    {
                        return 40;
                    }
                    break;
                }
            }
        }
    }
    return 0;
};

CO_KINDLE.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, map)
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
