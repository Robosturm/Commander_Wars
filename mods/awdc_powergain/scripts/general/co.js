CO.getStarGain = function(co, fundsDamage, x, y, hpDamage, defender, counterAttack)
{
    var powerGain = 0;
    if (co.inCORange(Qt.point(x, y), null) && !defender)
    {
       powerGain = hpDamage / 10.0;
    }
    return powerGain;
};

CO.onCOUnitLost = function(co)
{
    // called when a co unit got destroyed
    co.setPowerFilled(0);
};

