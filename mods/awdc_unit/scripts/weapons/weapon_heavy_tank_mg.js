WEAPON_HEAVY_TANK_MG.getEnviromentDamage = function(enviromentId)
{
    return 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG.damageTable, "INFANTRY");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG.damageTable, "MECH");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG.damageTable, "MOTORBIKE");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG.damageTable, "SNIPER");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 80;

// supporter
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG.damageTable, "APC");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG.damageTable, "FLARE");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG.damageTable, "RECON");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 40;

// tanks
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG.damageTable, "FLAK");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG.damageTable, "HOVERFLAK");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG.damageTable, "LIGHT_TANK");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG.damageTable, "HOVERCRAFT");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 8;

// heavy tanks
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 5;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG.damageTable, "HEAVY_TANK");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 5;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG.damageTable, "NEOTANK");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 5;

// very heavy tanks
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG.damageTable, "MEGATANK");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_HEAVY_TANK_MG.damageTable, "HOELLIUM");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 20;

// heli copter
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG.damageTable, "T_HELI");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 40;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG.damageTable, "K_HELI");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 24;

// ranged land units
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG.damageTable, "ARTILLERY");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG.damageTable, "ANTITANKCANNON");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG.damageTable, "MISSILE");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 60;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG.damageTable, "ROCKETTHROWER");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 60;
idx = getIndexOf1(WEAPON_HEAVY_TANK_MG.damageTable, "PIPERUNNER");
WEAPON_HEAVY_TANK_MG.damageTable[idx][1] = 6;
