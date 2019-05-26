CO_WAYLON.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        var airUnits = CO_WAYLON.getAirUnitIDS();
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (airUnits.indexOf(attacker.getUnitID()) >= 0)
            {
                return 60;
            }
            break;
        case GameEnums.PowerMode_Power:
            if (airUnits.indexOf(attacker.getUnitID()) >= 0)
            {
                return 20;
            }
            else
            {
                return 0;
            }
        default:
            if (airUnits.indexOf(attacker.getUnitID()) >= 0)
            {
                return 5;
            }
            break;
        }
    }
    return 0;
};

CO_WAYLON.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                        defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        var airUnits = CO_WAYLON.getAirUnitIDS();
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (airUnits.indexOf(defender.getUnitID()) >= 0)
            {
                return 270;
            }
            break;
        case GameEnums.PowerMode_Power:
            if (airUnits.indexOf(defender.getUnitID()) >= 0)
            {
                return 200;
            }
            else
            {
                return 0;
            }
        default:
            if (airUnits.indexOf(defender.getUnitID()) >= 0)
            {
                return 30;
            }
            break;
        }
    }
    return 0;
};
