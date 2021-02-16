var WEAPON =
{
    damageTable = [],
    getName : function()
    {
        return "";
    },
    getBaseDamage : function(unit)
    {
        return -1
    },

    getDamageFromTable : function(unit, damageTable, weaponID)
    {
        var unitID = unit.getUnitDamageID();
        for (var i = 0; i < damageTable.length; i++)
        {
            if (damageTable[i][0] === unitID)
            {
               return damageTable[i][1];
            }
        }
        // nothing found :(
        return unit.getUnitDamage(weaponID);
    },
    getEnviromentDamage : function(enviromentId)
    {
        return -1;
    },
    // aw dc damage formular used here.
    calculateDamage : function(hp, damage, offBonus, defBonus, luckDamage = 0)
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
        var calcDamage = (hp / 10.0) * damage * (offBonus / defBonus)
            + luckDamage * (hp / 10.0) * (offBonus / defBonus);
        return calcDamage;
    },

};
