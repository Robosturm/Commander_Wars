var idx = 0;
// air
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "DUSTER");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "FIGHTER");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 55 - WEAPON.effectiveBonus;
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "STEALTHBOMBER");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 55 + WEAPON.effectiveBonus;
