WEAPON_HOVERVULCAN_CANNON.getEnviromentDamage = function(enviromentId)
{
    return 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "INFANTRY");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "MECH");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "MOTORBIKE");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "SNIPER");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 105;

// supporter
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "APC");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 50;
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "FLARE");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "RECON");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 60;

// tanks
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "FLAK");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "HOVERFLAK");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "LIGHT_TANK");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 25;
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "HOVERCRAFT");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 25;

// heavy tanks
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "HEAVY_TANK");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "NEOTANK");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 5;

// very heavy tanks
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "MEGATANK");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "HOELLIUM");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 30;

// heli copter
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "T_HELI");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "K_HELI");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 105;

//air
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "DUSTER");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "FIGHTER");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "BOMBER");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "STEALTHBOMBER");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "BLACK_BOMB");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 120;
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "TRANSPORTPLANE");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 120;
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "WATERPLANE");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 75;

// ranged land units
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "ARTILLERY");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 50;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 50;
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "ANTITANKCANNON");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 25;
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "MISSILE");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "ROCKETTHROWER");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_HOVERVULCAN_CANNON.damageTable, "PIPERUNNER");
WEAPON_HOVERVULCAN_CANNON.damageTable[idx][1] = 25;
