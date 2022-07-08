CO_MEIYO.init = function(co, map)
{
    co.setPowerStars(3);
    co.setSuperpowerStars(3);
};
CO_MEIYO.activateSuperpower = function(co, powerMode, map)
{
	CO_MEIYO.activatePower(co, powerMode, map);
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
    switch (co.getPowerMode())
    {
    case GameEnums.PowerMode_Tagpower:
    case GameEnums.PowerMode_Superpower:
    case GameEnums.PowerMode_Power:
        switch (attacker.getUnitRank())
        {
        case 0:
            return 10;
        case 1:
            return 20;
        case 2:
            return 30;
        default:
            return 50;
        }
    default:
        if (co.inCORange(Qt.point(atkPosX, atkPosY), attacker))
        {
            switch (attacker.getUnitRank())
            {
            case 0:
                return 10;
            case 1:
                return 20;
            case 2:
                return 30;
            default:
                return 50;
            }
        }
        else
        {
            switch (attacker.getUnitRank())
            {
            case 0:
                return -5;
            case 1:
                return 2.5;
            case 2:
                return 5;
            default:
                return 10;
            }
        }
    }
};
CO_MEIYO.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                   defender, defPosX, defPosY, isAttacker, action, luckmode, map)
{
    switch (co.getPowerMode())
    {
    case GameEnums.PowerMode_Tagpower:
    case GameEnums.PowerMode_Superpower:
    case GameEnums.PowerMode_Power:
        switch (defender.getUnitRank())
        {
        case 0:
            return 10;
        case 1:
            return 20;
        case 2:
            return 30;
        default:
            return 50;
        }
    default:
        if (co.inCORange(Qt.point(defPosX, defPosY), defender))
        {
            switch (defender.getUnitRank())
            {
            case 0:
                return 10;
            case 1:
                return 20;
            case 2:
                return 30;
            default:
                return 50;
            }
        }
        else
        {
            switch (defender.getUnitRank())
            {
            case 0:
                return -5;
            case 1:
                return 2.5;
            case 2:
                return 5;
            default:
                return 10;
            }
        }
    }
};
