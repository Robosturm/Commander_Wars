WEAPON_ROCKET_MISSILE.getEnviromentDamage = function(enviromentId)
{
    return 55;
};
var idx = 0;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "INFANTRY");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "MECH");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "MOTORBIKE");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "SNIPER");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 90;

// supporter
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "APC");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "FLARE");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "RECON");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 90;

// tanks
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "FLAK");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "HOVERFLAK");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "LIGHT_TANK");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "HOVERCRAFT");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 70;

// heavy tanks
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "HEAVY_TANK");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "NEOTANK");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 55;

// very heavy tanks
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "MEGATANK");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 45;

idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "HOELLIUM");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 15;

// ranged land units
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "ARTILLERY");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "ANTITANKCANNON");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "MISSILE");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "ROCKETTHROWER");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "PIPERUNNER");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 85;

// ships
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "BATTLESHIP");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "CANNONBOAT");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "BLACK_BOAT");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "CRUISER");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "DESTROYER");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "SUBMARINE");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "LANDER");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "AIRCRAFTCARRIER");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 45;
