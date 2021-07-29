CO_EAGLE.init = function(co)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_EAGLE.activateSuperpower = function(co, powerMode)
{
    CO_EAGLE.activatePower(co, powerMode);
};
CO_EAGLE.getSuperPowerDescription = function()
{
    return CO_EAGLE.getPowerDescription();
};
CO_EAGLE.getSuperPowerName = function()
{
    return CO_EAGLE.getPowerName();
};
CO_EAGLE.getFuelCostModifier = function(co, unit, costs)
{
    if (co.getIsCO0() === true)
    {
        if (unit.getUnitType() === GameEnums.UnitType_Air)
        {
            return -2;
        }
    }
    return 0;
};

CO_EAGLE.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (attacker.getUnitType() === GameEnums.UnitType_Air)
            {
                return 0;
            }
            else if (attacker.getUnitType() === GameEnums.UnitType_Naval)
            {
                return -30;
            }
            else
            {
                return -20;
            }
        default:
            if (attacker.getUnitType() === GameEnums.UnitType_Air)
            {
                return 20;
            }
            break;
        }
        if (attacker.getUnitType() === GameEnums.UnitType_Naval)
        {
            return -10;
        }
    }
    return 0;
};

CO_EAGLE.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return -30;
        default:
            if (defender.getUnitType() === GameEnums.UnitType_Air)
            {
                return 10;
            }
            break;
        }
    }
    return 0;
};
