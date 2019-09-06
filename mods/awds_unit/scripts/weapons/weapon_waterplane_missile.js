WEAPON_WATERPLANE_MISSILE.getEnviromentDamage = function(enviromentId)
{
    return 45;
};
var idx = 0;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "INFANTRY");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "MECH");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "MOTORBIKE");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "SNIPER");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 85;

// supporter
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "APC");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "FLARE");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "RECON");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 80;

// tanks
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "FLAK");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "HOVERFLAK");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "LIGHT_TANK");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "HOVERCRAFT");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 75;

// heavy tanks
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "HEAVY_TANK");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "NEOTANK");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 65;

// very heavy tanks
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "MEGATANK");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 55;

idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "HOELLIUM");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 20;

// ranged land units
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "ARTILLERY");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "ANTITANKCANNON");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 50;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "MISSILE");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "ROCKETTHROWER");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "PIPERUNNER");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 70;

idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "DUSTER");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "FIGHTER");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "BOMBER");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "STEALTHBOMBER");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "TRANSPORTPLANE");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "BLACK_BOMB");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "WATERPLANE");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "K_HELI");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "T_HELI");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 95;

// ships
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "BATTLESHIP");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "CANNONBOAT");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "CRUISER");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 40;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "DESTROYER");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 40;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "SUBMARINE");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "LANDER");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_WATERPLANE_MISSILE.damageTable, "AIRCRAFTCARRIER");
WEAPON_WATERPLANE_MISSILE.damageTable[idx][1] = 65;
