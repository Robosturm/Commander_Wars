WEAPON_CANNON.getEnviromentDamage = function(enviromentId)
{
    return 45;
};
var idx = 0;
idx = getIndexOf1(WEAPON_CANNON.damageTable, "INFANTRY");
WEAPON_CANNON.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_CANNON.damageTable, "MECH");
WEAPON_CANNON.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_CANNON.damageTable, "MOTORBIKE");
WEAPON_CANNON.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_CANNON.damageTable, "SNIPER");
WEAPON_CANNON.damageTable[idx][1] = 85;

// supporter
idx = getIndexOf1(WEAPON_CANNON.damageTable, "APC");
WEAPON_CANNON.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_CANNON.damageTable, "FLARE");
WEAPON_CANNON.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_CANNON.damageTable, "RECON");
WEAPON_CANNON.damageTable[idx][1] = 75;

// tanks
idx = getIndexOf1(WEAPON_CANNON.damageTable, "FLAK");
WEAPON_CANNON.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_CANNON.damageTable, "HOVERFLAK");
WEAPON_CANNON.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_CANNON.damageTable, "LIGHT_TANK");
WEAPON_CANNON.damageTable[idx][1] = 60;
idx = getIndexOf1(WEAPON_CANNON.damageTable, "HOVERCRAFT");
WEAPON_CANNON.damageTable[idx][1] = 60;

// heavy tanks
idx = getIndexOf1(WEAPON_CANNON.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_CANNON.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_CANNON.damageTable, "HEAVY_TANK");
WEAPON_CANNON.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_CANNON.damageTable, "NEOTANK");
WEAPON_CANNON.damageTable[idx][1] = 45;

// very heavy tanks
idx = getIndexOf1(WEAPON_CANNON.damageTable, "MEGATANK");
WEAPON_CANNON.damageTable[idx][1] = 35;

idx = getIndexOf1(WEAPON_CANNON.damageTable, "HOELLIUM");
WEAPON_CANNON.damageTable[idx][1] = 5;

// ranged land units
idx = getIndexOf1(WEAPON_CANNON.damageTable, "ARTILLERY");
WEAPON_CANNON.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_CANNON.damageTable, "ANTITANKCANNON");
WEAPON_CANNON.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_CANNON.damageTable, "MISSILE");
WEAPON_CANNON.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_CANNON.damageTable, "ROCKETTHROWER");
WEAPON_CANNON.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_CANNON.damageTable, "PIPERUNNER");
WEAPON_CANNON.damageTable[idx][1] = 80;

// ships
idx = getIndexOf1(WEAPON_CANNON.damageTable, "BATTLESHIP");
WEAPON_CANNON.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_CANNON.damageTable, "CANNONBOAT");
WEAPON_CANNON.damageTable[idx][1] = 100;
idx = getIndexOf1(WEAPON_CANNON.damageTable, "BLACK_BOAT");
WEAPON_CANNON.damageTable[idx][1] = 100;
idx = getIndexOf1(WEAPON_CANNON.damageTable, "CRUISER");
WEAPON_CANNON.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_CANNON.damageTable, "DESTROYER");
WEAPON_CANNON.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_CANNON.damageTable, "SUBMARINE");
WEAPON_CANNON.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_CANNON.damageTable, "LANDER");
WEAPON_CANNON.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_CANNON.damageTable, "AIRCRAFTCARRIER");
WEAPON_CANNON.damageTable[idx][1] = 45;
