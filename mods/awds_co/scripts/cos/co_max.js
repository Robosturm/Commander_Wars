CO_MAX.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                    defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (attacker.getBaseMaxRange() === 1 &&
                attacker.getUnitType() !== GameEnums.UnitType_Infantry)
            {
                return 70;
            }
            return 10;
        case GameEnums.PowerMode_Power:
            if (attacker.getBaseMaxRange() === 1 &&
                attacker.getUnitType() !== GameEnums.UnitType_Infantry)
            {
                return 50;
            }
            return 10;
        default:
            if (attacker.getBaseMaxRange() === 1 &&
                attacker.getUnitType() !== GameEnums.UnitType_Infantry)
            {
                return 20;
            }
            break;
        }
    }
    return 0;
};

CO_MAX.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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

CO_MAX.getFirerangeModifier = function(co, unit, posX, posY, map)
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

CO_MAX.getMovementpointModifier = function(co, unit, posX, posY, map)
{
    return 0;
};
