WEAPON_VULCAN_CANNON.getEnviromentDamage = function(enviromentId)
{
    return 1;
};
var idx = 0;
idx = getIndexOf1(WEAPON_VULCAN_CANNON.damageTable, "INFANTRY");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "MECH");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "MOTORBIKE");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "SNIPER");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 105;

// supporter
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "APC");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 50;
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "FLARE");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "RECON");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 60;

// tanks
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "FLAK");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "HOVERFLAK");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 45;
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "LIGHT_TANK");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 25;
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "HOVERCRAFT");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 25;

// heavy tanks
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "HEAVY_HOVERCRAFT");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "HEAVY_TANK");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 10;
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "NEOTANK");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 5;

// very heavy tanks
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "MEGATANK");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 1;

idx = getIndexOf1(WEAPON_VULCAN_CANNON, "HOELLIUM");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 30;

// heli copter
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "T_HELI");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 105;
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "K_HELI");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 105;

//air
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "DUSTER");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "FIGHTER");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "BOMBER");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "STEALTHBOMBER");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "BLACK_BOMB");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 120;
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "TRANSPORTPLANE");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 120;
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "WATERPLANE");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 75;

// ranged land units
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "ARTILLERY");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 50;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 50;
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "ANTITANKCANNON");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 25;
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "MISSILE");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "ROCKETTHROWER");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_VULCAN_CANNON, "PIPERUNNER");
WEAPON_VULCAN_CANNON.damageTable[idx][1] = 25;
