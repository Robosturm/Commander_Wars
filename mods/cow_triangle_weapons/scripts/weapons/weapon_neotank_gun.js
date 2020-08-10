// tanks
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "FLAK");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "HOVERFLAK");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "LIGHT_TANK");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 75 - WEAPON.effectiveBonus;
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "HOVERCRAFT");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 75 + WEAPON.effectiveBonus;

// heavy tanks
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 55 + WEAPON.effectiveBonus;
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "HEAVY_TANK");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 55 - WEAPON.effectiveBonus;
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "NEOTANK");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 55;
