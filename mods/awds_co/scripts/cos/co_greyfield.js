CO_GREYFIELD.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                          defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        var boostUnit = CO_GREYFIELD.isBoostUnit(attacker);
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (boostUnit)
            {
                return 70;
            }
            return 10;
        case GameEnums.PowerMode_Power:
            if (boostUnit)
            {
                return 30;
            }
            return 10;
        default:
            if (boostUnit)
            {
                return 0;
            }
            break;
        }
    }
    return 0;
};

CO_GREYFIELD.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                           defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        var boostUnit = CO_GREYFIELD.isBoostUnit(defender);
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (boostUnit)
            {
                return 70;
            }
            return 10;
        case GameEnums.PowerMode_Power:
            if (boostUnit)
            {
                return 40;
            }
            return 10;
        default:
            if (boostUnit)
            {
                return 20;
            }
            break;
        }
    }
    return 0;
};

CO_GREYFIELD.getMovementpointModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        var boostUnit = CO_GREYFIELD.isBoostUnit(unit);
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
            if (boostUnit)
            {
                return 1;
            }
            return 0;
        case GameEnums.PowerMode_Power:
            return 0;
        default:
            break;
        }
    }
    return 0;
};
