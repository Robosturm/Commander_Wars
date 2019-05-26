CO_JULIA.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender)
{
    if (co.getIsCO0() === true)
    {
        var baseDamage = 75;
        return baseDamage * 10 / attacker.getHpRounded() - 100;
    }
    return 0;
};
