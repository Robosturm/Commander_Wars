CO_ADDER.getMovementpointModifier = function(co, unit, posX, posY)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() === GameEnums.PowerMode_Superpower ||
            co.getPowerMode() === GameEnums.PowerMode_Tagpower)
        {
            return 2;
        }
        else if (co.getPowerMode() === GameEnums.PowerMode_Power)
        {
            return 1;
        }
    }
    return 0;
};

CO_ADDER.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                 defender, defPosX, defPosY, isDefender)
{
    return 0;
};

CO_ADDER.gainPowerstar = function(co, fondsDamage, x, y)
{
    // adder gains more power than other co's
    co.setPowerFilled(co.getPowerFilled() + (fondsDamage / 11000) * 2 * 1.2)
};
