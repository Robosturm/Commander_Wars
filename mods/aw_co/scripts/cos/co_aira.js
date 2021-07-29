CO_AIRA.init = function(co)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_AIRA.activateSuperpower = function(co, powerMode)
{
    CO_AIRA.activatePower(co, powerMode);
};
CO_AIRA.getSuperPowerDescription = function()
{
    return CO_AIRA.getPowerDescription();
};
CO_AIRA.getSuperPowerName = function()
{
    return CO_AIRA.getPowerName();
};
CO_AIRA.getMovementFuelCostModifier = function(co, unit, fuelCost)
{
    if (co.getPowerMode() > GameEnums.PowerMode_Off &&
        co.getOwner().isEnemyUnit(unit) === true)
    {
        return fuelCost * 5;
    }
    return 0;
};
CO_AIRA.coZoneBonus = 0;
CO_AIRA.getMovementcostModifier = function(co, unit, posX, posY)
{
    return 0;
};

CO_AIRA.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action)
{
    return 0;
};

CO_AIRA.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action)
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
