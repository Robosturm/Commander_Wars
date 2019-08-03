WEAPON_MEGATANK_MG.getEnviromentDamage = function(enviromentId)
{
    WEAPON_MEGATANK_MG.damageTable[idx][1] = 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_MEGATANK_MG.damageTable, "INFANTRY");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_MEGATANK_MG.damageTable, "MECH");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_MEGATANK_MG.damageTable, "MOTORBIKE");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_MEGATANK_MG.damageTable, "SNIPER");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 95;

// supporter
idx = getIndexOf1(WEAPON_MEGATANK_MG.damageTable, "APC");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_MEGATANK_MG.damageTable, "FLARE");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 40;
idx = getIndexOf1(WEAPON_MEGATANK_MG.damageTable, "RECON");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 45;

// tanks
idx = getIndexOf1(WEAPON_MEGATANK_MG.damageTable, "FLAK");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_MEGATANK_MG.damageTable, "HOVERFLAK");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_MEGATANK_MG.damageTable, "LIGHT_TANK");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_MEGATANK_MG.damageTable, "HOVERCRAFT");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 10;

// heavy tanks
idx = getIndexOf1(WEAPON_MEGATANK_MG.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_MEGATANK_MG.damageTable, "HEAVY_TANK");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_MEGATANK_MG.damageTable, "NEOTANK");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 10;

// very heavy tanks
idx = getIndexOf1(WEAPON_MEGATANK_MG.damageTable, "MEGATANK");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_MEGATANK_MG.damageTable, "HOELLIUM");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 20;

// heli copter
idx = getIndexOf1(WEAPON_MEGATANK_MG.damageTable, "T_HELI");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_MEGATANK_MG.damageTable, "K_HELI");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 35;

// ranged land units
idx = getIndexOf1(WEAPON_MEGATANK_MG.damageTable, "ARTILLERY");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_MEGATANK_MG.damageTable, "ANTITANKCANNON");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_MEGATANK_MG.damageTable, "MISSILE");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_MEGATANK_MG.damageTable, "ROCKETTHROWER");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_MEGATANK_MG.damageTable, "PIPERUNNER");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 6;
