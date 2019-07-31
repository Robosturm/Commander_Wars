WEAPON_HEAVY_TANK_GUN.getEnviromentDamage = function(enviromentId)
{
    return 55;
};
var idx = 0;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "APC");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "FLARE");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "RECON");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 95;

// tanks
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "FLAK");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "HOVERFLAK");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "LIGHT_TANK");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "HOVERCRAFT");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 70;

// heavy tanks
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = WEAPON.HighDamage;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "HEAVY_TANK");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = WEAPON.mediumDamage;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "NEOTANK");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 45;

// very heavy tanks
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "MEGATANK");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 35;

idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "HOELLIUM");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 30;

// ranged land units
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "ARTILLERY");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "ANTITANKCANNON");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "MISSILE");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "ROCKETTHROWER");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "PIPERUNNER");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 90;

// ships
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "BATTLESHIP");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "CANNONBOAT");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "BLACK_BOAT");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "CRUISER");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 12;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "DESTROYER");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 12;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "SUBMARINE");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 12;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "LANDER");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 22;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "AIRCRAFTCARRIER");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 10;
