CO_ISABELLA.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_ISABELLA.activateSuperpower = function(co, powerMode, map)
{
	CO_ISABELLA.activatePower(co, powerMode);
};
CO_ISABELLA.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 20;
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                return 20;
            }
            break;
    }
    return 0;
};

CO_ISABELLA.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isAttacker, action, luckmode, map)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 20;
        default:
            if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                return 20;
            }
            break;
    }
    return 0;
};

CO_ISABELLA.getFirerangeModifier = function(co, unit, posX, posY, map)
{
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (unit.getBaseMaxRange() > 1)
            {
                return 1;
            }
            return 0;
        default:
            return 0;
    }
};

CO_ISABELLA.getMovementpointModifier = function(co, unit, posX, posY, map)
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
};
