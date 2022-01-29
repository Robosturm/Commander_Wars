CO_ADDER.init = function(co, map)
{
    co.setPowerStars(0);
    co.setSuperpowerStars(2);
};
CO_ADDER.activateSuperpower = function(co, powerMode, map)
{
    CO_ADDER.activatePower(co, powerMode);
};
CO_ADDER.getSuperPowerDescription = function()
{
    return CO_ADAM.getPowerDescription();
};
CO_ADDER.getSuperPowerName = function()
{
    return CO_ADDER.getPowerName();
};
CO_ADDER.getMovementpointModifier = function(co, unit, posX, posY, map)
{
    if (co.getIsCO0() === true)
    {
        if (co.getPowerMode() > GameEnums.PowerMode_Off)
        {
            return 1;
        }
    }
    return 0;
};
CO_ADDER.coZoneBonus = 0;
CO_ADDER.getOffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                      defender, defPosX, defPosY, isDefender, action, map)
{
    return 0;
};

CO_ADDER.getDeffensiveBonus = function(co, attacker, atkPosX, atkPosY,
                                       defender, defPosX, defPosY, isAttacker, action, map)
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

CO_ADDER.gainPowerstar = function(co, fundsDamage, x, y, hpDamage, defender, counterAttack, map)
{
    var powerGain = CO.getStarGain(co, fundsDamage, x, y, hpDamage, defender, counterAttack)
    co.setPowerFilled(co.getPowerFilled() + powerGain * 2 * 1.2);
};
