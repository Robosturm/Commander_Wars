CO_NELL.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(3);
};
CO_NELL.activateSuperpower = function(co, powerMode, map)
{
    CO_NELL.activatePower(co, powerMode);
};
CO_NELL.getSuperPowerDescription = function()
{
    return CO_NELL.getPowerDescription();
};
CO_NELL.getSuperPowerName = function()
{
    return CO_NELL.getPowerName();
};
CO_NELL.getBonusLuck = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        switch (co.getPowerMode())
        {
        case GameEnums.PowerMode_Tagpower:
        case GameEnums.PowerMode_Superpower:
        case GameEnums.PowerMode_Power:
            return 60;
        default:
            return 15;
        }
    }
    return 0;
};

CO_NELL.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    return 0;
};

CO_NELL.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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
