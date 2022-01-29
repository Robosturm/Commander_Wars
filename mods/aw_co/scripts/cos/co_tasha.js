CO_TASHA.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_TASHA.activateSuperpower = function(co, powerMode, map)
{
    CO_TASHA.activatePower(co, powerMode);
};
CO_TASHA.getSuperPowerDescription = function()
{
    return CO_TASHA.getPowerDescription();
};
CO_TASHA.getSuperPowerName = function()
{
    return CO_TASHA.getPowerName();
};
CO_TASHA.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (attacker.getUnitType() === GameEnums.UnitType_Air)
            {
                return 40;
            }
            else
            {
                return 0;
            }
        default:
            if (attacker.getUnitType() === GameEnums.UnitType_Air)
            {
                return 15;
            }
        }
    }
    return 0;
};

CO_TASHA.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (defender.getUnitType() === GameEnums.UnitType_Air)
            {
                return 30;
            }
            return 10;
        default:
            break;
        }
    }
    return 0;
};

CO_TASHA.getMovementpointModifier = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 1;
        }
    }
    return 0;
};
