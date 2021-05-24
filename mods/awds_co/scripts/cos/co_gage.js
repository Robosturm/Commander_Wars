CO_GAGE.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                     defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (attacker.getBaseMaxRange() > 1 ||
                attacker.getUnitType() === GameEnums.UnitType_Naval)
            {
                return 50;
            }
            break;
        case GameEnums.PowerMode_Power:
            if (attacker.getBaseMaxRange() > 1 ||
                attacker.getUnitType() === GameEnums.UnitType_Naval)
            {
                return 30;
            }
            break;
        default:
            if (attacker.getBaseMaxRange() > 1 ||
                attacker.getUnitType() === GameEnums.UnitType_Naval)
            {
                return 10;
            }
            break;
        }
    }
    return 0;
};
CO_GAGE.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (defender.getBaseMaxRange() > 1 ||
                defender.getUnitType() === GameEnums.UnitType_Naval)
            {
                return 30;
            }
            break;
        case GameEnums.PowerMode_Power:
            if (defender.getBaseMaxRange() > 1 ||
                defender.getUnitType() === GameEnums.UnitType_Naval)
            {
                return 20;
            }
            break;
        default:
            if (defender.getBaseMaxRange() > 1 ||
                defender.getUnitType() === GameEnums.UnitType_Naval)
            {
                return 5;
            }
            break;
        }
    }
    return 0;
};

CO_GAGE.getFirerangeModifier = function(co, unit, posX, posY)
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
            break;
        }
    }
    return 0;
};
