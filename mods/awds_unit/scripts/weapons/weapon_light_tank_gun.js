WEAPON_LIGHT_TANK_GUN.getEnviromentDamage = function(enviromentId)
{
    return 15;
};
var idx = 0;
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN.damageTable, "APC");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN, "FLARE");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN, "RECON");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 85;

// tanks
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN, "FLAK");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN, "HOVERFLAK");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN, "LIGHT_TANK");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN, "HOVERCRAFT");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 55;

// heavy tanks
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN, "HEAVY_HOVERCRAFT");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 15;
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN, "HEAVY_TANK");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 15;
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN, "NEOTANK");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 15;

// very heavy tanks
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN, "MEGATANK");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 10;

idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN, "HOELLIUM");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 20;

// ranged land units
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN, "ARTILLERY");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN, "ANTITANKCANNON");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 30;
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN, "MISSILE");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN, "ROCKETTHROWER");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN, "PIPERUNNER");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 55;

// ships
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN, "BATTLESHIP");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN, "CANNONBOAT");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN, "CRUISER");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 5;
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN, "DESTROYER");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 5;
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN, "SUBMARINE");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN, "LANDER");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN, "BLACK_BOAT");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_LIGHT_TANK_GUN, "AIRCRAFTCARRIER");
WEAPON_LIGHT_TANK_GUN.damageTable[idx][1] = 1;
