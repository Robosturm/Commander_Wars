var WEAPON =
{
    // locals for reuse in derived class
    HighDamage : 85,
    mediumDamage : 55,
    lowDamage : 15,

    getName : function()
    {
        return "";
    },

    getBaseDamage : function(unit)
    {
        return -1;
    },
    // aw dc damage formular used here.
    calculateDamage : function(hp, damage, bonusDmg, offBonus, defBonus)
    {
        if (defBonus <= 0)
        {
            offBonus += defBonus;
            defBonus = 1;
        }
        var calcDamage = (hp / 10.0) * damage * (offBonus / defBonus) + bonusDmg;
        return calcDamage;
    },

};
