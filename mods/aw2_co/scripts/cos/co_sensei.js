CO_SENSEI.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
            {
                return 40;
            }
            else if (attacker.getUnitID() === "K_HELI")
            {
                return 80;
            }
            else if (attacker.getUnitType() === GameEnums.UnitType_Naval ||
                     attacker.getUnitType() === GameEnums.UnitType_Ground ||
                     attacker.getUnitType() === GameEnums.UnitType_Hovercraft)
            {
                return -10;
            }
            return 0;
        case GameEnums.PowerMode_Power:
            if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
            {
                return 40;
            }
            else if (attacker.getUnitID() === "K_HELI")
            {
                return 80;
            }
            else if (attacker.getUnitType() === GameEnums.UnitType_Naval ||
                     attacker.getUnitType() === GameEnums.UnitType_Ground ||
                     attacker.getUnitType() === GameEnums.UnitType_Hovercraft)
            {
                return -10;
            }
            return 10;
        default:
            if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
            {
                return 40;
            }
            else if (attacker.getUnitID() === "K_HELI")
            {
                return 50;
            }
            break;
        }
        if (attacker.getUnitType() === GameEnums.UnitType_Naval ||
            attacker.getUnitType() === GameEnums.UnitType_Ground ||
            attacker.getUnitType() === GameEnums.UnitType_Hovercraft)
        {
            return -10;
        }
    }
    return 0;
};
CO_SENSEI.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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
CO_SENSEI.getMovementpointModifier = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        if (unit.isTransporter())
        {
            return 1;
        }
    }
    return 0;
};
