WEAPON_K_HELI_MG.getEnviromentDamage = function(enviromentId)
{
    return 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_K_HELI_MG.damageTable, "INFANTRY");
WEAPON_K_HELI_MG.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_K_HELI_MG, "MECH");
WEAPON_K_HELI_MG.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_K_HELI_MG, "MOTORBIKE");
WEAPON_K_HELI_MG.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_K_HELI_MG, "SNIPER");
WEAPON_K_HELI_MG.damageTable[idx][1] = 75;

// supporter
idx = getIndexOf1(WEAPON_K_HELI_MG, "APC");
WEAPON_K_HELI_MG.damageTable[idx][1] = 20;
idx = getIndexOf1(WEAPON_K_HELI_MG, "FLARE");
WEAPON_K_HELI_MG.damageTable[idx][1] = 30;
idx = getIndexOf1(WEAPON_K_HELI_MG, "RECON");
WEAPON_K_HELI_MG.damageTable[idx][1] = 30;

// tanks
idx = getIndexOf1(WEAPON_K_HELI_MG, "FLAK");
WEAPON_K_HELI_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_K_HELI_MG, "HOVERFLAK");
WEAPON_K_HELI_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_K_HELI_MG, "LIGHT_TANK");
WEAPON_K_HELI_MG.damageTable[idx][1] = 6;
idx = getIndexOf1(WEAPON_K_HELI_MG, "HOVERCRAFT");
WEAPON_K_HELI_MG.damageTable[idx][1] = 6;

// heavy tanks
idx = getIndexOf1(WEAPON_K_HELI_MG, "HEAVY_HOVERCRAFT");
WEAPON_K_HELI_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_K_HELI_MG, "HEAVY_TANK");
WEAPON_K_HELI_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_K_HELI_MG, "NEOTANK");
WEAPON_K_HELI_MG.damageTable[idx][1] = 1;

// very heavy tanks
idx = getIndexOf1(WEAPON_K_HELI_MG, "MEGATANK");
WEAPON_K_HELI_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_K_HELI_MG, "HOELLIUM");
WEAPON_K_HELI_MG.damageTable[idx][1] = 20;

// heli copter
idx = getIndexOf1(WEAPON_K_HELI_MG, "T_HELI");
WEAPON_K_HELI_MG.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_K_HELI_MG, "K_HELI");
WEAPON_K_HELI_MG.damageTable[idx][1] = 65;

// ranged land units
idx = getIndexOf1(WEAPON_K_HELI_MG, "ARTILLERY");
WEAPON_K_HELI_MG.damageTable[idx][1] = 25;
idx = getIndexOf1(WEAPON_K_HELI_MG, "ANTITANKCANNON");
WEAPON_K_HELI_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_K_HELI_MG, "MISSILE");
WEAPON_K_HELI_MG.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_K_HELI_MG, "ROCKETTHROWER");
WEAPON_K_HELI_MG.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_K_HELI_MG, "PIPERUNNER");
WEAPON_K_HELI_MG.damageTable[idx][1] = 6;
