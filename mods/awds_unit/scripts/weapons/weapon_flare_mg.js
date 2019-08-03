WEAPON_FLARE_MG.getEnviromentDamage = function(enviromentId)
{
    WEAPON_FLARE_MG.damageTable[idx][1] = 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_FLARE_MG.damageTable, "INFANTRY");
WEAPON_FLARE_MG.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_FLARE_MG, "MECH");
WEAPON_FLARE_MG.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_FLARE_MG, "MOTORBIKE");
WEAPON_FLARE_MG.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_FLARE_MG, "SNIPER");
WEAPON_FLARE_MG.damageTable[idx][1] = 70;

// supporter
idx = getIndexOf1(WEAPON_FLARE_MG, "APC");
WEAPON_FLARE_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_FLARE_MG, "FLARE");
WEAPON_FLARE_MG.damageTable[idx][1] = 50;
idx = getIndexOf1(WEAPON_FLARE_MG, "RECON");
WEAPON_FLARE_MG.damageTable[idx][1] = 60;

// tanks
idx = getIndexOf1(WEAPON_FLARE_MG, "FLAK");
WEAPON_FLARE_MG.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_FLARE_MG, "HOVERFLAK");
WEAPON_FLARE_MG.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_FLARE_MG, "LIGHT_TANK");
WEAPON_FLARE_MG.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_FLARE_MG, "HOVERCRAFT");
WEAPON_FLARE_MG.damageTable[idx][1] = 10;

// heavy tanks
idx = getIndexOf1(WEAPON_FLARE_MG, "HEAVY_HOVERCRAFT");
WEAPON_FLARE_MG.damageTable[idx][1] = 5;
idx = getIndexOf1(WEAPON_FLARE_MG, "HEAVY_TANK");
WEAPON_FLARE_MG.damageTable[idx][1] = 5;
idx = getIndexOf1(WEAPON_FLARE_MG, "NEOTANK");
WEAPON_FLARE_MG.damageTable[idx][1] = 5;

// very heavy tanks
idx = getIndexOf1(WEAPON_FLARE_MG, "MEGATANK");
WEAPON_FLARE_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_FLARE_MG, "HOELLIUM");
WEAPON_FLARE_MG.damageTable[idx][1] = 20;

// heli copter
idx = getIndexOf1(WEAPON_FLARE_MG, "T_HELI");
WEAPON_FLARE_MG.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_FLARE_MG, "K_HELI");
WEAPON_FLARE_MG.damageTable[idx][1] = 18;

// ranged land units
idx = getIndexOf1(WEAPON_FLARE_MG, "ARTILLERY");
WEAPON_FLARE_MG.damageTable[idx][1] = 50;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 50;
idx = getIndexOf1(WEAPON_FLARE_MG, "ANTITANKCANNON");
WEAPON_FLARE_MG.damageTable[idx][1] = 25;
idx = getIndexOf1(WEAPON_FLARE_MG, "MISSILE");
WEAPON_FLARE_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_FLARE_MG, "ROCKETTHROWER");
WEAPON_FLARE_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_FLARE_MG, "PIPERUNNER");
WEAPON_FLARE_MG.damageTable[idx][1] = 6;
