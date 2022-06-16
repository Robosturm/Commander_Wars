CO_DRAKE.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_DRAKE.activateSuperpower = function(co, powerMode, map)
{
    CO_DRAKE.activatePower(co, powerMode, map);
};
CO_DRAKE.getSuperPowerDescription = function()
{
    return CO_DRAKE.getPowerDescription();
};
CO_DRAKE.getSuperPowerName = function()
{
    return CO_DRAKE.getPowerName();
};
CO_DRAKE.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        if (attacker.getUnitType() === GameEnums.UnitType_Air)
        {
            return -20;
        }
    }
    return 0;
};

CO_DRAKE.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            if (defender.getUnitType() === GameEnums.UnitType_Naval)
            {
                return 30;
            }
            return 10;
        }
        else if (defender.getUnitType() === GameEnums.UnitType_Naval)
        {
            return 20;
        }
    }
    return 0;
};

CO_DRAKE.getMovementpointModifier = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        if (unit.getUnitType() === GameEnums.UnitType_Naval)
        {
            return 1;
        }
    }
    return 0;
};
