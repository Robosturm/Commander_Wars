WEAPON_HEAVY_TANK_MG.getEnviromentDamage = function(enviromentId)
{
    return 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG.damageTable, "INFANTRY");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG, "MECH");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG, "MOTORBIKE");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG, "SNIPER");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 95;

// supporter
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG, "APC");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG, "FLARE");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG, "RECON");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 45;

// tanks
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG, "FLAK");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 7;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG, "HOVERFLAK");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 7;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG, "LIGHT_TANK");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG, "HOVERCRAFT");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 8;

// heavy tanks
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG, "HEAVY_HOVERCRAFT");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG, "HEAVY_TANK");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG, "NEOTANK");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 1;

// very heavy tanks
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG, "MEGATANK");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_HEAVY_TANK_MG, "HOELLIUM");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 20;

// heli copter
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG, "T_HELI");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG, "K_HELI");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 12;

// ranged land units
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG, "ARTILLERY");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG, "ANTITANKCANNON");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG, "MISSILE");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG, "ROCKETTHROWER");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG, "PIPERUNNER");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 8;
