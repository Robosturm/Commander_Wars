CO_DRAKE.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                  defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (attacker.getUnitType() === GameEnums.UnitType_Naval)
            {
                return 20;
            }
            else if (defender.getUnitType() === GameEnums.UnitType_Air)
            {
                return -5;
            }
            return 10;
        case GameEnums.PowerMode_Power:
            if (attacker.getUnitType() === GameEnums.UnitType_Naval)
            {
                return 20;
            }
            else if (attacker.getUnitType() === GameEnums.UnitType_Air)
            {
                return -5;
            }
            return 10;
        default:
            if (attacker.getUnitType() === GameEnums.UnitType_Naval)
            {
                return 20;
            }
            break;
        }
        if (attacker.getUnitType() === GameEnums.UnitType_Air)
        {
            return -15;
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
