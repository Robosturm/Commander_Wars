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
    calculateDamage : function(hp, damage, offBonus, defBonus, luckDamage = 0, map)
    {
        var formula = map.getGameRules().getDamageFormula();
        var calcDamage = 0;
        if (formula === GameEnums.DamageFormula_AdvanceWars4)
        {
            if (defBonus <= 0)
            {
                offBonus += defBonus;
                defBonus = 1;
            }
            if (hp <= 0)
            {
                hp = 0;
            }
            calcDamage = (hp / 10.0) * damage * (offBonus / defBonus)
                    + luckDamage * (hp / 10.0) * 100 / defBonus;
        }
        else
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
            calcDamage = (hp / 10.0) * (damage * offBonus) / 100 * ((200 - defBonus) / 100)
                    + luckDamage * (hp / 10.0) * ((200 - defBonus) / 100);
            if (calcDamage < 0 && damage >= 0)
            {
                calcDamage = 0;
            }
        }

        return calcDamage;
    },

};
