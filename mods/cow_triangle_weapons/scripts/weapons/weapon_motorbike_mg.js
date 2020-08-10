var idx = 0;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG.damageTable, "INFANTRY");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 55 - WEAPON.effectiveBonus;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG.damageTable, "MECH");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 55 + WEAPON.effectiveBonus;
