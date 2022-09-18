CO_MAX.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_MAX.activateSuperpower = function(co, powerMode, map)
{
    CO_MAX.activatePower(co, powerMode, map);
};
CO_MAX.getSuperPowerDescription = function()
{
    return CO_MAX.getPowerDescription();
};
CO_MAX.getSuperPowerName = function()
{
    return CO_MAX.getPowerName();
};
CO_MAX.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                    defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (attacker.getBaseMaxRange() === 1 &&
                attacker.getUnitType() !== GameEnums.UnitType_Infantry)
            {
                return 70;
            }
            else if (attacker.getBaseMaxRange() > 1)
            {
                return -10;
            }
            return 0;
        default:
            if (attacker.getBaseMaxRange() === 1 &&
                attacker.getUnitType() !== GameEnums.UnitType_Infantry)
            {
                return 50;
            }
            break;
        }
        if (attacker.getBaseMaxRange() > 1)
        {
            return -10;
        }
    }
    return 0;
};

CO_MAX.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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

CO_MAX.getFirerangeModifier = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        if (unit.getBaseMaxRange() > 1)
        {
            return -1;
        }
    }
    return 0;
};

CO_MAX.getMovementpointModifier = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            if (unit.getBaseMaxRange() === 1 &&
                unit.getUnitType() !== GameEnums.UnitType_Infantry)
            {
                return 1;
            }
        }
    }
    return 0;
};
