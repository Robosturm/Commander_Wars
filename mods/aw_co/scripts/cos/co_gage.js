CO_GAGE.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_GAGE.activateSuperpower = function(co, powerMode, map)
{
    CO_GAGE.activatePower(co, map);
};
CO_GAGE.getSuperPowerDescription = function()
{
    return CO_GAGE.getPowerDescription();
};
CO_GAGE.getSuperPowerName = function()
{
    return CO_GAGE.getPowerName();
};
CO_GAGE.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                     defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (attacker.getBaseMaxRange() > 1 ||
                attacker.getUnitType() === GameEnums.UnitType_Naval)
            {
                return 20;
            }
            break;
        default:
            if (attacker.getBaseMaxRange() > 1 ||
                attacker.getUnitType() === GameEnums.UnitType_Naval)
            {
                return 10;
            }
            break;
        }
    }
    return 0;
};
CO_GAGE.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isAttacker, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (defender.getBaseMaxRange() > 1 ||
                defender.getUnitType() === GameEnums.UnitType_Naval)
            {
                return 20;
            }
            break;
        default:
            if (defender.getBaseMaxRange() > 1 ||
                defender.getUnitType() === GameEnums.UnitType_Naval)
            {
                return 5;
            }
            break;
        }
    }
    return 0;
};

CO_GAGE.getFirerangeModifier = function(co, unit, posX, posY, map)
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
                return 2;
            }
            break;
        default:
            break;
        }
    }
    return 0;
};
