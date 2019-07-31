WEAPON_CANNON.getEnviromentDamage = function(enviromentId)
{
    return 45;
};
var idx = 0;
idx = getIndexOf1(WEAPON_CANNON.damageTable, "INFANTRY");
WEAPON_CANNON.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_CANNON, "MECH");
WEAPON_CANNON.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_CANNON, "MOTORBIKE");
WEAPON_CANNON.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_CANNON, "SNIPER");
WEAPON_CANNON.damageTable[idx][1] = 85;

// supporter
idx = getIndexOf1(WEAPON_CANNON, "APC");
WEAPON_CANNON.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_CANNON, "FLARE");
WEAPON_CANNON.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_CANNON, "RECON");
WEAPON_CANNON.damageTable[idx][1] = 80;

// tanks
idx = getIndexOf1(WEAPON_CANNON, "FLAK");
WEAPON_CANNON.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_CANNON, "HOVERFLAK");
WEAPON_CANNON.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_CANNON, "LIGHT_TANK");
WEAPON_CANNON.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_CANNON, "HOVERCRAFT");
WEAPON_CANNON.damageTable[idx][1] = 70;

// heavy tanks
idx = getIndexOf1(WEAPON_CANNON, "HEAVY_HOVERCRAFT");
WEAPON_CANNON.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_CANNON, "HEAVY_TANK");
WEAPON_CANNON.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_CANNON, "NEOTANK");
WEAPON_CANNON.damageTable[idx][1] = 40;

// very heavy tanks
idx = getIndexOf1(WEAPON_CANNON, "MEGATANK");
WEAPON_CANNON.damageTable[idx][1] = 15;

// ranged land units
idx = getIndexOf1(WEAPON_CANNON, "ARTILLERY");
WEAPON_CANNON.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_CANNON, "ANTITANKCANNON");
WEAPON_CANNON.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_CANNON, "MISSILE");
WEAPON_CANNON.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_CANNON, "ROCKETTHROWER");
WEAPON_CANNON.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_CANNON, "PIPERUNNER");
WEAPON_CANNON.damageTable[idx][1] = 70;

idx = getIndexOf1(WEAPON_CANNON, "HOELLIUM");
WEAPON_CANNON.damageTable[idx][1] = 5;

// ships
idx = getIndexOf1(WEAPON_CANNON, "BATTLESHIP");
WEAPON_CANNON.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_CANNON, "CANNONBOAT");
WEAPON_CANNON.damageTable[idx][1] = 60;
idx = getIndexOf1(WEAPON_CANNON, "CRUISER");
WEAPON_CANNON.damageTable[idx][1] = 60;
idx = getIndexOf1(WEAPON_CANNON, "DESTROYER");
WEAPON_CANNON.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_CANNON, "SUBMARINE");
WEAPON_CANNON.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_CANNON, "LANDER");
WEAPON_CANNON.damageTable[idx][1] = 60;
idx = getIndexOf1(WEAPON_CANNON, "BLACK_BOAT");
WEAPON_CANNON.damageTable[idx][1] = 60;
idx = getIndexOf1(WEAPON_CANNON, "AIRCRAFTCARRIER");
WEAPON_CANNON.damageTable[idx][1] = 60;
