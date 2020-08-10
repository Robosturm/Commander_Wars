var idx = 0;
idx = getIndexOf1(WEAPON_TORPEDO.damageTable, "CRUISER");
WEAPON_TORPEDO.damageTable[idx][1] = 55 - WEAPON.effectiveBonus;
idx = getIndexOf1(WEAPON_TORPEDO.damageTable, "DESTROYER");
WEAPON_TORPEDO.damageTable[idx][1] = 55 + WEAPON.effectiveBonus;
idx = getIndexOf1(WEAPON_TORPEDO.damageTable, "SUBMARINE");
WEAPON_TORPEDO.damageTable[idx][1] = 55;
