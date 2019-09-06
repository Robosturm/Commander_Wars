WEAPON_HEAVY_TANK_MG.getEnviromentDamage = function(enviromentId)
{
    return 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG.damageTable, "INFANTRY");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "MECH");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "MOTORBIKE");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "SNIPER");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 95;

// supporter
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "APC");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "FLARE");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "RECON");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 45;

// tanks
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "FLAK");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 7;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "HOVERFLAK");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 7;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "LIGHT_TANK");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "HOVERCRAFT");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 8;

// heavy tanks
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "HEAVY_TANK");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "NEOTANK");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 1;

// very heavy tanks
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "MEGATANK");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "HOELLIUM");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 20;

// heli copter
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "T_HELI");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "K_HELI");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 12;

// ranged land units
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "ARTILLERY");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "ANTITANKCANNON");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "MISSILE");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "ROCKETTHROWER");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_HEAVY_TANK_GUN.damageTable, "PIPERUNNER");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 8;
