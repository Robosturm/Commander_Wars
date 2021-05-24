CO_JESS.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                     defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if ((attacker.getUnitType() !== GameEnums.UnitType_Air) &&
                (attacker.getUnitType() !== GameEnums.UnitType_Naval) &&
                (attacker.getUnitType() !== GameEnums.UnitType_Infantry))
            {
                return 70;
            }
            else if ((attacker.getUnitType() === GameEnums.UnitType_Air) ||
                     (attacker.getUnitType() === GameEnums.UnitType_Naval))
            {
                return 0;
            }
            return 10;
        case GameEnums.PowerMode_Power:
            if ((attacker.getUnitType() !== GameEnums.UnitType_Air) &&
                (attacker.getUnitType() !== GameEnums.UnitType_Naval) &&
                (attacker.getUnitType() !== GameEnums.UnitType_Infantry))
            {
                return 50;
            }
            else if ((attacker.getUnitType() === GameEnums.UnitType_Air) ||
                     (attacker.getUnitType() === GameEnums.UnitType_Naval))
            {
                return 0;
            }
            return 10;
        default:
            if ((attacker.getUnitType() !== GameEnums.UnitType_Air) &&
                (attacker.getUnitType() !== GameEnums.UnitType_Naval) &&
                (attacker.getUnitType() !== GameEnums.UnitType_Infantry))
            {
                return 20;
            }
            break;
        }
        if ((attacker.getUnitType() === GameEnums.UnitType_Air) ||
            (attacker.getUnitType() === GameEnums.UnitType_Naval))
        {
            return -10;
        }
    }
    return 0;
};

CO_JESS.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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

CO_JESS.getMovementpointModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Power)
        {
            if ((unit.getUnitType() !== GameEnums.UnitType_Air) &&
                (unit.getUnitType() !== GameEnums.UnitType_Naval) &&
                (unit.getUnitType() !== GameEnums.UnitType_Infantry))
            {
                return 1;
            }
        }
        else if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                 co.getPowerMode() === GameEnums.PowerMode_Tagpower)
        {
            if ((unit.getUnitType() !== GameEnums.UnitType_Air) &&
                (unit.getUnitType() !== GameEnums.UnitType_Naval) &&
                (unit.getUnitType() !== GameEnums.UnitType_Infantry))
            {
                return 2;
            }
        }
    }
    return 0;
};
