CO_SAMI.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                     defender, defPosX, defPosY, isDefender, action, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
            {
                return 80;
            }
            else if (attacker.getBaseMaxRange() === 1)
            {
                return -10;
            }
            return 0;
        case GameEnums.PowerMode_Power:
            if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
            {
                return 50;
            }
            else if (attacker.getBaseMaxRange() === 1)
            {
                return -10;
            }
            return 0;
        default:
            if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
            {
                return 30;
            }
            break;
        }
        if (attacker.getBaseMaxRange() === 1)
        {
            return -10;
        }
    }
    return 0;
};
CO_SAMI.getCaptureBonus = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
            co.getPowerMode() === GameEnums.PowerMode_Tagpower)
        {
            return 20;
        }
        else
        {
            var hp = unit.getHpRounded();
            return hp / 2;
        }
    }
    return 0;
};
CO_SAMI.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, map)
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
CO_SAMI.getMovementpointModifier = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        if (unit.isTransporter())
        {
            return 1;
        }
        if (unit.getUnitType() === GameEnums.UnitType_Infantry)
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
                return 2;
            case GameEnums.PowerMode_Power:
                return 1;
            default:
                return 0;
            }
        }
    }
    return 0;
};
