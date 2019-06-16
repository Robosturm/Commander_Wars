CO_EAGLE.getFuelCostModifier = function(co, unit, costs)
{
    if (co.getIsCO0() === true)
    {
        var airUnits = CO_EAGLE.getAirUnitIDS();
        if (airUnits.indexOf(unit.getUnitID()) >= 0)
        {
            return -2;
        }
    }
    return 0;
};

CO_EAGLE.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        var airUnits = CO_EAGLE.getAirUnitIDS();
        var seaUnits = CO_EAGLE.getSeaUnitIDS();

        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Superpower:
            if (airUnits.indexOf(attacker.getUnitID()) >= 0)
            {
                return 30;
            }
            else if (seaUnits.indexOf(attacker.getUnitID()) >= 0)
            {
                return 0;
            }
            return 10;
        case GameEnums.PowerMode_Power:
            if (airUnits.indexOf(attacker.getUnitID()) >= 0)
            {
                return -30;
            }
            else if (seaUnits.indexOf(attacker.getUnitID()) >= 0)
            {
                return -40;
            }
            else
            {
                return -45;
            }
        default:
            if (airUnits.indexOf(attacker.getUnitID()) >= 0)
            {
                    return 20;
            }
            break;
        }
        if (seaUnits.indexOf(attacker.getUnitID()) >= 0)
        {
            return -10;
        }
    }
    return 0;
};

CO_EAGLE.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender)
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
