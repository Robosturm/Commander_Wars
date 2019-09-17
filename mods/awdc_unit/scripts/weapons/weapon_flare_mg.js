WEAPON_FLARE_MG.getEnviromentDamage = function(enviromentId)
{
    return 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_FLARE_MG.damageTable, "INFANTRY");
WEAPON_FLARE_MG.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_FLARE_MG.damageTable, "MECH");
WEAPON_FLARE_MG.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_FLARE_MG.damageTable, "MOTORBIKE");
WEAPON_FLARE_MG.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_FLARE_MG.damageTable, "SNIPER");
WEAPON_FLARE_MG.damageTable[idx][1] = 70;

// supporter
idx = getIndexOf1(WEAPON_FLARE_MG.damageTable, "APC");
WEAPON_FLARE_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_FLARE_MG.damageTable, "FLARE");
WEAPON_FLARE_MG.damageTable[idx][1] = 50;
idx = getIndexOf1(WEAPON_FLARE_MG.damageTable, "RECON");
WEAPON_FLARE_MG.damageTable[idx][1] = 60;

// tanks
idx = getIndexOf1(WEAPON_FLARE_MG.damageTable, "FLAK");
WEAPON_FLARE_MG.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_FLARE_MG.damageTable, "HOVERFLAK");
WEAPON_FLARE_MG.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_FLARE_MG.damageTable, "LIGHT_TANK");
WEAPON_FLARE_MG.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_FLARE_MG.damageTable, "HOVERCRAFT");
WEAPON_FLARE_MG.damageTable[idx][1] = 10;

// heavy tanks
idx = getIndexOf1(WEAPON_FLARE_MG.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_FLARE_MG.damageTable[idx][1] = 5;
idx = getIndexOf1(WEAPON_FLARE_MG.damageTable, "HEAVY_TANK");
WEAPON_FLARE_MG.damageTable[idx][1] = 5;
idx = getIndexOf1(WEAPON_FLARE_MG.damageTable, "NEOTANK");
WEAPON_FLARE_MG.damageTable[idx][1] = 5;

// very heavy tanks
idx = getIndexOf1(WEAPON_FLARE_MG.damageTable, "MEGATANK");
WEAPON_FLARE_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_FLARE_MG.damageTable, "HOELLIUM");
WEAPON_FLARE_MG.damageTable[idx][1] = 20;

// heli copter
idx = getIndexOf1(WEAPON_FLARE_MG.damageTable, "T_HELI");
WEAPON_FLARE_MG.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_FLARE_MG.damageTable, "K_HELI");
WEAPON_FLARE_MG.damageTable[idx][1] = 18;

// ranged land units
idx = getIndexOf1(WEAPON_FLARE_MG.damageTable, "ARTILLERY");
WEAPON_FLARE_MG.damageTable[idx][1] = 50;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 50;
idx = getIndexOf1(WEAPON_FLARE_MG.damageTable, "ANTITANKCANNON");
WEAPON_FLARE_MG.damageTable[idx][1] = 25;
idx = getIndexOf1(WEAPON_FLARE_MG.damageTable, "MISSILE");
WEAPON_FLARE_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_FLARE_MG.damageTable, "ROCKETTHROWER");
WEAPON_FLARE_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_FLARE_MG.damageTable, "PIPERUNNER");
WEAPON_FLARE_MG.damageTable[idx][1] = 6;
