CO_AIRA.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_AIRA.activateSuperpower = function(co, powerMode, map)
{
    CO_AIRA.activatePower(co, powerMode, map);
};
CO_AIRA.getSuperPowerDescription = function()
{
    return CO_AIRA.getPowerDescription();
};
CO_AIRA.getSuperPowerName = function()
{
    return CO_AIRA.getPowerName();
};
CO_AIRA.getMovementFuelCostModifier = function(co, unit, fuelCost, map)
{
    if (co.getPowerMode() > GameEnums.PowerMode_Off &&
        co.getOwner().isEnemyUnit(unit) === true)
    {
        return fuelCost * 5;
    }
    return 0;
};
CO_AIRA.coZoneBonus = 0;
CO_AIRA.getMovementcostModifier = function(co, unit, posX, posY, map)
{
    return 0;
};

CO_AIRA.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    return 0;
};

CO_AIRA.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
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
