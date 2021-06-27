CO.gainPowerstar = function(co, fundsDamage, x, y, hpDamage, defender, counterAttack)
{
    var powerGain = CO.getStarGain(co, fundsDamage, x, y, hpDamage, defender, counterAttack)
    co.setPowerFilled(co.getPowerFilled() + powerGain * 2);

};

