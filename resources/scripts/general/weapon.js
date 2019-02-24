var WEAPON =
{
    // locals for reuse in derived class
    HighDamage : 85,
    mediumDamage : 55,
    lowDamage : 15,

    softCounterDamage : 35,
    softDamage : 65,


    getName : function()
    {
        return "";
    },

    getBaseDamage : function(unit)
    {
        return -1;
    },
    // aw dc damage formular used here.
    calculateDamage : function(hp, damage, offBonus, defBonus)
    {
        if (defBonus <= 0)
        {
            offBonus += defBonus;
            defBonus = 1;
        }
        if (hp < 0)
        {
            hp = 0;
        }
        var calcDamage = (hp / 10.0) * damage * (offBonus / defBonus);
        return calcDamage;
    },

};
