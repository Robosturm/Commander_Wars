CO_SAMI.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(2);
};
CO_SAMI.activateSuperpower = function(co, powerMode, map)
{
    CO_SAMI.activatePower(co, powerMode);
};
CO_SAMI.getSuperPowerDescription = function()
{
    return CO_SAMI.getPowerDescription();
};
CO_SAMI.getSuperPowerName = function()
{
    return CO_SAMI.getPowerName();
};
CO_SAMI.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                     defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
            {
                return 50;
            }
            else if (attacker.getBaseMaxRange() === 1)
            {
                return -10;
            }
            return 0;
        default:
            if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
            {
                return 20;
            }
            break;
        }
        if (attacker.getBaseMaxRange() === 1)
        {
            return -10;
        }
    }
    return 0;
};
CO_SAMI.getCaptureBonus = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        var hp = unit.getHpRounded();
        return hp / 2;
    }
    return 0;
};
CO_SAMI.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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
CO_SAMI.getMovementpointModifier = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        if (unit.isTransporter())
        {
            return 1;
        }
        if (unit.getUnitType() === GameEnums.UnitType_Infantry)
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                return 1;
            default:
                return 0;
            }
        }
    }
    return 0;
};
CO_SAMI.getMovementcostModifier = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        if (unit.getOwner() === co.getOwner())
        {
            switch (co.getPowerMode())
            {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                return -999;
            default:
                return 0;
            }
        }
    }
    return 0;
};
