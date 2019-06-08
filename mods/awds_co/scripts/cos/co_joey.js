CO_JOEY.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                     defender, defPosX, defPosY, isDefender)
{
    if(defender !== null)
    {
        var attackerValue = attacker.getUnitValue();
        var defenderValue = defender.getUnitValue();
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (attackerValue > defenderValue)
            {
                return -10;
            }
            else if (attackerValue < defenderValue)
            {
                return 40;
            }
            else
            {
                return 0;
            }
        case GameEnums.PowerMode_Power:
            if (attackerValue > defenderValue)
            {
                return -10;
            }
            else if (attackerValue < defenderValue)
            {
                return 40;
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
                return 20;
            }
            else
            {
                return 0;
            }
        }
    }
    return 0;
};

CO_JOEY.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender)
{
    if(attacker !== null)
    {
        var attackerValue = attacker.getUnitValue();
        var defenderValue = defender.getUnitValue();
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            return 0;
        case GameEnums.PowerMode_Power:
            if (attackerValue < defenderValue)
            {
                return 20;
            }
            return 0;
        default:
        }
    }
    return 0;
};

CO_JOEY.getFirstStrike = function(co, unit, posX, posY, defender)
{
    if(defender !== null)
    {
        var attackerValue = unit.getUnitValue();
        var defenderValue = defender.getUnitValue();
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (attackerValue < defenderValue)
            {
                return true;
            }
            return false;
        case GameEnums.PowerMode_Power:
            return false;
        default:
            return false;
        }
    }
};

CO_JOEY.getCostModifier = function(co, id, baseCost)
{
    switch (co.getPowerMode())
    {
    case GameEnums.PowerMode_Tagpower:
    case GameEnums.PowerMode_Superpower:
        return -baseCost * 0.2;
    case GameEnums.PowerMode_Power:
        return 0;
    default:
        return 0;
    }
};
