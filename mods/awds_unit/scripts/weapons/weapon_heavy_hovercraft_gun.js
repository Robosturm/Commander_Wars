WEAPON_HEAVY_HOVERCRAFT_GUN.getEnviromentDamage = function(enviromentId)
{
    return 55;
};
var idx = 0;
idx = getIndexOf1(WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable, "APC");
WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable, "FLARE");
WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable, "RECON");
WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable[idx][1] = 105;

// tanks
idx = getIndexOf1(WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable, "FLAK");
WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable, "HOVERFLAK");
WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable, "LIGHT_TANK");
WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable, "HOVERCRAFT");
WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable[idx][1] = 85;

// heavy tanks
idx = getIndexOf1(WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable, "HEAVY_TANK");
WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable, "NEOTANK");
WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable[idx][1] = 45;

// very heavy tanks
idx = getIndexOf1(WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable, "MEGATANK");
WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable[idx][1] = 25;

idx = getIndexOf1(WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable, "HOELLIUM");
WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable[idx][1] = 30;

// ranged land units
idx = getIndexOf1(WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable, "ARTILLERY");
WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable, "ANTITANKCANNON");
WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable, "MISSILE");
WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable, "ROCKETTHROWER");
WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable, "PIPERUNNER");
WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable[idx][1] = 85;

// ships
idx = getIndexOf1(WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable, "BATTLESHIP");
WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable, "CANNONBOAT");
WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable, "CRUISER");
WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable[idx][1] = 30;
idx = getIndexOf1(WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable, "DESTROYER");
WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable[idx][1] = 30;
idx = getIndexOf1(WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable, "SUBMARINE");
WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable, "LANDER");
WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable, "BLACK_BOAT");
WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable, "AIRCRAFTCARRIER");
WEAPON_HEAVY_HOVERCRAFT_GUN.damageTable[idx][1] = 10;
