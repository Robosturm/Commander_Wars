WEAPON_MOTORBIKE_MG.getEnviromentDamage = function(enviromentId)
{
    return 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG.damageTable, "INFANTRY");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG.damageTable, "MECH");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG.damageTable, "MOTORBIKE");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG.damageTable, "SNIPER");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 75;

idx = getIndexOf1(WEAPON_MOTORBIKE_MG.damageTable, "APC");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 20;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG.damageTable, "FLARE");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 18;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG.damageTable, "RECON");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 18;

idx = getIndexOf1(WEAPON_MOTORBIKE_MG.damageTable, "FLAK");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 6;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG.damageTable, "HOVERFLAK");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 6;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG.damageTable, "LIGHT_TANK");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG.damageTable, "HOVERCRAFT");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_MOTORBIKE_MG.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG.damageTable, "HEAVY_TANK");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG.damageTable, "NEOTANK");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_MOTORBIKE_MG.damageTable, "MEGATANK");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_MOTORBIKE_MG.damageTable, "HOELLIUM");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 20;

idx = getIndexOf1(WEAPON_MOTORBIKE_MG.damageTable, "T_HELI");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 30;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG.damageTable, "K_HELI");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 10;

idx = getIndexOf1(WEAPON_MOTORBIKE_MG.damageTable, "ARTILLERY");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 32;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 32;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG.damageTable, "ANTITANKCANNON");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG.damageTable, "MISSILE");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG.damageTable, "ROCKETTHROWER");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 35;
idx = getIndexOf1(WEAPON_MOTORBIKE_MG.damageTable, "PIPERUNNER");
WEAPON_MOTORBIKE_MG.damageTable[idx][1] = 6;
