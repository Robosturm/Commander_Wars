CO_JULIA.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(5);
};
CO_JULIA.activateSuperpower = function(co, powerMode, map)
{
    CO_JULIA.activatePower(co, powerMode, map);
};
CO_JULIA.getSuperPowerDescription = function()
{
    return CO_JULIA.getPowerDescription();
};
CO_JULIA.getSuperPowerName = function()
{
    return CO_JULIA.getPowerName();
};
CO_JULIA.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, luckmode, map)
{
    if (co.getIsCO0() === true)
    {
        var baseDamage = 70;
        return baseDamage * 10 / attacker.getHpRounded() - 100;
    }
    return 0;
};

CO_JULIA.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
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
