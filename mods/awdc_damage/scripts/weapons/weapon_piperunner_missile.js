WEAPON_PIPERUNNER_MISSILE.getEnviromentDamage = function(enviromentId)
{
    return 55;
};
var idx = 0;
idx = getIndexOf1(WEAPON_PIPERUNNER_MISSILE.damageTable, "INFANTRY");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "MECH");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "MOTORBIKE");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "SNIPER");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 85;

// supporter
idx = getIndexOf1(WEAPON_NEOTANK_MG, "APC");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "FLARE");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "RECON");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 80;

// tanks
idx = getIndexOf1(WEAPON_NEOTANK_MG, "FLAK");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "HOVERFLAK");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "LIGHT_TANK");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "HOVERCRAFT");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 65;

// heavy tanks
idx = getIndexOf1(WEAPON_NEOTANK_MG, "HEAVY_HOVERCRAFT");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "HEAVY_TANK");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "NEOTANK");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 55;

// very heavy tanks
idx = getIndexOf1(WEAPON_NEOTANK_MG, "MEGATANK");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 35;

idx = getIndexOf1(WEAPON_NEOTANK_MG, "HOELLIUM");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 15;

// heli copter
idx = getIndexOf1(WEAPON_NEOTANK_MG, "T_HELI");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 120;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "K_HELI");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 105;

//air
idx = getIndexOf1(WEAPON_NEOTANK_MG, "DUSTER");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "FIGHTER");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "BOMBER");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "STEALTHBOMBER");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "TRANSPORTPLANE");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "BLACK_BOMB");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "WATERPLANE");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 75;

// ranged land units
idx = getIndexOf1(WEAPON_NEOTANK_MG, "ARTILLERY");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "ANTITANKCANNON");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "MISSILE");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "ROCKETTHROWER");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "PIPERUNNER");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 85;

// ships
idx = getIndexOf1(WEAPON_NEOTANK_MG, "BATTLESHIP");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "CANNONBOAT");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "BLACK_BOAT");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "CRUISER");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "DESTROYER");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "SUBMARINE");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "LANDER");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "AIRCRAFTCARRIER");
WEAPON_PIPERUNNER_MISSILE.damageTable[idx][1] = 55;
