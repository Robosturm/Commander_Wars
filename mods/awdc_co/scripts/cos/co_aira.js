CO_AIRA.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_AIRA.activateSuperpower = function(co, powerMode)
{
	CO_AIRA.activatePower(co, powerMode);
};
CO_AIRA.getSuperPowerDescription = function()
{
    return CO_AIRA.getPowerDescription();
};
CO_AIRA.getSuperPowerName = function()
{
    return CO_AIRA.getPowerName();
};
CO_AIRA.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return CO_AIRA.coZoneBonus;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return CO_AIRA.coZoneBonus;
            }
            break;
    }
    return 0;
};

CO_AIRA.getMovementFuelCostModifier = function(co, unit, fuelCost)
{
    if (co.getPowerMode() > GameEnums.PowerMode_Off &&
        co.getOwner().isEnemyUnit(unit) === true)
    {
        return fuelCost * 5;
    }
    return 0;
};

CO_AIRA.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isAttacker, action)
    {
        if (co.inCORange(Qt.point(defPosX, defPosY), defender) ||
            co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return CO_AIRA.coZoneBonus;
        }
        return 0;
    };

CO_AIRA.getMovementcostModifier = function(co, unit, posX, posY)
{
    return 0;
};
