CO_RATTIGAN.init = function(co)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_RATTIGAN.activateSuperpower = function(co, powerMode)
{
	CO_RATTIGAN.activatePower(co, powerMode);
};
CO_RATTIGAN.getSuperPowerDescription = function()
{
    return CO_RATTIGAN.getPowerDescription();
};
CO_RATTIGAN.getSuperPowerName = function()
{
    return CO_RATTIGAN.getPowerName();
};
CO_RATTIGAN.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action)
{
    var count = CO_RATTIGAN.getUnitCount(co, defPosX, defPosY);
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (count > 0)
            {
                return 20 + count * 5 + 10;
            }
            return 10;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                if (count > 0)
                {
                    return 50;
                }
                return 0;
            }
            break;
    }
    if (count > 0)
    {
        return 5;
    }
    else
    {
        return -10;
    }
};

CO_RATTIGAN.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isAttacker, action)
{
    var count = CO_RATTIGAN.getUnitCount(co, defPosX, defPosY);
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 10;
        default:
            if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return 10;
            }
            break;
    }
    return 0;
};
CO_RATTIGAN.getMovementpointModifier = function(co, unit, posX, posY)
{
    if (co.getPowerMode() > GameEnums.PowerMode_Off)
    {
        return 1;
    }
    return 0;
};
