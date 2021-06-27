CO_GRIT.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_GRIT.activateSuperpower = function(co, powerMode)
{
	CO_GRIT.activatePower(co, powerMode);
};
CO_GRIT.getSuperPowerDescription = function()
{
    return CO_GRIT.getPowerDescription();
};
CO_GRIT.getSuperPowerName = function()
{
    return CO_GRIT.getPowerName();
};
CO_GRIT.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (attacker.getBaseMaxRange() > 1)
            {
                return 50;
            }
            else if (attacker.getUnitType() === GameEnums.UnitType_Ground)
            {
                return -10;
            }
            return 10;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                if (attacker.getBaseMaxRange() > 1)
                {
                    return 30;
                }
                else if (attacker.getUnitType() === GameEnums.UnitType_Ground)
                {
                    return -10;
                }
                return 10;
            }
            break;
    }
    if (attacker.getBaseMaxRange() === 1 &&
        attacker.getUnitType() !== GameEnums.UnitType_Infantry)
    {
        return -20;
    }
    return 0;
};
CO_GRIT.getFirerangeModifier = function(co, unit, posX, posY)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (unit.getBaseMaxRange() > 1)
            {
                return 2;
            }
            break;
        default:
            if (unit.getBaseMaxRange() > 1)
            {
                return 1;
            }
            break;
    }
    return 0;
};
