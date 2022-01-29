CO_HACHI.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_HACHI.activateSuperpower = function(co, powerMode, map)
{
    CO_HACHI.activatePower(co, powerMode);
};
CO_HACHI.getSuperPowerDescription = function()
{
    return CO_HACHI.getPowerDescription();
};
CO_HACHI.getSuperPowerName = function()
{
    return CO_HACHI.getPowerName();
};
CO_HACHI.getCostModifier = function(co, id, baseCost, posX, posY, map)
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

CO_HACHI.getAdditionalBuildingActions = function(co, building, map)
{
    return "";
};

CO_HACHI.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, map)
{
    return 0;
};

CO_HACHI.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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
