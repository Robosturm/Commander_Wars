CO_KINDLE.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        if (typeof map !== 'undefined')
        {
            if (map.onMap(atkPosX, atkPosY))
            {
                var building = map.getTerrain(atkPosX, atkPosY).getBuilding();
                switch (co.getPowerMode())
                {
                case GameEnums.PowerMode_Tagpower:
                case GameEnums.PowerMode_Superpower:
                    var ret = 0;
                    if (building !== null)
                    {
                        ret = 130;
                    }
                    ret += co.getOwner().getBuildingCount() * 3 + 10;
                    return ret;
                case GameEnums.PowerMode_Power:
                    if (building !== null)
                    {
                        return 80;
                    }
                    return 10;
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
                                       defender, defPosX, defPosY, isDefender, action)
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
