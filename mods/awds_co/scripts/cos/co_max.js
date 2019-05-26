CO_MAX.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                    defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (attacker.getBaseMaxRange() === 1)
            {
                return 70;
            }
            break;
        case GameEnums.PowerMode_Power:
            if (attacker.getBaseMaxRange() === 1)
            {
                return 45;
            }
            break;
        default:
            if (attacker.getBaseMaxRange() === 1)
            {
                return 20;
            }
            break;
        }
        if (attacker.getBaseMaxRange() > 1)
        {
            return -10;
        }
    }
    return 0;
};
CO_MAX.getFirerangeModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        if (unit.getBaseMaxRange() > 1)
        {
            return -1;
        }
    }
    return 0;
};

CO_MAX.getMovementpointModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Power)
        {
            if (unit.getBaseMaxRange() === 1)
            {
                return 1;
            }
        }
        else if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                 co.getPowerMode() === GameEnums.PowerMode_Tagpower)
        {
            if (unit.getBaseMaxRange() === 1)
            {
                return 2;
            }
        }
    }
    return 0;
};
