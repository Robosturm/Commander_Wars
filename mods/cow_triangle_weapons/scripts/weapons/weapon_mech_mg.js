WEAPON_MECH_MG.getEnviromentDamage = function(enviromentId)
{
    return 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_MECH_MG.damageTable, "INFANTRY");
WEAPON_MECH_MG.damageTable[idx][1] = 55 + WEAPON.effectiveBonus;
idx = getIndexOf1(WEAPON_MECH_MG.damageTable, "MOTORBIKE");
WEAPON_MECH_MG.damageTable[idx][1] = 55 - WEAPON.effectiveBonus;
