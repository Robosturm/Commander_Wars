CO_AIRA.getMovementFuelCostModifier = function(co, unit, fuelCost)
{
    if (co.getPowerMode() === GameEnums.PowerMode_Power &&
            co.getOwner().isEnemyUnit(unit) === true)
    {
        return fuelCost * 5;
    }
    return 0;
};
CO_AIRA.getMovementcostModifier = function(co, unit, posX, posY)
{
    if ((co.getPowerMode() === GameEnums.PowerMode_Superpower ||
         co.getPowerMode() === GameEnums.PowerMode_Tagpower) &&
            co.getOwner().isEnemyUnit(unit) === true)
    {
        return 1;
    }
    return 0;
};

CO_AIRA.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                     defender, defPosX, defPosY, isDefender)
{
    return 0;
};
