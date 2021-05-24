CO_HACHI.getCostModifier = function(co, id, baseCost)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return -baseCost * 0.5;
        default:
            break;
        }
        return -baseCost * 0.1;
    }
    return 0;
};

CO_HACHI.getAdditionalBuildingActions = function(co, building)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (building.getBuildingID() === "TOWN" &&
                    building.getOwner() === co.getOwner())
            {
                return "ACTION_BUILD_UNITS";
            }
            break;
        case GameEnums.PowerMode_Power:
            break;
        default:
            break;
        }
    }
    return "";
};

CO_HACHI.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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

CO_HACHI.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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
