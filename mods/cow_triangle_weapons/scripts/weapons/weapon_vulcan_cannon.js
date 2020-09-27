// tanks
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "FLAK");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "HOVERFLAK");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 55 + WEAPON.effectiveBonus;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "LIGHT_TANK");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 55 - WEAPON.effectiveBonus;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "HOVERCRAFT");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 55 + WEAPON.effectiveBonus;

// heavy tanks
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 10 + WEAPON.effectiveBonus;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "HEAVY_TANK");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "NEOTANK");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 10;
