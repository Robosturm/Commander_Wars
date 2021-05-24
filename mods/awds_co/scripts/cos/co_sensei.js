CO_SENSEI.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        var unitInfantryIDs = CO_SENSEI.getInfantryIDS();
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (unitInfantryIDs.indexOf(attacker.getUnitID()) >= 0)
            {
                return 30;
            }
            else if (attacker.getUnitID() === "K_HELI")
            {
                return 80;
            }
            else if (attacker.getUnitType() === GameEnums.UnitType_Naval)
            {
                return 0;
            }
            return 10;
        case GameEnums.PowerMode_Power:
            if (unitInfantryIDs.indexOf(attacker.getUnitID()) >= 0)
            {
                return 30;
            }
            else if (attacker.getUnitID() === "K_HELI")
            {
                return 80;
            }
            else if (attacker.getUnitType() === GameEnums.UnitType_Naval)
            {
                return 0;
            }
            return 10;
        default:
            if (unitInfantryIDs.indexOf(attacker.getUnitID()) >= 0)
            {
                return 20;
            }
            else if (attacker.getUnitID() === "K_HELI")
            {
                return 50;
            }
            break;
        }
        if (attacker.getUnitType() === GameEnums.UnitType_Naval)
        {
            return -10;
        }
    }
    return 0;
};
CO_SENSEI.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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
CO_SENSEI.getMovementpointModifier = function(co, unit, posX, posY)
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
