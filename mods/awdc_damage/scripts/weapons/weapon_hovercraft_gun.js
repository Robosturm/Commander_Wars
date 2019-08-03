WEAPON_HOVERCRAFT_GUN.getEnviromentDamage = function(enviromentId)
{
    return 15;
};
var idx = 0;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN.damageTable, "APC");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN, "FLARE");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN, "RECON");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 85;

// tanks
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN, "FLAK");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN, "HOVERFLAK");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN, "LIGHT_TANK");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN, "HOVERCRAFT");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 55;

// heavy tanks
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN, "HEAVY_HOVERCRAFT");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN, "HEAVY_TANK");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN, "NEOTANK");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 35;

// very heavy tanks
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN, "MEGATANK");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 20;

idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN, "HOELLIUM");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 20;

// ranged land units
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN, "ARTILLERY");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN, "ANTITANKCANNON");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 30;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN, "MISSILE");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN, "ROCKETTHROWER");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN, "PIPERUNNER");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 85;

// ships
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN, "BATTLESHIP");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN, "CANNONBOAT");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN, "BLACK_BOAT");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN, "CRUISER");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 9;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN, "DESTROYER");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 9;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN, "SUBMARINE");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 9;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN, "LANDER");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 18;
idx = getIndexOf1(WEAPON_HOVERCRAFT_GUN, "AIRCRAFTCARRIER");
WEAPON_HOVERCRAFT_GUN.damageTable[idx][1] = 8;
