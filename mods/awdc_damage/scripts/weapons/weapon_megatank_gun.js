WEAPON_MEGATANK_GUN.getEnviromentDamage = function(enviromentId)
{
    return 65;
};
var idx = 0;
idx = getIndexOf1(WEAPON_MEGATANK_GUN.damageTable, "APC");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_MEGATANK_GUN.damageTable, "FLARE");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_MEGATANK_GUN.damageTable, "RECON");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 105;

// tanks
idx = getIndexOf1(WEAPON_MEGATANK_GUN.damageTable, "FLAK");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_MEGATANK_GUN.damageTable, "HOVERFLAK");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_MEGATANK_GUN.damageTable, "LIGHT_TANK");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_MEGATANK_GUN.damageTable, "HOVERCRAFT");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 85;

// heavy tanks
idx = getIndexOf1(WEAPON_MEGATANK_GUN.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_MEGATANK_GUN.damageTable, "HEAVY_TANK");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_MEGATANK_GUN.damageTable, "NEOTANK");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 75;

// very heavy tanks
idx = getIndexOf1(WEAPON_MEGATANK_GUN.damageTable, "MEGATANK");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 55;

idx = getIndexOf1(WEAPON_MEGATANK_GUN.damageTable, "HOELLIUM");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 35;

// ranged land units
idx = getIndexOf1(WEAPON_MEGATANK_GUN.damageTable, "ARTILLERY");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_MEGATANK_GUN.damageTable, "ANTITANKCANNON");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_MEGATANK_GUN.damageTable, "MISSILE");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_MEGATANK_GUN.damageTable, "ROCKETTHROWER");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_MEGATANK_GUN.damageTable, "PIPERUNNER");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 105;

// ships
idx = getIndexOf1(WEAPON_MEGATANK_GUN.damageTable, "BATTLESHIP");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 12;
idx = getIndexOf1(WEAPON_MEGATANK_GUN.damageTable, "CANNONBOAT");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_MEGATANK_GUN.damageTable, "BLACK_BOAT");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_MEGATANK_GUN.damageTable, "CRUISER");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 14;
idx = getIndexOf1(WEAPON_MEGATANK_GUN.damageTable, "DESTROYER");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 14;
idx = getIndexOf1(WEAPON_MEGATANK_GUN.damageTable, "SUBMARINE");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 14;
idx = getIndexOf1(WEAPON_MEGATANK_GUN.damageTable, "LANDER");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 28;
idx = getIndexOf1(WEAPON_MEGATANK_GUN.damageTable, "AIRCRAFTCARRIER");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 12;
