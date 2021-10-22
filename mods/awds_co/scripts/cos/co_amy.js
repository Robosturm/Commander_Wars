CO_AMY.powerHoverCraftBoost = 40;
CO_AMY.hoverCraftBoost = 20;
CO_AMY.superPowerDeffensiveBonus = 100;
CO_AMY.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                    defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (attacker.getMovementType() === "MOVE_HOVERCRAFT")
            {
                return CO_AMY.powerHoverCraftBoost;
            }
            return 10;
        case GameEnums.PowerMode_Power:
            if (attacker.getMovementType() === "MOVE_HOVERCRAFT")
            {
                return CO_AMY.powerHoverCraftBoost;
            }
            return 10;
        default:
            if (attacker.getMovementType() === "MOVE_HOVERCRAFT")
            {
                return CO_AMY.hoverCraftBoost;
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
            if (isAttacker === true)
            {
                return 99999;
            }
            return CO_AMY.superPowerDeffensiveBonus;
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
