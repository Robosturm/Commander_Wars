CO_TASHA.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        var airUnits = CO_TASHA.getAirUnitIDS();
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (airUnits.indexOf(attacker.getUnitID()) >= 0)
            {
                return 70;
            }
            break;
        case GameEnums.PowerMode_Power:
            if (airUnits.indexOf(attacker.getUnitID()) >= 0)
            {
                return 50;
            }
            else
            {
                return 10;
            }
        default:
            if (airUnits.indexOf(attacker.getUnitID()) >= 0)
            {
                return 15;
            }
        }
    }
    return 0;
};

CO_TASHA.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        var airUnits = CO_TASHA.getAirUnitIDS();
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Superpower:
            if (airUnits.indexOf(defender.getUnitID()) >= 0)
            {
                return 30;
            }
            return 10;
        case GameEnums.PowerMode_Power:
            if (airUnits.indexOf(defender.getUnitID()) >= 0)
            {
                return 30;
            }
            return 10;
        default:
            break;
        }
    }
    return 0;
};

CO_TASHA.getMovementpointModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
            co.getPowerMode() === GameEnums.PowerMode_Tagpower)
        {
            return 2;
        }
        else if (co.getPowerMode() === GameEnums.PowerMode_Power)
        {
            return 1;
        }
    }
    return 0;
};
