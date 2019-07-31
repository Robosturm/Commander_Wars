WEAPON_INFANTRY_MG.getEnviromentDamage = function(enviromentId)
{
    return 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_INFANTRY_MG.damageTable, "INFANTRY");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_INFANTRY_MG, "MECH");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_INFANTRY_MG, "MOTORBIKE");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_INFANTRY_MG, "SNIPER");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 55;

// supporter
idx = getIndexOf1(WEAPON_INFANTRY_MG, "APC");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 14;
idx = getIndexOf1(WEAPON_INFANTRY_MG, "FLARE");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_INFANTRY_MG, "RECON");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 12;

// tanks
idx = getIndexOf1(WEAPON_INFANTRY_MG, "FLAK");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 5;
idx = getIndexOf1(WEAPON_INFANTRY_MG, "HOVERFLAK");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 5;
idx = getIndexOf1(WEAPON_INFANTRY_MG, "LIGHT_TANK");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 5;
idx = getIndexOf1(WEAPON_INFANTRY_MG, "HOVERCRAFT");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 5;

// heavy tanks
idx = getIndexOf1(WEAPON_INFANTRY_MG, "HEAVY_HOVERCRAFT");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_INFANTRY_MG, "HEAVY_TANK");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_INFANTRY_MG, "NEOTANK");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 1;

// very heavy tanks
idx = getIndexOf1(WEAPON_INFANTRY_MG, "MEGATANK");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_INFANTRY_MG, "HOELLIUM");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 20;

// heli copter
idx = getIndexOf1(WEAPON_INFANTRY_MG, "T_HELI");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 30;
idx = getIndexOf1(WEAPON_INFANTRY_MG, "K_HELI");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 7;

// ranged land units
idx = getIndexOf1(WEAPON_INFANTRY_MG, "ARTILLERY");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 15;
idx = getIndexOf1(WEAPON_INFANTRY_MG, "ANTITANKCANNON");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_INFANTRY_MG, "MISSILE");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 25;
idx = getIndexOf1(WEAPON_INFANTRY_MG, "ROCKETTHROWER");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 25;
idx = getIndexOf1(WEAPON_INFANTRY_MG, "PIPERUNNER");
WEAPON_INFANTRY_MG.damageTable[idx][1] = 5;
