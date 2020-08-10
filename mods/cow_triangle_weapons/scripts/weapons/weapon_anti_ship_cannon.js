var idx = 0;
idx = getIndexOf1(WEAPON_ANTI_SHIP_CANNON.damageTable, "CRUISER");
WEAPON_ANTI_SHIP_CANNON.damageTable[idx][1] = 55 + WEAPON.effectiveBonus;
idx = getIndexOf1(WEAPON_ANTI_SHIP_CANNON.damageTable, "DESTROYER");
WEAPON_ANTI_SHIP_CANNON.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_ANTI_SHIP_CANNON.damageTable, "SUBMARINE");
WEAPON_ANTI_SHIP_CANNON.damageTable[idx][1] = 55 - WEAPON.effectiveBonus;
