WEAPON_BOMB.getEnviromentDamage = function(enviromentId)
{
    return 95;
};
var idx = 0;
idx = getIndexOf1(WEAPON_BOMB.damageTable, "INFANTRY");
WEAPON_BOMB.damageTable[idx][1] = 115;
idx = getIndexOf1(WEAPON_BOMB.damageTable, "MECH");
WEAPON_BOMB.damageTable[idx][1] = 110;
idx = getIndexOf1(WEAPON_BOMB.damageTable, "MOTORBIKE");
WEAPON_BOMB.damageTable[idx][1] = 110;
idx = getIndexOf1(WEAPON_BOMB.damageTable, "SNIPER");
WEAPON_BOMB.damageTable[idx][1] = 110;

// supporter
idx = getIndexOf1(WEAPON_BOMB.damageTable, "APC");
WEAPON_BOMB.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_BOMB.damageTable, "FLARE");
WEAPON_BOMB.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_BOMB.damageTable, "RECON");
WEAPON_BOMB.damageTable[idx][1] = 105;

// tanks
idx = getIndexOf1(WEAPON_BOMB.damageTable, "FLAK");
WEAPON_BOMB.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_BOMB.damageTable, "HOVERFLAK");
WEAPON_BOMB.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_BOMB.damageTable, "LIGHT_TANK");
WEAPON_BOMB.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_BOMB.damageTable, "HOVERCRAFT");
WEAPON_BOMB.damageTable[idx][1] = 105;

// heavy tanks
idx = getIndexOf1(WEAPON_BOMB.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_BOMB.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_BOMB.damageTable, "HEAVY_TANK");
WEAPON_BOMB.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_BOMB.damageTable, "NEOTANK");
WEAPON_BOMB.damageTable[idx][1] = 95;

// very heavy tanks
idx = getIndexOf1(WEAPON_BOMB.damageTable, "MEGATANK");
WEAPON_BOMB.damageTable[idx][1] = 75;

idx = getIndexOf1(WEAPON_BOMB.damageTable, "HOELLIUM");
WEAPON_BOMB.damageTable[idx][1] = 35;

// ranged land units
idx = getIndexOf1(WEAPON_BOMB.damageTable, "ARTILLERY");
WEAPON_BOMB.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_BOMB.damageTable, "ANTITANKCANNON");
WEAPON_BOMB.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_BOMB.damageTable, "MISSILE");
WEAPON_BOMB.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_BOMB.damageTable, "ROCKETTHROWER");
WEAPON_BOMB.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_BOMB.damageTable, "PIPERUNNER");
WEAPON_BOMB.damageTable[idx][1] = 105;

// ships
idx = getIndexOf1(WEAPON_BOMB.damageTable, "BATTLESHIP");
WEAPON_BOMB.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_BOMB.damageTable, "CANNONBOAT");
WEAPON_BOMB.damageTable[idx][1] = 120;
idx = getIndexOf1(WEAPON_BOMB.damageTable, "BLACK_BOAT");
WEAPON_BOMB.damageTable[idx][1] = 120;
idx = getIndexOf1(WEAPON_BOMB.damageTable, "CRUISER");
WEAPON_BOMB.damageTable[idx][1] = 50;
idx = getIndexOf1(WEAPON_BOMB.damageTable, "DESTROYER");
WEAPON_BOMB.damageTable[idx][1] = 50;
idx = getIndexOf1(WEAPON_BOMB.damageTable, "SUBMARINE");
WEAPON_BOMB.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_BOMB.damageTable, "LANDER");
WEAPON_BOMB.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_BOMB.damageTable, "AIRCRAFTCARRIER");
WEAPON_BOMB.damageTable[idx][1] = 85;
