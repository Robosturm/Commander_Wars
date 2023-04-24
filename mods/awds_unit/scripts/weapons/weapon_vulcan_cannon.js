WEAPON_VULCAN_CANNON.getEnviromentDamage = function(enviromentId)
{
    return 10;
};
var idx = 0;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "INFANTRY");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "MECH");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "MOTORBIKE");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "SNIPER");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 105;

// supporter
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "APC");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 50;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "FLARE");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "RECON");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 60;

// tanks
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "FLAK");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "HOVERFLAK");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "LIGHT_TANK");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 25;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "HOVERCRAFT");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 25;

// heavy tanks
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "HEAVY_TANK");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "NEOTANK");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 5;

// very heavy tanks
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "MEGATANK");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "HOELLIUM");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 30;

// heli copter
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "T_HELI");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "K_HELI");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 105;

//air
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "DUSTER");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "FIGHTER");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "BOMBER");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "STEALTHBOMBER");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "BLACK_BOMB");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 120;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "TRANSPORTPLANE");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 120;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "WATERPLANE");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 75;

// ranged land units
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "ARTILLERY");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 50;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 50;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "ANTITANKCANNON");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 25;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "MISSILE");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "ROCKETTHROWER");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "PIPERUNNER");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 25;
