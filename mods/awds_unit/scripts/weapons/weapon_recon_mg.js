WEAPON_RECON_MG.getEnviromentDamage = function(enviromentId)
{
    return 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_RECON_MG.damageTable, "INFANTRY");
WEAPON_RECON_MG.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_RECON_MG, "MECH");
WEAPON_RECON_MG.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_RECON_MG, "MOTORBIKE");
WEAPON_RECON_MG.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_RECON_MG, "SNIPER");
WEAPON_RECON_MG.damageTable[idx][1] = 70;

// supporter
idx = getIndexOf1(WEAPON_RECON_MG, "APC");
WEAPON_RECON_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_RECON_MG, "FLARE");
WEAPON_RECON_MG.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_RECON_MG, "RECON");
WEAPON_RECON_MG.damageTable[idx][1] = 35;

// tanks
idx = getIndexOf1(WEAPON_RECON_MG, "FLAK");
WEAPON_RECON_MG.damageTable[idx][1] = 4;
idx = getIndexOf1(WEAPON_RECON_MG, "HOVERFLAK");
WEAPON_RECON_MG.damageTable[idx][1] = 4;
idx = getIndexOf1(WEAPON_RECON_MG, "LIGHT_TANK");
WEAPON_RECON_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_RECON_MG, "HOVERCRAFT");
WEAPON_RECON_MG.damageTable[idx][1] = 1;

// heavy tanks
idx = getIndexOf1(WEAPON_RECON_MG, "HEAVY_HOVERCRAFT");
WEAPON_RECON_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_RECON_MG, "HEAVY_TANK");
WEAPON_RECON_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_RECON_MG, "NEOTANK");
WEAPON_RECON_MG.damageTable[idx][1] = 1;

// very heavy tanks
idx = getIndexOf1(WEAPON_RECON_MG, "MEGATANK");
WEAPON_RECON_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_RECON_MG, "HOELLIUM");
WEAPON_RECON_MG.damageTable[idx][1] = 20;

// heli copter
idx = getIndexOf1(WEAPON_RECON_MG, "T_HELI");
WEAPON_RECON_MG.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_RECON_MG, "K_HELI");
WEAPON_RECON_MG.damageTable[idx][1] = 18;

// ranged land units
idx = getIndexOf1(WEAPON_RECON_MG, "ARTILLERY");
WEAPON_RECON_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_RECON_MG, "ANTITANKCANNON");
WEAPON_RECON_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_RECON_MG, "MISSILE");
WEAPON_RECON_MG.damageTable[idx][1] = 25;
idx = getIndexOf1(WEAPON_RECON_MG, "ROCKETTHROWER");
WEAPON_RECON_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_RECON_MG, "PIPERUNNER");
WEAPON_RECON_MG.damageTable[idx][1] = 6;
