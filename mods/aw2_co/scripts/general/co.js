CO.gainPowerstar = function(co, fundsDamage, x, y, hpDamage, defender, counterAttack)
{
    var powerGain = CO.getStarGain(co, fundsDamage, x, y, hpDamage, defender, counterAttack)
    co.setPowerFilled(co.getPowerFilled() + powerGain);
};


CO.getStarGain = function(co, fundsDamage, x, y, hpDamage, defender, counterAttack)
{
    var powerGain = fundsDamage;
    // reduce power meter filling based on power usages
    powerGain *= 1 / (1.0 + co.getPowerUsed() * 0.2);
    if (!defender)
    {
        powerGain *= 0.5;
    }
    return powerGain / 9000;
};
