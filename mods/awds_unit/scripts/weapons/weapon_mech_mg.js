WEAPON_MECH_MG.getEnviromentDamage = function(enviromentId)
{
    return 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_MECH_MG.damageTable, "INFANTRY");
WEAPON_MECH_MG.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_MECH_MG, "MECH");
WEAPON_MECH_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_MECH_MG, "MOTORBIKE");
WEAPON_MECH_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_MECH_MG, "SNIPER");
WEAPON_MECH_MG.damageTable[idx][1] = 75;

idx = getIndexOf1(WEAPON_MECH_MG, "APC");
WEAPON_MECH_MG.damageTable[idx][1] = 20;
idx = getIndexOf1(WEAPON_MECH_MG, "FLARE");
WEAPON_MECH_MG.damageTable[idx][1] = 18;
idx = getIndexOf1(WEAPON_MECH_MG, "RECON");
WEAPON_MECH_MG.damageTable[idx][1] = 18;

idx = getIndexOf1(WEAPON_MECH_MG, "FLAK");
WEAPON_MECH_MG.damageTable[idx][1] = 6;
idx = getIndexOf1(WEAPON_MECH_MG, "HOVERFLAK");
WEAPON_MECH_MG.damageTable[idx][1] = 6;
idx = getIndexOf1(WEAPON_MECH_MG, "LIGHT_TANK");
WEAPON_MECH_MG.damageTable[idx][1] = 6;
idx = getIndexOf1(WEAPON_MECH_MG, "HOVERCRAFT");
WEAPON_MECH_MG.damageTable[idx][1] = 6;

idx = getIndexOf1(WEAPON_MECH_MG, "HEAVY_HOVERCRAFT");
WEAPON_MECH_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_MECH_MG, "HEAVY_TANK");
WEAPON_MECH_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_MECH_MG, "NEOTANK");
WEAPON_MECH_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_MECH_MG, "MEGATANK");
WEAPON_MECH_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_MECH_MG, "HOELLIUM");
WEAPON_MECH_MG.damageTable[idx][1] = 20;

idx = getIndexOf1(WEAPON_MECH_MG, "T_HELI");
WEAPON_MECH_MG.damageTable[idx][1] = 30;
idx = getIndexOf1(WEAPON_MECH_MG, "K_HELI");
WEAPON_MECH_MG.damageTable[idx][1] = 10;

idx = getIndexOf1(WEAPON_MECH_MG, "ARTILLERY");
WEAPON_MECH_MG.damageTable[idx][1] = 32;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 32;
idx = getIndexOf1(WEAPON_MECH_MG, "ANTITANKCANNON");
WEAPON_MECH_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_MECH_MG, "MISSILE");
WEAPON_MECH_MG.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_MECH_MG, "ROCKETTHROWER");
WEAPON_MECH_MG.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_MECH_MG, "PIPERUNNER");
WEAPON_MECH_MG.damageTable[idx][1] = 6;
