CO_JOEY.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_JOEY.activateSuperpower = function(co, powerMode, map)
{
    CO_JOEY.activatePower(co, map);
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
    if (co.getIsCO0() === true)
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
                    return -10;
                }
                else if (attackerValue < defenderValue)
                {
                    return 30;
                }
                else
                {
                    return 0;
                }
            default:
                if (attackerValue > defenderValue)
                {
                    return -10;
                }
                else if (attackerValue < defenderValue)
                {
                    return 10;
                }
                else
                {
                    return 0;
                }
            }
        }
    }
    return 0;
};

CO_JOEY.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isAttacker, action, luckmode, map)
{
    if (co.getIsCO0() === true)
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
                if (attackerValue < defenderValue)
                {
                    return 30;
                }
                return 10;
            default:
            }
        }
    }
    return 0;
};

CO_JOEY.getFirstStrike = function(co, unit, posX, posY, attacker, isDefender, map, atkPosX, atkPosY)
{
    return false;
};

CO_JOEY.getCostModifier = function(co, id, baseCost, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 0;
        default:
            return 0;
        }
    }
    return 0;
};
