CO_AMY.powerHoverCraftBoost = 30;
CO_AMY.hoverCraftBoost = 20;
CO_AMY.superPowerDeffensiveBonus = 110;
CO_AMY.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                    defender, defPosX, defPosY, isDefender, action, luckmode, map)
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
                return CO_AMY.powerHoverCraftBoost;
            }
            return 0;
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
                                     defender, defPosX, defPosY, isAttacker, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return CO_AMY.superPowerDeffensiveBonus;
        case GameEnums.PowerMode_Power:
            return 10;
        default:
            break;
        }
    }
    return 0;
};

CO_AMY.getDamageReduction = function(co, damage, attacker, atkPosX, atkPosY, attackerBaseHp,
                                   defender, defPosX, defPosY, isDefender, luckMode, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (isDefender === true)
            {
                return damage;
            }
        }
    }
    return 0;
};

CO_AMY.getMovementpointModifier = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        if (unit.getMovementType() === "MOVE_HOVERCRAFT")
        {
            if (co.getPowerMode() === GameEnums.PowerMode_Power)
            {

                return 2;
            }
            else
            {
                return 1;
            }
        }
    }
    return 0;
};
CO_AMY.getCostModifier = function(co, id, baseCost, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Power)
        {
            if (Global[id].getMovementType() === "MOVE_HOVERCRAFT")
            {
                return -baseCost * CO_AMY.powerCostReduction;
            }
        }
    }
    return 0;
};

CO_AMY.getMovementcostModifier = function(co, unit, posX, posY, map)
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
