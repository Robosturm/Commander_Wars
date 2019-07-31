WEAPON_MEGATANK_GUN.getEnviromentDamage = function(enviromentId)
{
    return 125;
};
var idx = 0;
idx = getIndexOf1(WEAPON_MEGATANK_GUN.damageTable, "APC");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 195;
idx = getIndexOf1(WEAPON_MEGATANK_GUN, "FLARE");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 195;
idx = getIndexOf1(WEAPON_MEGATANK_GUN, "RECON");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 195;

// tanks
idx = getIndexOf1(WEAPON_MEGATANK_GUN, "FLAK");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 195;
idx = getIndexOf1(WEAPON_MEGATANK_GUN, "HOVERFLAK");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 195;
idx = getIndexOf1(WEAPON_MEGATANK_GUN, "LIGHT_TANK");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 180;
idx = getIndexOf1(WEAPON_MEGATANK_GUN, "HOVERCRAFT");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 180;

// heavy tanks
idx = getIndexOf1(WEAPON_MEGATANK_GUN, "HEAVY_HOVERCRAFT");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 125;
idx = getIndexOf1(WEAPON_MEGATANK_GUN, "HEAVY_TANK");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 125;
idx = getIndexOf1(WEAPON_MEGATANK_GUN, "NEOTANK");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 115;

// very heavy tanks
idx = getIndexOf1(WEAPON_MEGATANK_GUN, "MEGATANK");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 65;

// ranged land units
idx = getIndexOf1(WEAPON_MEGATANK_GUN, "ARTILLERY");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 195;
idx = getIndexOf1(WEAPON_MEGATANK_GUN, "ANTITANKCANNON");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_MEGATANK_GUN, "MISSILE");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 195;
idx = getIndexOf1(WEAPON_MEGATANK_GUN, "ROCKETTHROWER");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 195;
idx = getIndexOf1(WEAPON_MEGATANK_GUN, "PIPERUNNER");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 180;

idx = getIndexOf1(WEAPON_MEGATANK_GUN, "HOELLIUM");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 35;

// ships
idx = getIndexOf1(WEAPON_MEGATANK_GUN, "BATTLESHIP");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_MEGATANK_GUN, "CANNONBOAT");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_MEGATANK_GUN, "CRUISER");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_MEGATANK_GUN, "DESTROYER");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_MEGATANK_GUN, "SUBMARINE");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_MEGATANK_GUN, "LANDER");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_MEGATANK_GUN, "BLACK_BOAT");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_MEGATANK_GUN, "AIRCRAFTCARRIER");
WEAPON_MEGATANK_GUN.damageTable[idx][1] = 45;
