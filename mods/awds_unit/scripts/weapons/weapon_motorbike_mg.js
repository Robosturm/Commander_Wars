WEAPON_MOTORBIKE_MG.getEnviromentDamage = function(enviromentId)
{
    return 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG.damageTable, "INFANTRY");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG, "MECH");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG, "MOTORBIKE");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG, "SNIPER");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 75;

idx = getIndexOf1(WEAPON_MOTORBIKE_MG, "APC");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 20;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG, "FLARE");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 18;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG, "RECON");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 18;

idx = getIndexOf1(WEAPON_MOTORBIKE_MG, "FLAK");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 6;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG, "HOVERFLAK");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 6;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG, "LIGHT_TANK");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG, "HOVERCRAFT");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_MOTORBIKE_MG, "HEAVY_HOVERCRAFT");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG, "HEAVY_TANK");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG, "NEOTANK");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_MOTORBIKE_MG, "MEGATANK");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_MOTORBIKE_MG, "HOELLIUM");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 20;

idx = getIndexOf1(WEAPON_MOTORBIKE_MG, "T_HELI");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 30;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG, "K_HELI");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 10;

idx = getIndexOf1(WEAPON_MOTORBIKE_MG, "ARTILLERY");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 32;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG, "ANTITANKCANNON");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG, "MISSILE");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG, "ROCKETTHROWER");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG, "PIPERUNNER");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 6;
