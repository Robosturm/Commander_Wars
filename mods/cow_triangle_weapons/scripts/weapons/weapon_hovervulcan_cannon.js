var idx = 0;
// tanks
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "FLAK");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "HOVERFLAK");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 55 + WEAPON.effectiveBonus;;
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "LIGHT_TANK");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 55 - WEAPON.effectiveBonus;;
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "HOVERCRAFT");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 55 + WEAPON.effectiveBonus;;

// heavy tanks
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 10 + WEAPON.effectiveBonus;
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "HEAVY_TANK");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 0;
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "NEOTANK");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 10;
