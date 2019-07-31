WEAPON_A_TANK_CANNON.getEnviromentDamage = function(enviromentId)
{
    return 55;
};
var idx = 0;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "INFANTRY");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "MECH");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "MOTORBIKE");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "SNIPER");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 65;
// supporter
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "APC");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "FLARE");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "RECON");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 75;
// tanks
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "FLAK");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "HOVERFLAK");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "LIGHT_TANK");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "HOVERCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 75;
// heavy tanks
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "HEAVY_TANK");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "NEOTANK");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 65;
// very heavy tanks
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "MEGATANK");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "HOELLIUM");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 15;
// heli copter
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "T_HELI");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "K_HELI");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 45;
// ranged land units
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERY");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ANTITANKCANNON");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "MISSILE");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ROCKETTHROWER");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "PIPERUNNER");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 70;
