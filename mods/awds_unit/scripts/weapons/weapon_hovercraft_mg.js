WEAPON_HOVERCRAFT_MG.getEnviromentDamage = function(enviromentId)
{
    return 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG.damageTable, "INFANTRY");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG, "MECH");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG, "MOTORBIKE");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG, "SNIPER");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 70;

// supporter
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG, "APC");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG, "FLARE");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG, "RECON");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 35;

// tanks
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG, "FLAK");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 6;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG, "HOVERFLAK");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 6;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG, "LIGHT_TANK");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 6;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG, "HOVERCRAFT");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 6;

// heavy tanks
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG, "HEAVY_HOVERCRAFT");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG, "HEAVY_TANK");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG, "NEOTANK");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 1;

// very heavy tanks
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG, "MEGATANK");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_HOVERCRAFT_MG, "HOELLIUM");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 20;

// heli copter
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG, "T_HELI");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 40;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG, "K_HELI");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 10;

// ranged land units
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG, "ARTILLERY");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG, "ANTITANKCANNON");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG, "MISSILE");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG, "ROCKETTHROWER");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_HOVERCRAFT_MG, "PIPERUNNER");
WEAPON_HOVERCRAFT_MG.damageTable[idx][1] = 6;
