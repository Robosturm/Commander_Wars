WEAPON_SNIPER.getEnviromentDamage = function(enviromentId)
{
    return 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_SNIPER.damageTable, "INFANTRY");
WEAPON_SNIPER.damageTable[idx][1] = 150;
idx = getIndexOf1(WEAPON_SNIPER.damageTable, "MECH");
WEAPON_SNIPER.damageTable[idx][1] = 150;
idx = getIndexOf1(WEAPON_SNIPER.damageTable, "MOTORBIKE");
WEAPON_SNIPER.damageTable[idx][1] = 150;
idx = getIndexOf1(WEAPON_SNIPER.damageTable, "SNIPER");
WEAPON_SNIPER.damageTable[idx][1] = 150;

// supporter
idx = getIndexOf1(WEAPON_SNIPER.damageTable, "APC");
WEAPON_SNIPER.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_SNIPER.damageTable, "FLARE");
WEAPON_SNIPER.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_SNIPER.damageTable, "RECON");
WEAPON_SNIPER.damageTable[idx][1] = 35;

// tanks
idx = getIndexOf1(WEAPON_SNIPER.damageTable, "FLAK");
WEAPON_SNIPER.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_SNIPER.damageTable, "HOVERFLAK");
WEAPON_SNIPER.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_SNIPER.damageTable, "LIGHT_TANK");
WEAPON_SNIPER.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_SNIPER.damageTable, "HOVERCRAFT");
WEAPON_SNIPER.damageTable[idx][1] = 1;

// heavy tanks
idx = getIndexOf1(WEAPON_SNIPER.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_SNIPER.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_SNIPER.damageTable, "HEAVY_TANK");
WEAPON_SNIPER.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_SNIPER.damageTable, "NEOTANK");
WEAPON_SNIPER.damageTable[idx][1] = 1;

// very heavy tanks
idx = getIndexOf1(WEAPON_SNIPER.damageTable, "MEGATANK");
WEAPON_SNIPER.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_SNIPER.damageTable, "HOELLIUM");
WEAPON_SNIPER.damageTable[idx][1] = 20;

// heli copter
idx = getIndexOf1(WEAPON_SNIPER.damageTable, "T_HELI");
WEAPON_SNIPER.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_SNIPER.damageTable, "K_HELI");
WEAPON_SNIPER.damageTable[idx][1] = 9;

// ranged land units
idx = getIndexOf1(WEAPON_SNIPER.damageTable, "ARTILLERY");
WEAPON_SNIPER.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_SNIPER.damageTable, "ANTITANKCANNON");
WEAPON_SNIPER.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_SNIPER.damageTable, "MISSILE");
WEAPON_SNIPER.damageTable[idx][1] = 12;
idx = getIndexOf1(WEAPON_SNIPER.damageTable, "ROCKETTHROWER");
WEAPON_SNIPER.damageTable[idx][1] = 12;
idx = getIndexOf1(WEAPON_SNIPER.damageTable, "PIPERUNNER");
WEAPON_SNIPER.damageTable[idx][1] = 6;
