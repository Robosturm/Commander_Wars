CO_JOEY.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                     defender, defPosX, defPosY, isDefender, action)
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
                if (attackerValue > defenderValue)
                {
                    return 0;
                }
                else if (attackerValue < defenderValue)
                {
                    return 40;
                }
                else
                {
                    return 10;
                }
            case GameEnums.PowerMode_Power:
                if (attackerValue > defenderValue)
                {
                    return 0;
                }
                else if (attackerValue < defenderValue)
                {
                    return 40;
                }
                else
                {
                    return 10;
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
                                      defender, defPosX, defPosY, isDefender, action)
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
                return 10;
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

CO_JOEY.getFirstStrike = function(co, unit, posX, posY, attacker)
{
    if (co.getIsCO0() === true)
    {
        if(unit !== null)
        {
            var defenderValue = unit.getUnitValue();
            var attackerValue = attacker.getUnitValue();
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
    }
    return false;
};

CO_JOEY.getCostModifier = function(co, id, baseCost)
{
    if (co.getIsCO0() === true)
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
    }
    return 0;
};
