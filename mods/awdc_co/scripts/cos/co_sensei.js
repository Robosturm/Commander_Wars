CO_SENSEI.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_SENSEI.activateSuperpower = function(co, powerMode, map)
{
	CO_SENSEI.activatePower(co, powerMode);
};
CO_SENSEI.getSuperPowerDescription = function()
{
    return CO_SENSEI.getPowerDescription();
};
CO_SENSEI.getSuperPowerName = function()
{
    return CO_SENSEI.getPowerName();
};
CO_SENSEI.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action, map)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
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
            return 10
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
                {
                    return 40;
                }
                else if (attacker.getUnitID() === "K_HELI")
                {
                    return 50;
                }
                else if (attacker.getUnitType() === GameEnums.UnitType_Naval)
                {
                    return 0;
                }
                return 10;
            }
            else if (attacker.getUnitID() === "K_HELI")
            {
                return 30;
            }
            break;
    }
    if (attacker.getUnitType() === GameEnums.UnitType_Naval)
    {
        return -10;
    }
    return 0;
};
