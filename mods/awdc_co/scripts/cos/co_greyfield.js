CO_GREYFIELD.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_GREYFIELD.activateSuperpower = function(co, powerMode, map)
{
	CO_GREYFIELD.activatePower(co, powerMode);
};
CO_GREYFIELD.getSuperPowerDescription = function()
{
    return CO_GREYFIELD.getPowerDescription();
};
CO_GREYFIELD.getSuperPowerName = function()
{
    return CO_GREYFIELD.getPowerName();
};
CO_GREYFIELD.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    var boostUnit = CO_GREYFIELD.isBoostUnit(attacker);
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (boostUnit)
            {
                return 20;
            }
            else
            {
                return 10;
            }
        default:
            if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
            {
                if (boostUnit)
                {
                    return 20;
                }
                return 10;
            }
            break;
    }
    return 0;
};

CO_GREYFIELD.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isAttacker, action, luckmode, map)
{
    var boostUnit = CO_GREYFIELD.isBoostUnit(defender);
    switch (co.getPowerMode())
    {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (boostUnit)
            {
                return 50;
            }
            return 10;
        default:
            if (co.inCORange(Qt.point(defPosX, defPosY), defender))
            {
                if (boostUnit)
                {
                    return 50;
                }
                return 10;
            }
            break;
    }
    return 0;
};

CO_GREYFIELD.getMovementpointModifier = function(co, unit, posX, posY, map)
{
    return 0;
};
