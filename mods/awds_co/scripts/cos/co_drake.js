CO_DRAKE.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                  defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        var airUnits = CO_DRAKE.getAirUnitIDS();
        var seaUnits = CO_DRAKE.getSeaUnitIDS();
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (seaUnits.indexOf(attacker.getUnitID()) >= 0)
            {
                return 20;
            }
            break;
        case GameEnums.PowerMode_Power:
            if (seaUnits.indexOf(attacker.getUnitID()) >= 0)
            {
                return 20;
            }
            break;
        default:
            if (seaUnits.indexOf(attacker.getUnitID()) >= 0)
            {
                return 20;
            }
            break;
        }
        if (airUnits.indexOf(attacker.getUnitID()) >= 0)
        {
            return -15;
        }
    }
    return 0;
};
CO_DRAKE.getMovementpointModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        var seaUnits = CO_DRAKE.getSeaUnitIDS();
        if (seaUnits.indexOf(unit.getUnitID()) >= 0)
        {
            return 1;
        }
    }
};
