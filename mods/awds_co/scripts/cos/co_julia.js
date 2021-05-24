CO_JULIA.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action)
{
    if (co.getIsCO0() === true)
    {
        var baseDamage = 70;
        if (co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            baseDamage += 10;
        }
        return baseDamage * 10 / attacker.getHpRounded() - 100;
    }
    return 0;
};

CO_JULIA.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isDefender, action)
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
