var idx = 0;
// tanks
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "FLAK");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 55 - WEAPON.effectiveBonus;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "HOVERFLAK");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 55 - WEAPON.effectiveBonus;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "LIGHT_TANK");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 55 + WEAPON.effectiveBonus;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "HOVERCRAFT");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 55;

// heavy tanks
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 35 - WEAPON.effectiveBonus;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "HEAVY_TANK");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 35 + WEAPON.effectiveBonus;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "NEOTANK");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 35;
