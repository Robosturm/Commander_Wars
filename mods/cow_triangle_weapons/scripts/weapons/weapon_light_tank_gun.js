var idx = 0;
// tanks
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN.damageTable, "FLAK");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 55 + WEAPON.effectiveBonus;
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN.damageTable, "HOVERFLAK");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 55 + WEAPON.effectiveBonus;
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN.damageTable, "LIGHT_TANK");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN.damageTable, "HOVERCRAFT");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 55 - WEAPON.effectiveBonus;

// heavy tanks
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 35 - WEAPON.effectiveBonus;
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN.damageTable, "HEAVY_TANK");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN.damageTable, "NEOTANK");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 35 + WEAPON.effectiveBonus;
