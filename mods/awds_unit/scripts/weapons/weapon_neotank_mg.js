WEAPON_NEOTANK_MG.getEnviromentDamage = function(enviromentId)
{
    return 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "INFANTRY");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 125;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "MECH");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 115;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "MOTORBIKE");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 115;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "SNIPER");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 115;

// supporter
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "APC");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "FLARE");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "RECON");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 65;

// tanks
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "FLAK");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 17;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "HOVERFLAK");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 17;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "LIGHT_TANK");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "HOVERCRAFT");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 1;

// heavy tanks
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "HEAVY_TANK");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "NEOTANK");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 1;

// very heavy tanks
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "MEGATANK");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "HOELLIUM");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 20;

// heli copter
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "T_HELI");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "K_HELI");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 22;

// ranged land units
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "ARTILLERY");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "ANTITANKCANNON");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 1;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "MISSILE");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "ROCKETTHROWER");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_NEOTANK_MG.damageTable, "PIPERUNNER");
WEAPON_NEOTANK_MG.damageTable[idx][1] = 10;
