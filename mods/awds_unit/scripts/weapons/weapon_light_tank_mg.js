WEAPON_LIGHT_TANK_MG.getEnviromentDamage = function(enviromentId)
{
    return 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG.damageTable, "INFANTRY");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG.damageTable, "MECH");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG.damageTable, "MOTORBIKE");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG.damageTable, "SNIPER");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 70;

// supporter
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG.damageTable, "APC");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG.damageTable, "FLARE");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG.damageTable, "RECON");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 35;

// tanks
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG.damageTable, "FLAK");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 6;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG.damageTable, "HOVERFLAK");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 6;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG.damageTable, "LIGHT_TANK");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 6;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG.damageTable, "HOVERCRAFT");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 6;

// heavy tanks
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG.damageTable, "HEAVY_TANK");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG.damageTable, "NEOTANK");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 1;

// very heavy tanks
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG.damageTable, "MEGATANK");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_LIGHT_TANK_MG.damageTable, "HOELLIUM");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 20;

// heli copter
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG.damageTable, "T_HELI");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 40;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG.damageTable, "K_HELI");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 10;

// ranged land units
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG.damageTable, "ARTILLERY");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG.damageTable, "ANTITANKCANNON");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG.damageTable, "MISSILE");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG.damageTable, "ROCKETTHROWER");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_LIGHT_TANK_MG.damageTable, "PIPERUNNER");
WEAPON_LIGHT_TANK_MG.damageTable[idx][1] = 6;
