WEAPON_K_HELI_MG.getEnviromentDamage = function(enviromentId)
{
    return 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_K_HELI_MG.damageTable, "INFANTRY");
WEAPON_K_HELI_MG.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_K_HELI_MG.damageTable, "MECH");
WEAPON_K_HELI_MG.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_K_HELI_MG.damageTable, "MOTORBIKE");
WEAPON_K_HELI_MG.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_K_HELI_MG.damageTable, "SNIPER");
WEAPON_K_HELI_MG.damageTable[idx][1] = 65;

// supporter
idx = getIndexOf1(WEAPON_K_HELI_MG.damageTable, "APC");
WEAPON_K_HELI_MG.damageTable[idx][1] = 20;
idx = getIndexOf1(WEAPON_K_HELI_MG.damageTable, "FLARE");
WEAPON_K_HELI_MG.damageTable[idx][1] = 30;
idx = getIndexOf1(WEAPON_K_HELI_MG.damageTable, "RECON");
WEAPON_K_HELI_MG.damageTable[idx][1] = 30;

// tanks
idx = getIndexOf1(WEAPON_K_HELI_MG.damageTable, "FLAK");
WEAPON_K_HELI_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_K_HELI_MG.damageTable, "HOVERFLAK");
WEAPON_K_HELI_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_K_HELI_MG.damageTable, "LIGHT_TANK");
WEAPON_K_HELI_MG.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_K_HELI_MG.damageTable, "HOVERCRAFT");
WEAPON_K_HELI_MG.damageTable[idx][1] = 8;

// heavy tanks
idx = getIndexOf1(WEAPON_K_HELI_MG.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_K_HELI_MG.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_K_HELI_MG.damageTable, "HEAVY_TANK");
WEAPON_K_HELI_MG.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_K_HELI_MG.damageTable, "NEOTANK");
WEAPON_K_HELI_MG.damageTable[idx][1] = 8;

// very heavy tanks
idx = getIndexOf1(WEAPON_K_HELI_MG.damageTable, "MEGATANK");
WEAPON_K_HELI_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_K_HELI_MG.damageTable, "HOELLIUM");
WEAPON_K_HELI_MG.damageTable[idx][1] = 20;

// heli copter
idx = getIndexOf1(WEAPON_K_HELI_MG.damageTable, "T_HELI");
WEAPON_K_HELI_MG.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_K_HELI_MG.damageTable, "K_HELI");
WEAPON_K_HELI_MG.damageTable[idx][1] = 65;

// ranged land units
idx = getIndexOf1(WEAPON_K_HELI_MG.damageTable, "ARTILLERY");
WEAPON_K_HELI_MG.damageTable[idx][1] = 25;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 25;
idx = getIndexOf1(WEAPON_K_HELI_MG.damageTable, "ANTITANKCANNON");
WEAPON_K_HELI_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_K_HELI_MG.damageTable, "MISSILE");
WEAPON_K_HELI_MG.damageTable[idx][1] = 25;
idx = getIndexOf1(WEAPON_K_HELI_MG.damageTable, "ROCKETTHROWER");
WEAPON_K_HELI_MG.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_K_HELI_MG.damageTable, "PIPERUNNER");
WEAPON_K_HELI_MG.damageTable[idx][1] = 6;
