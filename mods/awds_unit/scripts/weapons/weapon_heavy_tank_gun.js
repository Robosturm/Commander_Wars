WEAPON_HEAVY_TANK_GUN.getEnviromentDamage = function(enviromentId)
{
    return 55;
};
var idx = 0;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "APC");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN, "FLARE");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN, "RECON");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 105;

// tanks
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN, "FLAK");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN, "HOVERFLAK");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN, "LIGHT_TANK");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN, "HOVERCRAFT");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 85;

// heavy tanks
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN, "HEAVY_HOVERCRAFT");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN, "HEAVY_TANK");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN, "NEOTANK");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 45;

// very heavy tanks
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN, "MEGATANK");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 25;

idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN, "HOELLIUM");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 30;

// ranged land units
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN, "ARTILLERY");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN, "ANTITANKCANNON");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN, "MISSILE");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN, "ROCKETTHROWER");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN, "PIPERUNNER");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 85;

// ships
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN, "BATTLESHIP");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN, "CANNONBOAT");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN, "CRUISER");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 30;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN, "DESTROYER");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 30;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN, "SUBMARINE");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN, "LANDER");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN, "BLACK_BOAT");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN, "AIRCRAFTCARRIER");
WEAPON_HEAVY_TANK_GUN.damageTable[idx][1] = 10;
