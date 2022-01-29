CO_ROBOSTURM.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_ROBOSTURM.activateSuperpower = function(co, powerMode, map)
{
	CO_ROBOSTURM.activatePower(co, powerMode);
};
CO_ROBOSTURM.getSuperPowerDescription = function()
{
    return CO_ROBOSTURM.getPowerDescription();
};
CO_ROBOSTURM.getSuperPowerName = function()
{
    return CO_ROBOSTURM.getPowerName();
};
CO_ROBOSTURM.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isAttacker, action, map)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 60;
        default:
            if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return 30;
            }
            break;
    }
    return 0;
};

CO_ROBOSTURM.getMovementpointModifier = function(co, unit, posX, posY, map)
{
    if (co.getPowerMode() > GameEnums.PowerMode_Off)
    {
        return 2;
    }
    return 1;
};
