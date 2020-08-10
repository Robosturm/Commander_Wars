// air units
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "DUSTER");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 55 + WEAPON.effectiveBonus;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "FIGHTER");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 55 - WEAPON.effectiveBonus;
