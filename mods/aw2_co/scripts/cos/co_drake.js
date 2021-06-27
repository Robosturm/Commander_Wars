CO_DRAKE.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        if (attacker.getUnitType() === GameEnums.UnitType_Air)
        {
            return -30;
        }
    }
    return 0;
};

CO_DRAKE.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            if (defender.getUnitType() === GameEnums.UnitType_Naval)
            {
                return 20;
            }
            return 10;
        }
        else if (defender.getUnitType() === GameEnums.UnitType_Naval)
        {
            return 10;
        }
    }
    return 0;
};

CO_DRAKE.getMovementpointModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        if (unit.getUnitType() === GameEnums.UnitType_Naval)
        {
            return 1;
        }
    }
};
