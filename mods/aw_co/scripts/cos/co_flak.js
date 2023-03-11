CO_FLAK.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_FLAK.activateSuperpower = function(co, powerMode, map)
{
    CO_FLAK.activatePower(co, map);
};
CO_FLAK.getSuperPowerDescription = function()
{
    return CO_FLAK.getPowerDescription();
};
CO_FLAK.getSuperPowerName = function()
{
    return CO_FLAK.getPowerName();
};
CO_FLAK.getBonusLuck = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 40;
        default:
            return 15;
        }
    }
    return 0;
};

CO_FLAK.getBonusMisfortune = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 20;
        default:
            return 10;
        }
    }
    return 0;
};

CO_FLAK.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    return 0;
};

CO_FLAK.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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
