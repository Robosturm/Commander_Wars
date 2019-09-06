WEAPON_PIPERUNNER_MISSILE.getEnviromentDamage = function(enviromentId)
{
    return 55;
};
var idx = 0;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "INFANTRY");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "MECH");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "MOTORBIKE");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "SNIPER");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 85;

// supporter
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "APC");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "FLARE");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "RECON");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 80;

// tanks
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "FLAK");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "HOVERFLAK");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "LIGHT_TANK");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "HOVERCRAFT");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 65;

// heavy tanks
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "HEAVY_TANK");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "NEOTANK");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 55;

// very heavy tanks
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "MEGATANK");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 35;

idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "HOELLIUM");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 15;

// heli copter
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "T_HELI");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 120;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "K_HELI");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 105;

//air
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "DUSTER");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "FIGHTER");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "BOMBER");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "STEALTHBOMBER");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "TRANSPORTPLANE");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "BLACK_BOMB");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "WATERPLANE");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 75;

// ranged land units
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "ARTILLERY");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "ANTITANKCANNON");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "MISSILE");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "ROCKETTHROWER");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "PIPERUNNER");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 85;

// ships
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "BATTLESHIP");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "CANNONBOAT");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "BLACK_BOAT");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "CRUISER");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "DESTROYER");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "SUBMARINE");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "LANDER");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "AIRCRAFTCARRIER");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 55;
