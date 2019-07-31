WEAPON_LIGHT_TANK_MG.getEnviromentDamage = function(enviromentId)
{
    return 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG.damageTable, "INFANTRY");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG, "MECH");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG, "MOTORBIKE");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG, "SNIPER");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 70;

// supporter
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG, "APC");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG, "FLARE");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG, "RECON");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 35;

// tanks
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG, "FLAK");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 6;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG, "HOVERFLAK");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 6;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG, "LIGHT_TANK");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 6;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG, "HOVERCRAFT");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 6;

// heavy tanks
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG, "HEAVY_HOVERCRAFT");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG, "HEAVY_TANK");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG, "NEOTANK");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 1;

// very heavy tanks
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG, "MEGATANK");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_LIGHT_TANK_MG, "HOELLIUM");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 20;

// heli copter
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG, "T_HELI");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 40;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG, "K_HELI");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 10;

// ranged land units
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG, "ARTILLERY");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG, "ANTITANKCANNON");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG, "MISSILE");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG, "ROCKETTHROWER");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG, "PIPERUNNER");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 6;
