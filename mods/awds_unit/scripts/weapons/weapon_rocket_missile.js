WEAPON_ROCKET_MISSILE.getEnviromentDamage = function(enviromentId)
{
    return 55;
};
var idx = 0;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE.damageTable, "INFANTRY");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "MECH");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "MOTORBIKE");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "SNIPER");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 95;

// supporter
idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "APC");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "FLARE");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "RECON");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 90;

// tanks
idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "FLAK");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "HOVERFLAK");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "LIGHT_TANK");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "HOVERCRAFT");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 80;

// heavy tanks
idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "HEAVY_HOVERCRAFT");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "HEAVY_TANK");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "NEOTANK");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 50;

// very heavy tanks
idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "MEGATANK");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 25;

idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "HOELLIUM");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 15;

// ranged land units
idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "ARTILLERY");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "ANTITANKCANNON");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "MISSILE");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "ROCKETTHROWER");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "PIPERUNNER");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 80;

// ships
idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "BATTLESHIP");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "CANNONBOAT");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 60;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "CRUISER");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 60;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "DESTROYER");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 60;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "SUBMARINE");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "LANDER");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 60;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "BLACK_BOAT");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 60;
idx = getIndexOf1(WEAPON_ROCKET_MISSILE, "AIRCRAFTCARRIER");
WEAPON_ROCKET_MISSILE.damageTable[idx][1] = 60;
