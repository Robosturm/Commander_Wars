CO_JESS.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_JESS.activateSuperpower = function(co, powerMode, map)
{
	CO_JESS.activatePower(co, powerMode);
};
CO_JESS.getSuperPowerDescription = function()
{
    return CO_JESS.getPowerDescription();
};
CO_JESS.getSuperPowerName = function()
{
    return CO_JESS.getPowerName();
};
CO_JESS.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action, map)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if ((attacker.getUnitType() !== GameEnums.UnitType_Air) &&
                (attacker.getUnitType() !== GameEnums.UnitType_Naval) &&
                (attacker.getUnitType() !== GameEnums.UnitType_Infantry))
            {
                return 50;
            }
            else if ((attacker.getUnitType() === GameEnums.UnitType_Air) ||
                     (attacker.getUnitType() === GameEnums.UnitType_Naval))
            {
                return 0;
            }
            return 10;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                if ((attacker.getUnitType() !== GameEnums.UnitType_Air) &&
                    (attacker.getUnitType() !== GameEnums.UnitType_Naval) &&
                    (attacker.getUnitType() !== GameEnums.UnitType_Infantry))
                {

                    return 40;
                }
                else if ((attacker.getUnitType() === GameEnums.UnitType_Air) ||
                         (attacker.getUnitType() === GameEnums.UnitType_Naval))
                {
                    return 0;
                }
                return 10;
            }
            break;
    }
    if ((attacker.getUnitType() !== GameEnums.UnitType_Air) &&
        (attacker.getUnitType() !== GameEnums.UnitType_Naval) &&
        (attacker.getUnitType() !== GameEnums.UnitType_Infantry))
    {
        return 10;
    }
    else if ((attacker.getUnitType() === GameEnums.UnitType_Air) ||
        (attacker.getUnitType() === GameEnums.UnitType_Naval))
    {
        return -10;
    }
    return 0;
};
CO_JESS.getMovementpointModifier = function(co, unit, posX, posY, map)
{
    if (co.getPowerMode() > GameEnums.PowerMode_Off)
    {
        if ((unit.getUnitType() !== GameEnums.UnitType_Air) &&
            (unit.getUnitType() !== GameEnums.UnitType_Naval) &&
            (unit.getUnitType() !== GameEnums.UnitType_Infantry))
        {
            return 1;
        }
    }
    return 0;
};
