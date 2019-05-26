CO_KINDLE.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
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
                ret += co.getOwner().getBuildingCount() * 3;
                return ret;
            case GameEnums.PowerMode_Power:
                if (building !== null)
                {
                    return 80;
                }
                break;
            default:
                if (building !== null)
                {
                    return 40;
                }
                break;
            }
        }
    }
    return 0;
};
