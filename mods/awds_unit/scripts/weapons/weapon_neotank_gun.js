WEAPON_NEOTANK_GUN.getEnviromentDamage = function(enviromentId)
{
    return 75;
};
var idx = 0;
idx = getIndexOf1(WEAPON_NEOTANK_GUN.damageTable, "APC");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 125;
idx = getIndexOf1(WEAPON_NEOTANK_GUN, "FLARE");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 125;
idx = getIndexOf1(WEAPON_NEOTANK_GUN, "RECON");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 125;

// tanks
idx = getIndexOf1(WEAPON_NEOTANK_GUN, "FLAK");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 115;
idx = getIndexOf1(WEAPON_NEOTANK_GUN, "HOVERFLAK");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 115;
idx = getIndexOf1(WEAPON_NEOTANK_GUN, "LIGHT_TANK");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_NEOTANK_GUN, "HOVERCRAFT");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 105;

// heavy tanks
idx = getIndexOf1(WEAPON_NEOTANK_GUN, "HEAVY_HOVERCRAFT");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_NEOTANK_GUN, "HEAVY_TANK");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_NEOTANK_GUN, "NEOTANK");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 55;

// very heavy tanks
idx = getIndexOf1(WEAPON_NEOTANK_GUN, "MEGATANK");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 35;

idx = getIndexOf1(WEAPON_NEOTANK_GUN, "HOELLIUM");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 20;

// ranged land units
idx = getIndexOf1(WEAPON_NEOTANK_GUN, "ARTILLERY");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 115;
idx = getIndexOf1(WEAPON_NEOTANK_GUN, "ANTITANKCANNON");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_NEOTANK_GUN, "MISSILE");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 125;
idx = getIndexOf1(WEAPON_NEOTANK_GUN, "ROCKETTHROWER");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 125;
idx = getIndexOf1(WEAPON_NEOTANK_GUN, "PIPERUNNER");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 105;

// ships
idx = getIndexOf1(WEAPON_NEOTANK_GUN, "BATTLESHIP");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 15;
idx = getIndexOf1(WEAPON_NEOTANK_GUN, "CANNONBOAT");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 40;
idx = getIndexOf1(WEAPON_NEOTANK_GUN, "CRUISER");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 30;
idx = getIndexOf1(WEAPON_NEOTANK_GUN, "DESTROYER");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 30;
idx = getIndexOf1(WEAPON_NEOTANK_GUN, "SUBMARINE");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 15;
idx = getIndexOf1(WEAPON_NEOTANK_GUN, "LANDER");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 40;
idx = getIndexOf1(WEAPON_NEOTANK_GUN, "BLACK_BOAT");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 40;
idx = getIndexOf1(WEAPON_NEOTANK_GUN, "AIRCRAFTCARRIER");
WEAPON_NEOTANK_GUN.damageTable[idx][1] = 15;
