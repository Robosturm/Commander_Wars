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

    getDamageFromTable : function(unit, damageTable)
    {
        var unitID = unit.getUnitID();
        for (var i = 0; i < damageTable.length; i++)
        {
            if (damageTable[i][0] === unitID)
            {
               return damageTable[i][1];
            }
        }
        return -1;
    },
    getEnviromentDamage : function(enviromentId)
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
