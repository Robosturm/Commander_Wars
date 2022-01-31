CO_MEIYO.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_MEIYO.activateSuperpower = function(co, powerMode, map)
{
    CO_MEIYO.activatePower(co, powerMode);
};
CO_MEIYO.getSuperPowerDescription = function()
{
    return CO_MEIYO.getPowerDescription();
};
CO_MEIYO.getSuperPowerName = function()
{
    return CO_MEIYO.getPowerName();
};
CO_MEIYO.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            switch (attacker.getUnitRank())
            {
            case 0:
                return 5;
            case 1:
                return 20;
            case 2:
                return 30;
            default:
                return 50;
            }
        default:
            switch (attacker.getUnitRank())
            {
            case 0:
                return -5;
            case 1:
                return 10;
            case 2:
                return 15;
            default:
                return 25;
            }
        }
    }
    return 0;
};

CO_MEIYO.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            switch (defender.getUnitRank())
            {
            case 0:
                return 5;
            case 1:
                return 20;
            case 2:
                return 30;
            default:
                return 50;
            }
        default:
            switch (defender.getUnitRank())
            {
            case 0:
                return -5;
            case 1:
                return 10;
            case 2:
                return 15;
            default:
                return 25;
            }
        }
    }
    return 0;
};
