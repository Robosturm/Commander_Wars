CO.gainPowerstar = function(co, fundsDamage, x, y, hpDamage, defender)
{
    var powerGain = CO.getStarGain(co, fundsDamage, x, y, hpDamage, defender)
    co.setPowerFilled(co.getPowerFilled() + powerGain * 2);
};

