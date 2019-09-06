WEAPON_STEALTHROCKET.getEnviromentDamage = function(enviromentId)
{
    return 70;
};
var idx = 0;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "INFANTRY");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "MECH");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "MOTORBIKE");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "SNIPER");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 90;

// supporter
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "APC");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "FLARE");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "RECON");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 85;

// tanks
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "FLAK");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 50;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "HOVERFLAK");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 50;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "LIGHT_TANK");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "HOVERCRAFT");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 75;

// heavy tanks
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "HEAVY_TANK");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "NEOTANK");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 60;

// very heavy tanks
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "MEGATANK");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 25;

idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "HOELLIUM");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 30;

// ranged land units
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "ARTILLERY");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "ANTITANKCANNON");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "MISSILE");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "ROCKETTHROWER");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "PIPERUNNER");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 80;

// air units
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "DUSTER");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "FIGHTER");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "BOMBER");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "STEALTHBOMBER");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "TRANSPORTPLANE");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 120;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "BLACK_BOMB");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 120;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "WATERPLANE");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "K_HELI");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 115;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "T_HELI");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 115;


// ships
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "BATTLESHIP");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "CANNONBOAT");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 60;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "CRUISER");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "DESTROYER");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "SUBMARINE");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "LANDER");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 60;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "BLACK_BOAT");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 60;
idx = getIndexOf1(WEAPON_STEALTHROCKET.damageTable, "AIRCRAFTCARRIER");
WEAPON_STEALTHROCKET.damageTable[idx][1] = 60;
