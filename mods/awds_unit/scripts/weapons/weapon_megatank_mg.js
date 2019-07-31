WEAPON_MEGATANK_MG.getEnviromentDamage = function(enviromentId)
{
    return 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_MEGATANK_MG.damageTable, "INFANTRY");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 135;
idx = getIndexOf1(WEAPON_MEGATANK_MG, "MECH");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 125;
idx = getIndexOf1(WEAPON_MEGATANK_MG, "MOTORBIKE");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 125;
idx = getIndexOf1(WEAPON_MEGATANK_MG, "SNIPER");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 135;

// supporter
idx = getIndexOf1(WEAPON_MEGATANK_MG, "APC");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_MEGATANK_MG, "FLARE");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_MEGATANK_MG, "RECON");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 65;

// tanks
idx = getIndexOf1(WEAPON_MEGATANK_MG, "FLAK");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 17;
idx = getIndexOf1(WEAPON_MEGATANK_MG, "HOVERFLAK");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 17;
idx = getIndexOf1(WEAPON_MEGATANK_MG, "LIGHT_TANK");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_MEGATANK_MG, "HOVERCRAFT");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 10;

// heavy tanks
idx = getIndexOf1(WEAPON_MEGATANK_MG, "HEAVY_HOVERCRAFT");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_MEGATANK_MG, "HEAVY_TANK");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_MEGATANK_MG, "NEOTANK");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 1;

// very heavy tanks
idx = getIndexOf1(WEAPON_MEGATANK_MG, "MEGATANK");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_MEGATANK_MG, "HOELLIUM");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 20;

// heli copter
idx = getIndexOf1(WEAPON_MEGATANK_MG, "T_HELI");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_MEGATANK_MG, "K_HELI");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 22;

// ranged land units
idx = getIndexOf1(WEAPON_MEGATANK_MG, "ARTILLERY");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_MEGATANK_MG, "ANTITANKCANNON");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_MEGATANK_MG, "MISSILE");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_MEGATANK_MG, "ROCKETTHROWER");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_MEGATANK_MG, "PIPERUNNER");
WEAPON_MEGATANK_MG.damageTable[idx][1] = 10;
