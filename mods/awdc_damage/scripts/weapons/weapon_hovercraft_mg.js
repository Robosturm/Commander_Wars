WEAPON_HOVERCRAFT_MG.getEnviromentDamage = function(enviromentId)
{
    return 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG.damageTable, "INFANTRY");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG.damageTable, "MECH");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG.damageTable, "MOTORBIKE");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG.damageTable, "SNIPER");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 70;

// supporter
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG.damageTable, "APC");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG.damageTable, "FLARE");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG.damageTable, "RECON");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 40;

// tanks
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG.damageTable, "FLAK");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG.damageTable, "HOVERFLAK");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG.damageTable, "LIGHT_TANK");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG.damageTable, "HOVERCRAFT");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 8;

// heavy tanks
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 5;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG.damageTable, "HEAVY_TANK");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 5;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG.damageTable, "NEOTANK");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 5;

// very heavy tanks
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG.damageTable, "MEGATANK");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_HOVERCRAFT_MG.damageTable, "HOELLIUM");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 20;

// heli copter
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG.damageTable, "T_HELI");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 40;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG.damageTable, "K_HELI");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 18;

// ranged land units
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG.damageTable, "ARTILLERY");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG.damageTable, "ANTITANKCANNON");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG.damageTable, "MISSILE");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG.damageTable, "ROCKETTHROWER");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG.damageTable, "PIPERUNNER");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 6;
