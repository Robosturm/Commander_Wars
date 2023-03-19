CO_EAGLE.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_EAGLE.activateSuperpower = function(co, powerMode, map)
{
	CO_EAGLE.activatePower(co, map);
};
CO_EAGLE.getSuperPowerDescription = function()
{
    return CO_EAGLE.getPowerDescription();
};
CO_EAGLE.getSuperPowerName = function()
{
    return CO_EAGLE.getPowerName();
};
CO_EAGLE.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (attacker.getUnitType() === GameEnums.UnitType_Air)
            {
                return -30;
            }
            else if (attacker.getUnitType() === GameEnums.UnitType_Naval)
            {
                return -40;
            }
            else
            {
                return -45;
            }
        default:
            if (attacker.getUnitType() === GameEnums.UnitType_Air)
            {
                if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                {
                    return 30;
                }
                return 10;
            }
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                if (attacker.getUnitType() === GameEnums.UnitType_Naval)
                {
                    return 0;
                }
                return 10;
            }
            break;
    }
    if (attacker.getUnitType() === GameEnums.UnitType_Naval)
    {
        return -10;
    }
    return 0;
};
