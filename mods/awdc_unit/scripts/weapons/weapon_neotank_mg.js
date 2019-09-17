WEAPON_NEOTANK_MG.getEnviromentDamage = function(enviromentId)
{
    return 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "INFANTRY");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "MECH");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "MOTORBIKE");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "SNIPER");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 80;

// supporter
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "APC");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "FLARE");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "RECON");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 40;

// tanks
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "FLAK");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "HOVERFLAK");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "LIGHT_TANK");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "HOVERCRAFT");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 8;

// heavy tanks
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 5;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "HEAVY_TANK");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 5;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "NEOTANK");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 5;

// very heavy tanks
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "MEGATANK");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "HOELLIUM");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 20;

// heli copter
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "T_HELI");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 40;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "K_HELI");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 24;

// ranged land units
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "ARTILLERY");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "ANTITANKCANNON");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "MISSILE");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 60;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "ROCKETTHROWER");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 60;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "PIPERUNNER");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 6;
