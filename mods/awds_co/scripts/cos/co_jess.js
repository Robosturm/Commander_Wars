CO_JESS.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                     defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        var airUnits = CO_JESS.getAirUnitIDS();
        var seaUnits = CO_JESS.getSeaUnitIDS();
        var infantryUnits = CO_JESS.getInfatryUnitIDS();
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if ((airUnits.indexOf(attacker.getUnitID()) < 0) &&
                    (seaUnits.indexOf(attacker.getUnitID()) < 0) &&
                    (infantryUnits.indexOf(attacker.getUnitID()) < 0))
            {
                return 70;
            }
            break;
        case GameEnums.PowerMode_Power:
            if ((airUnits.indexOf(attacker.getUnitID()) < 0) &&
                    (seaUnits.indexOf(attacker.getUnitID()) < 0) &&
                    (infantryUnits.indexOf(attacker.getUnitID()) < 0))
            {
                return 50;
            }
            break;
        default:
            if ((airUnits.indexOf(attacker.getUnitID()) < 0) &&
                    (seaUnits.indexOf(attacker.getUnitID()) < 0) &&
                    (infantryUnits.indexOf(attacker.getUnitID()) < 0))
            {
                return 20;
            }
            break;
        }
        if ((airUnits.indexOf(attacker.getUnitID()) >= 0) ||
                (seaUnits.indexOf(attacker.getUnitID()) >= 0))
        {
            return -10;
        }
    }
    return 0;
};

CO_JESS.getMovementpointModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        var airUnits = CO_JESS.getAirUnitIDS();
        var seaUnits = CO_JESS.getSeaUnitIDS();
        var infantryUnits = CO_JESS.getInfatryUnitIDS();
        if (co.getPowerMode() === GameEnums.PowerMode_Power)
        {
            if ((airUnits.indexOf(unit.getUnitID()) < 0) &&
                    (seaUnits.indexOf(unit.getUnitID()) < 0) &&
                    (infantryUnits.indexOf(unit.getUnitID()) < 0))
            {
                return 1;
            }
        }
        else if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
                 co.getPowerMode() === GameEnums.PowerMode_Tagpower)
        {
            if ((airUnits.indexOf(unit.getUnitID()) < 0) &&
                    (seaUnits.indexOf(unit.getUnitID()) < 0) &&
                    (infantryUnits.indexOf(unit.getUnitID()) < 0))
            {
                return 2;
            }
        }
    }
    return 0;
};
