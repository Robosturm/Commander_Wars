CO_GREYFIELD.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                          defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        var seaUnits = CO_GREYFIELD.getSeaUnitIDS();
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (seaUnits.indexOf(attacker.getUnitID()) >= 0)
            {
                return 60;
            }
            break;
        case GameEnums.PowerMode_Power:
            if (seaUnits.indexOf(attacker.getUnitID()) >= 0)
            {
                return 20;
            }
            else
            {
                return 0;
            }
        default:
            if (seaUnits.indexOf(attacker.getUnitID()) >= 0)
            {
                return 5;
            }
            break;
        }
    }
    return 0;
};

CO_GREYFIELD.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                           defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        var seaUnits = CO_GREYFIELD.getSeaUnitIDS();
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (seaUnits.indexOf(defender.getUnitID()) >= 0)
            {
                return 70;
            }
            break;
        case GameEnums.PowerMode_Power:
            if (seaUnits.indexOf(defender.getUnitID()) >= 0)
            {
                return 40;
            }
            else
            {
                return 0;
            }
        default:
            if (seaUnits.indexOf(defender.getUnitID()) >= 0)
            {
                return 25;
            }
            break;
        }
    }
    return 0;
};

CO_GREYFIELD.getMovementpointModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        var seaUnits = CO_GREYFIELD.getSeaUnitIDS();
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (seaUnits.indexOf(unit.getUnitID()) >= 0)
            {
                return 1;
            }
            return 0;
        case GameEnums.PowerMode_Power:
            return 0;
        default:
            break;
        }
    }
    return 0;
};
