CO_SENSEI.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        var seaUnitIDs = CO_SENSEI.getSeaUnitIDS();
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
            break;
        case GameEnums.PowerMode_Power:
            if (unitInfantryIDs.indexOf(attacker.getUnitID()) >= 0)
            {
                return 30;
            }
            else if (attacker.getUnitID() === "K_HELI")
            {
                return 80;
            }
            break;
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
        if (seaUnitIDs.indexOf(attacker.getUnitID()) >= 0)
        {
            return -10;
        }
    }
    return 0;
};

CO_SENSEI.getMovementpointModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        var unitTransportIDs = ["APC", "LANDER", "T_HELI", "TRANSPORTPLANE", "BLACK_BOOT"];
        if (unitTransportIDs.indexOf(unit.getUnitID()) >= 0)
        {
            return 1;
        }
    }
    return 0;
};
