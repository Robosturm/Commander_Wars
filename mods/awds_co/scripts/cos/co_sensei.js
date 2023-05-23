
CO_SENSEI.superPowerInfOffBonus = 20;
CO_SENSEI.superPowerHeliOffBonus = 90;
CO_SENSEI.powerGroundBonus = 10;
CO_SENSEI.powerInfOffBonus = 20;
CO_SENSEI.powerHeliOffBonus = 90;
CO_SENSEI.powerNavalOffBonus = 0;
CO_SENSEI.powerOffBonus = 10;
CO_SENSEI.powerDefBonus = 10;
CO_SENSEI.powerSpawnHp = 9;
CO_SENSEI.d2dCoZoneGroundBonus = 0;
CO_SENSEI.d2dCoZoneInfOffBonus = 0;
CO_SENSEI.d2dCoZoneHeliOffBonus = 0;
CO_SENSEI.d2dCoZoneNavalOffBonus = 0;
CO_SENSEI.d2dCoZoneOffBonus = 0;
CO_SENSEI.d2dCoZoneDefBonus = 0;
CO_SENSEI.d2dInfOffBonus = 0;
CO_SENSEI.d2dHeliOffBonus = 30;
CO_SENSEI.d2dNavalOffBonus = -10;
CO_SENSEI.d2dGroundBonus = 0;
CO_SENSEI.d2dTransporterMovementBonus =1;

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
                return 20;
            }
            else if (attacker.getUnitID() === "K_HELI")
            {
                return 90;
            }
            else if (attacker.getUnitType() === GameEnums.UnitType_Naval)
            {
                return 0;
            }
            return 10;
        case GameEnums.PowerMode_Power:
            if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
            {
                return 20;
            }
            else if (attacker.getUnitID() === "K_HELI")
            {
                return 90;
            }
            else if (attacker.getUnitType() === GameEnums.UnitType_Naval)
            {
                return 0;
            }
            return 10;
        default:
            if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
            {
                return 10;
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
