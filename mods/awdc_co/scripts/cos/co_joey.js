CO_JOEY.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_JOEY.activateSuperpower = function(co, powerMode, map)
{
	CO_JOEY.activatePower(co, powerMode);
};
CO_JOEY.getSuperPowerDescription = function()
{
    return CO_JOEY.getPowerDescription();
};
CO_JOEY.getSuperPowerName = function()
{
    return CO_JOEY.getPowerName();
};
CO_JOEY.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    if(defender !== null)
    {
        var attackerValue = attacker.getUnitValue();
        var defenderValue = defender.getUnitValue();
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                if (attackerValue > defenderValue)
                {
                    return 0;
                }
                else if (attackerValue < defenderValue)
                {
                    return 60;
                }
                else
                {
                    return 10;
                }
            default:
                if (attackerValue > defenderValue)
                {
                    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                    {
                        return 0;
                    }
                    return -10;
                }
                else if (attackerValue < defenderValue)
                {
                    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                    {
                        return 60;
                    }
                    return 10;
                }
                else
                {
                    if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
                    {
                        return 10;
                    }
                    return 0;
                }
            }
    }
    return 0;
};

CO_JOEY.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                             defender, defPosX, defPosY, isAttacker, action, luckmode, map)
{
    if(attacker !== null)
    {
        var attackerValue = attacker.getUnitValue();
        var defenderValue = defender.getUnitValue();
        switch (co.getPowerMode())
        {
            case GameEnums.PowerMode_Tagpower:
            case GameEnums.PowerMode_Superpower:
            case GameEnums.PowerMode_Power:
                if (attackerValue > defenderValue)
                {
                    return 30;
                }
                return 10;
            default:
        }
    }
    return 0;
};
CO_JOEY.getFirstStrike = function(co, unit, posX, posY, attacker, isDefender, map)
{
    return false;
};

CO_JOEY.getCostModifier = function(co, id, baseCost, posX, posY, map)
{
    return 0;
};
