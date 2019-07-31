WEAPON_SNIPER.getEnviromentDamage = function(enviromentId)
{
    return 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_SNIPER.damageTable, "INFANTRY");
WEAPON_SNIPER.damageTable[idx][1] = 150;
idx = getIndexOf1(WEAPON_SNIPER, "MECH");
WEAPON_SNIPER.damageTable[idx][1] = 150;
idx = getIndexOf1(WEAPON_SNIPER, "MOTORBIKE");
WEAPON_SNIPER.damageTable[idx][1] = 150;
idx = getIndexOf1(WEAPON_SNIPER, "SNIPER");
WEAPON_SNIPER.damageTable[idx][1] = 150;

// supporter
idx = getIndexOf1(WEAPON_SNIPER, "APC");
WEAPON_SNIPER.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_SNIPER, "FLARE");
WEAPON_SNIPER.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_SNIPER, "RECON");
WEAPON_SNIPER.damageTable[idx][1] = 35;

// tanks
idx = getIndexOf1(WEAPON_SNIPER, "FLAK");
WEAPON_SNIPER.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_SNIPER, "HOVERFLAK");
WEAPON_SNIPER.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_SNIPER, "LIGHT_TANK");
WEAPON_SNIPER.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_SNIPER, "HOVERCRAFT");
WEAPON_SNIPER.damageTable[idx][1] = 1;

// heavy tanks
idx = getIndexOf1(WEAPON_SNIPER, "HEAVY_HOVERCRAFT");
WEAPON_SNIPER.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_SNIPER, "HEAVY_TANK");
WEAPON_SNIPER.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_SNIPER, "NEOTANK");
WEAPON_SNIPER.damageTable[idx][1] = 1;

// very heavy tanks
idx = getIndexOf1(WEAPON_SNIPER, "MEGATANK");
WEAPON_SNIPER.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_SNIPER, "HOELLIUM");
WEAPON_SNIPER.damageTable[idx][1] = 20;

// heli copter
idx = getIndexOf1(WEAPON_SNIPER, "T_HELI");
WEAPON_SNIPER.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_SNIPER, "K_HELI");
WEAPON_SNIPER.damageTable[idx][1] = 9;

// ranged land units
idx = getIndexOf1(WEAPON_SNIPER, "ARTILLERY");
WEAPON_SNIPER.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_SNIPER, "ANTITANKCANNON");
WEAPON_SNIPER.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_SNIPER, "MISSILE");
WEAPON_SNIPER.damageTable[idx][1] = 12;
idx = getIndexOf1(WEAPON_SNIPER, "ROCKETTHROWER");
WEAPON_SNIPER.damageTable[idx][1] = 12;
idx = getIndexOf1(WEAPON_SNIPER, "PIPERUNNER");
WEAPON_SNIPER.damageTable[idx][1] = 6;
