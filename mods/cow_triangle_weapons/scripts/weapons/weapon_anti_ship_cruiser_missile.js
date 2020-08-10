var idx = 0;

idx = getIndexOf1(WEAPON_ANTI_SHIP_CRUISER_MISSILE.damageTable, "CRUISER");
WEAPON_ANTI_SHIP_CRUISER_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_ANTI_SHIP_CRUISER_MISSILE.damageTable, "DESTROYER");
WEAPON_ANTI_SHIP_CRUISER_MISSILE.damageTable[idx][1] = 55 - WEAPON.effectiveBonus;
idx = getIndexOf1(WEAPON_ANTI_SHIP_CRUISER_MISSILE.damageTable, "SUBMARINE");
WEAPON_ANTI_SHIP_CRUISER_MISSILE.damageTable[idx][1] = 55 + WEAPON.effectiveBonus;
