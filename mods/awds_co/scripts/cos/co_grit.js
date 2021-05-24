CO_GRIT.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                     defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (attacker.getBaseMaxRange() > 1)
            {
                return 70;
            }
            else if (attacker.getUnitType() === GameEnums.UnitType_Ground)
            {
                return -10;
            }
            return 10;
        case GameEnums.PowerMode_Power:
            if (attacker.getBaseMaxRange() > 1)
            {
                return 50;
            }
            else if (attacker.getUnitType() === GameEnums.UnitType_Ground)
            {
                return -10;
            }
            return 10;
        default:
            if (attacker.getBaseMaxRange() > 1)
            {
                return 20;
            }
            break;
        }
        if (attacker.getBaseMaxRange() === 1 &&
            attacker.getUnitType() !== GameEnums.UnitType_Infantry)
        {
            return -20;
        }
    }
    return 0;
};
CO_GRIT.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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
CO_GRIT.getFirerangeModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (unit.getBaseMaxRange() > 1)
            {
                return 3;
            }
            break;
        case GameEnums.PowerMode_Power:
            if (unit.getBaseMaxRange() > 1)
            {
                return 2;
            }
            break;
        default:
            if (unit.getBaseMaxRange() > 1)
            {
                return 1;
            }
            break;
        }
    }
    return 0;
};
