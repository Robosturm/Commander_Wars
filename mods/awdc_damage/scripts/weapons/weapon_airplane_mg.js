WEAPON_AIRPLANE_MG.getEnviromentDamage = function(enviromentId)
{
    return 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "INFANTRY");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "MECH");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "MOTORBIKE");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "SNIPER");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 45;

// supporter
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "APC");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 15;
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "FLARE");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 15;
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "RECON");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 18;

// tanks
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "FLAK");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 5;
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "HOVERFLAK");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 5;
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "LIGHT_TANK");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "HOVERCRAFT");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 8;

// heavy tanks
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 5;
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "HEAVY_TANK");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 5;
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "NEOTANK");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 5;

// very heavy tanks
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "MEGATANK");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "HOELLIUM");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 20;

// ranged land units
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "ARTILLERY");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 15;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 15;
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "ANTITANKCANNON");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 5;
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "MISSILE");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 20;
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "ROCKETTHROWER");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 20;

idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "PIPERUNNER");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 6;

// air
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "DUSTER");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "FIGHTER");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 40;
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "BOMBER");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "STEALTHBOMBER");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "TRANSPORTPLANE");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "BLACK_BOMB");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "WATERPLANE");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "K_HELI");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_AIRPLANE_MG.damageTable, "T_HELI");
WEAPON_AIRPLANE_MG.damageTable[idx][1] = 95;
