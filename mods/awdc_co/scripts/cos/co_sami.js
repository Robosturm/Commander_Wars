CO_SAMI.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
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
            return 0;
        }
        return 10;
    default:
        if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
        {
            if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
            {
                return 50;
            }
            else if (attacker.getBaseMaxRange() === 1)
            {
                return 0;
            }
            return 10;
        }
        if (attacker.getUnitType() === GameEnums.UnitType_Infantry)
        {
            return 10;
        }
        break;
    }
    if (attacker.getBaseMaxRange() === 1)
    {
        return -10;
    }
    return 0;
};

CO_SAMI.getCaptureBonus = function(co, unit, posX, posY, map)
{
    var hp = unit.getHpRounded();
    return hp / 2;
};

CO_SAMI.getMovementpointModifier = function(co, unit, posX, posY, map)
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
    return 0;
};
