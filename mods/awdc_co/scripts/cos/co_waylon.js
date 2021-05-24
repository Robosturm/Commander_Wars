CO_WAYLON.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_WAYLON.activateSuperpower = function(co, powerMode)
{
	CO_WAYLON.activatePower(co, powerMode);
};
CO_WAYLON.getSuperPowerDescription = function()
{
    return CO_WAYLON.getPowerDescription();
};
CO_WAYLON.getSuperPowerName = function()
{
    return CO_WAYLON.getPowerName();
};
CO_WAYLON.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender, action)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (attacker.getUnitType() === GameEnums.UnitType_Air)
            {
                return 30;
            }
            return 10;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                if (attacker.getUnitType() === GameEnums.UnitType_Air)
                {
                    return 30;
                }
                return 10;
            }
            break;
    }
    return 0;
};
CO_WAYLON.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
		case GameEnums.PowerMode_Power:
            if (defender.getUnitType() === GameEnums.UnitType_Air)
            {
                return 270;
            }
            return 10;
        default:
            if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                if (defender.getUnitType() === GameEnums.UnitType_Air)
                {
                    return 40;
                }
                return 10;
            }
            break;
    }
    return 0;
};
