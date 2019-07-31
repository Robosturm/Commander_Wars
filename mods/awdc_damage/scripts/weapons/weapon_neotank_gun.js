WEAPON_NEOTANK_GUN.getEnviromentDamage = function(enviromentId)
{
    return 55;
};
var idx = 0;
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "APC");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "FLARE");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "RECON");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 95;

// tanks
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "FLAK");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "HOVERFLAK");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "LIGHT_TANK");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "HOVERCRAFT");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 70;

// heavy tanks
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "HEAVY_TANK");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = WEAPON.HighDamage;
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "NEOTANK");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = WEAPON.mediumDamage;

// very heavy tanks
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "MEGATANK");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 35;

idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "HOELLIUM");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 30;

// ranged land units
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "ARTILLERY");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "ANTITANKCANNON");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "MISSILE");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "ROCKETTHROWER");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "PIPERUNNER");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 90;

// ships
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "BATTLESHIP");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "CANNONBOAT");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "BLACK_BOAT");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "CRUISER");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 12;
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "DESTROYER");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 12;
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "SUBMARINE");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 12;
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "LANDER");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 22;
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "AIRCRAFTCARRIER");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 10;
