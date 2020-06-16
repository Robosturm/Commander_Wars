WEAPON_HOVERCRAFT_GUN.getEnviromentDamage = function(enviromentId)
{
    return 15;
};
var idx = 0;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "APC");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "FLARE");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "RECON");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 85;

// tanks
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "FLAK");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "HOVERFLAK");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "LIGHT_TANK");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "HOVERCRAFT");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 55;

// heavy tanks
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "HEAVY_TANK");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "NEOTANK");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 35;

// very heavy tanks
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "MEGATANK");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 20;

idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "HOELLIUM");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 20;

// ranged land units
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "ARTILLERY");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "ANTITANKCANNON");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 30;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "MISSILE");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "ROCKETTHROWER");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "PIPERUNNER");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 85;

// ships
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "BATTLESHIP");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "CANNONBOAT");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "BLACK_BOAT");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "CRUISER");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 9;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "DESTROYER");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 9;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "SUBMARINE");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 9;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "LANDER");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 18;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "AIRCRAFTCARRIER");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 8;
