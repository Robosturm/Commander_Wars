CO_GRIT.init = function(co)
{
    co.setPowerStars(0);
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
    if (co.getIsCO0() === true)
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
                return -20;
            }
            return 0;
        default:
            if (attacker.getBaseMaxRange() > 1)
            {
                return 20;
            }
            break;
        }
        if (attacker.getBaseMaxRange() === 1 &&
            attacker.getUnitType() !== GameEnums.UnitType_Infantry)
        {
            return -20;
        }
    }
    return 0;
};
CO_GRIT.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
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
CO_GRIT.getFirerangeModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (unit.getBaseMaxRange() > 1)
            {
                return 3;
            }
            break;
        default:
            if (unit.getBaseMaxRange() > 1)
            {
                return 1;
            }
            break;
        }
    }
    return 0;
};
