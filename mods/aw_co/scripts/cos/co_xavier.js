CO_XAVIER.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(4);
};
CO_XAVIER.activateSuperpower = function(co, powerMode, map)
{
    CO_XAVIER.activatePower(co, map);
};
CO_XAVIER.getSuperPowerDescription = function()
{
    return CO_XAVIER.getPowerDescription();
};
CO_XAVIER.getSuperPowerName = function()
{
    return CO_XAVIER.getPowerName();
};
CO_XAVIER.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action, luckmode, map)
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
            break;
        }
    }
    return 0;
};
CO_XAVIER.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                        defender, defPosX, defPosY, isAttacker, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 10;
        }
    }
    return 0;
};

CO_XAVIER.getBonusLuck = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        var hpRounded = unit.getHpRounded();
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (hpRounded <= 5)
            {
                return 20 + hpRounded / 2;
            }
            return 20;
        default:
            if (hpRounded <= 5)
            {
                return hpRounded / 2;
            }
        }
    }
    return 0;
};

CO_XAVIER.getBonusMisfortune = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        var hpRounded = unit.getHpRounded();
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            if (hpRounded <= 5)
            {
                return -20 + -hpRounded;
            }
            return -20;
        default:
            if (hpRounded <= 5)
            {
                return -hpRounded;
            }
            break;
        }
    }
    return 0;
};
