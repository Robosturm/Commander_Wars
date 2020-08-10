var idx = 0;
idx = getIndexOf1(WEAPON_AIR_TO_AIR_MISSILE.damageTable, "DUSTER");
WEAPON_AIR_TO_AIR_MISSILE.damageTable[idx][1] = 55 - WEAPON.effectiveBonus;
idx = getIndexOf1(WEAPON_AIR_TO_AIR_MISSILE.damageTable, "FIGHTER");
WEAPON_AIR_TO_AIR_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_AIR_TO_AIR_MISSILE.damageTable, "STEALTHBOMBER");
WEAPON_AIR_TO_AIR_MISSILE.damageTable[idx][1] = 55 + WEAPON.effectiveBonus;
