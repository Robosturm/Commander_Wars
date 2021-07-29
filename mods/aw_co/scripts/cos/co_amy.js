CO_AMY.init = function(co)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_AMY.activateSuperpower = function(co, powerMode)
{
    CO_AMY.activatePower(co, powerMode);
};
CO_AMY.getSuperPowerDescription = function()
{
    return CO_AMY.getPowerDescription();
};
CO_AMY.getSuperPowerName = function()
{
    return CO_AMY.getPowerName();
};
CO_AMY.hoverCraftBoost = 0;
CO_AMY.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                    defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (attacker.getMovementType() === "MOVE_HOVERCRAFT")
            {
                return 30;
            }
            return 0;
        default:
            if (attacker.getMovementType() === "MOVE_HOVERCRAFT")
            {
                return 20;
            }
            break;
        }
    }
    return 0;
};
CO_AMY.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                     defender, defPosX, defPosY, isAttacker, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 10;
        default:
            break;
        }
    }
    return 0;
};
CO_AMY.getMovementpointModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Power)
        {
            if (unit.getMovementType() === "MOVE_HOVERCRAFT")
            {
                return 1;
            }
        }
    }
    return 0;
};
CO_AMY.getCostModifier = function(co, id, baseCost)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Power)
        {
            if (Global[id].getMovementType() === "MOVE_HOVERCRAFT")
            {
                return -baseCost * 0.3;
            }
        }
    }
    return 0;
};

CO_AMY.getMovementcostModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        if (map.getTerrain(posX, posY).getTerrainID() === "REAF")
        {
            return -999;
        }
    }
    return 0;
};
