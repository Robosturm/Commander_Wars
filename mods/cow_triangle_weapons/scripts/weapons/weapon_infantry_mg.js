var idx = 0;

idx = getIndexOf1(WEAPON_INFANTRY_MG.damageTable, "MECH");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 55 - WEAPON.effectiveBonus;
idx = getIndexOf1(WEAPON_INFANTRY_MG.damageTable, "MOTORBIKE");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 55 + WEAPON.effectiveBonus;
