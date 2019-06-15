CO_SAMI.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                     defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        var unitInfantryIDs = ["INFANTRY", "MECH", "SNIPER", "MOTORBIKE"];
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (unitInfantryIDs.indexOf(attacker.getUnitID()) >= 0)
            {
                return 80;
            }
            break;
        case GameEnums.PowerMode_Power:
            if (unitInfantryIDs.indexOf(attacker.getUnitID()) >= 0)
            {
                return 50;
            }
            break;
        default:
            if (unitInfantryIDs.indexOf(attacker.getUnitID()) >= 0)
            {
                return 20;
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
CO_SAMI.getCaptureBonus = function(co, unit, posX, posY)
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

CO_SAMI.getMovementpointModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        var unitTransportIDs = ["APC", "LANDER", "T_HELI", "TRANSPORTPLANE", "BLACK_BOOT"];
        if (unitTransportIDs.indexOf(unit.getUnitID()) >= 0)
        {
            return 1;
        }
        var unitInfantryIDs = ["INFANTRY", "MECH", "SNIPER", "MOTORBIKE"];
        if (unitInfantryIDs.indexOf(unit.getUnitID()) >= 0)
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
