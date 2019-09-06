WEAPON_INFANTRY_MG.getEnviromentDamage = function(enviromentId)
{
    return 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_INFANTRY_MG.damageTable, "INFANTRY");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_INFANTRY_MG.damageTable, "MECH");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_INFANTRY_MG.damageTable, "MOTORBIKE");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_INFANTRY_MG.damageTable, "SNIPER");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 55;

// supporter
idx = getIndexOf1(WEAPON_INFANTRY_MG.damageTable, "APC");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 14;
idx = getIndexOf1(WEAPON_INFANTRY_MG.damageTable, "FLARE");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_INFANTRY_MG.damageTable, "RECON");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 12;

// tanks
idx = getIndexOf1(WEAPON_INFANTRY_MG.damageTable, "FLAK");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 5;
idx = getIndexOf1(WEAPON_INFANTRY_MG.damageTable, "HOVERFLAK");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 5;
idx = getIndexOf1(WEAPON_INFANTRY_MG.damageTable, "LIGHT_TANK");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 5;
idx = getIndexOf1(WEAPON_INFANTRY_MG.damageTable, "HOVERCRAFT");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 5;

// heavy tanks
idx = getIndexOf1(WEAPON_INFANTRY_MG.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_INFANTRY_MG.damageTable, "HEAVY_TANK");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_INFANTRY_MG.damageTable, "NEOTANK");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 1;

// very heavy tanks
idx = getIndexOf1(WEAPON_INFANTRY_MG.damageTable, "MEGATANK");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_INFANTRY_MG.damageTable, "HOELLIUM");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 20;

// heli copter
idx = getIndexOf1(WEAPON_INFANTRY_MG.damageTable, "T_HELI");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 30;
idx = getIndexOf1(WEAPON_INFANTRY_MG.damageTable, "K_HELI");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 7;

// ranged land units
idx = getIndexOf1(WEAPON_INFANTRY_MG.damageTable, "ARTILLERY");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 15;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 15;
idx = getIndexOf1(WEAPON_INFANTRY_MG.damageTable, "ANTITANKCANNON");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_INFANTRY_MG.damageTable, "MISSILE");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 25;
idx = getIndexOf1(WEAPON_INFANTRY_MG.damageTable, "ROCKETTHROWER");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 25;
idx = getIndexOf1(WEAPON_INFANTRY_MG.damageTable, "PIPERUNNER");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 5;
