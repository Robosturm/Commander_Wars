WEAPON_RECON_MG.getEnviromentDamage = function(enviromentId)
{
    return 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_RECON_MG.damageTable, "INFANTRY");
WEAPON_RECON_MG.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_RECON_MG.damageTable, "MECH");
WEAPON_RECON_MG.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_RECON_MG.damageTable, "MOTORBIKE");
WEAPON_RECON_MG.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_RECON_MG.damageTable, "SNIPER");
WEAPON_RECON_MG.damageTable[idx][1] = 65;

// supporter
idx = getIndexOf1(WEAPON_RECON_MG.damageTable, "APC");
WEAPON_RECON_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_RECON_MG.damageTable, "FLARE");
WEAPON_RECON_MG.damageTable[idx][1] = 30;
idx = getIndexOf1(WEAPON_RECON_MG.damageTable, "RECON");
WEAPON_RECON_MG.damageTable[idx][1] = 35;

// tanks
idx = getIndexOf1(WEAPON_RECON_MG.damageTable, "FLAK");
WEAPON_RECON_MG.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_RECON_MG.damageTable, "HOVERFLAK");
WEAPON_RECON_MG.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_RECON_MG.damageTable, "LIGHT_TANK");
WEAPON_RECON_MG.damageTable[idx][1] = 8;
idx = getIndexOf1(WEAPON_RECON_MG.damageTable, "HOVERCRAFT");
WEAPON_RECON_MG.damageTable[idx][1] = 8;

// heavy tanks
idx = getIndexOf1(WEAPON_RECON_MG.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_RECON_MG.damageTable[idx][1] = 5;
idx = getIndexOf1(WEAPON_RECON_MG.damageTable, "HEAVY_TANK");
WEAPON_RECON_MG.damageTable[idx][1] = 5;
idx = getIndexOf1(WEAPON_RECON_MG.damageTable, "NEOTANK");
WEAPON_RECON_MG.damageTable[idx][1] = 5;

// very heavy tanks
idx = getIndexOf1(WEAPON_RECON_MG.damageTable, "MEGATANK");
WEAPON_RECON_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_RECON_MG.damageTable, "HOELLIUM");
WEAPON_RECON_MG.damageTable[idx][1] = 20;

// heli copter
idx = getIndexOf1(WEAPON_RECON_MG.damageTable, "T_HELI");
WEAPON_RECON_MG.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_RECON_MG.damageTable, "K_HELI");
WEAPON_RECON_MG.damageTable[idx][1] = 18;

// ranged land units
idx = getIndexOf1(WEAPON_RECON_MG.damageTable, "ARTILLERY");
WEAPON_RECON_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_RECON_MG.damageTable, "ANTITANKCANNON");
WEAPON_RECON_MG.damageTable[idx][1] = 25;
idx = getIndexOf1(WEAPON_RECON_MG.damageTable, "MISSILE");
WEAPON_RECON_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_RECON_MG.damageTable, "ROCKETTHROWER");
WEAPON_RECON_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_RECON_MG.damageTable, "PIPERUNNER");
WEAPON_RECON_MG.damageTable[idx][1] = 6;
