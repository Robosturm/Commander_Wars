WEAPON_NEOTANK_MG.getEnviromentDamage = function(enviromentId)
{
    return 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "INFANTRY");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 125;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "MECH");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 115;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "MOTORBIKE");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 115;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "SNIPER");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 115;

// supporter
idx = getIndexOf1(WEAPON_NEOTANK_MG, "APC");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "FLARE");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "RECON");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 65;

// tanks
idx = getIndexOf1(WEAPON_NEOTANK_MG, "FLAK");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 17;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "HOVERFLAK");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 17;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "LIGHT_TANK");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "HOVERCRAFT");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 1;

// heavy tanks
idx = getIndexOf1(WEAPON_NEOTANK_MG, "HEAVY_HOVERCRAFT");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "HEAVY_TANK");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "NEOTANK");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 1;

// very heavy tanks
idx = getIndexOf1(WEAPON_NEOTANK_MG, "MEGATANK");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_NEOTANK_MG, "HOELLIUM");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 20;

// heli copter
idx = getIndexOf1(WEAPON_NEOTANK_MG, "T_HELI");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "K_HELI");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 22;

// ranged land units
idx = getIndexOf1(WEAPON_NEOTANK_MG, "ARTILLERY");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "ANTITANKCANNON");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "MISSILE");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "ROCKETTHROWER");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_NEOTANK_MG, "PIPERUNNER");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 10;
