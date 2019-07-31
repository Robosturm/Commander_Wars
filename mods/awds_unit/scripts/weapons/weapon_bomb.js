WEAPON_BOMB.getEnviromentDamage = function(enviromentId)
{
    return 95;
};
var idx = 0;
idx = getIndexOf1(WEAPON_BOMB.damageTable, "INFANTRY");
WEAPON_BOMB.damageTable[idx][1] = 110;
idx = getIndexOf1(WEAPON_BOMB, "MECH");
WEAPON_BOMB.damageTable[idx][1] = 110;
idx = getIndexOf1(WEAPON_BOMB, "MOTORBIKE");
WEAPON_BOMB.damageTable[idx][1] = 110;
idx = getIndexOf1(WEAPON_BOMB, "SNIPER");
WEAPON_BOMB.damageTable[idx][1] = 110;

// supporter
idx = getIndexOf1(WEAPON_BOMB, "APC");
WEAPON_BOMB.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_BOMB, "FLARE");
WEAPON_BOMB.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_BOMB, "RECON");
WEAPON_BOMB.damageTable[idx][1] = 105;

// tanks
idx = getIndexOf1(WEAPON_BOMB, "FLAK");
WEAPON_BOMB.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_BOMB, "HOVERFLAK");
WEAPON_BOMB.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_BOMB, "LIGHT_TANK");
WEAPON_BOMB.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_BOMB, "HOVERCRAFT");
WEAPON_BOMB.damageTable[idx][1] = 105;

// heavy tanks
idx = getIndexOf1(WEAPON_BOMB, "HEAVY_HOVERCRAFT");
WEAPON_BOMB.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_BOMB, "HEAVY_TANK");
WEAPON_BOMB.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_BOMB, "NEOTANK");
WEAPON_BOMB.damageTable[idx][1] = 90;

// very heavy tanks
idx = getIndexOf1(WEAPON_BOMB, "MEGATANK");
WEAPON_BOMB.damageTable[idx][1] = 35;

idx = getIndexOf1(WEAPON_BOMB, "HOELLIUM");
WEAPON_BOMB.damageTable[idx][1] = 35;

// ranged land units
idx = getIndexOf1(WEAPON_BOMB, "ARTILLERY");
WEAPON_BOMB.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_BOMB, "ANTITANKCANNON");
WEAPON_BOMB.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_BOMB, "MISSILE");
WEAPON_BOMB.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_BOMB, "ROCKETTHROWER");
WEAPON_BOMB.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_BOMB, "PIPERUNNER");
WEAPON_BOMB.damageTable[idx][1] = 105;

// ships
idx = getIndexOf1(WEAPON_BOMB, "BATTLESHIP");
WEAPON_BOMB.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_BOMB, "CANNONBOAT");
WEAPON_BOMB.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_BOMB, "CRUISER");
WEAPON_BOMB.damageTable[idx][1] = 50;
idx = getIndexOf1(WEAPON_BOMB, "DESTROYER");
WEAPON_BOMB.damageTable[idx][1] = 50;
idx = getIndexOf1(WEAPON_BOMB, "SUBMARINE");
WEAPON_BOMB.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_BOMB, "LANDER");
WEAPON_BOMB.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_BOMB, "BLACK_BOAT");
WEAPON_BOMB.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_BOMB, "AIRCRAFTCARRIER");
WEAPON_BOMB.damageTable[idx][1] = 105;
