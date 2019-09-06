WEAPON_BATTLESHIP_CANNON.getEnviromentDamage = function(enviromentId)
{
    return 55;
};
var idx = 0;
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "INFANTRY");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "MECH");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "MOTORBIKE");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "SNIPER");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 90;

// supporter
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "APC");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 60;
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "FLARE");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 90;
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "RECON");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 90;

// tanks
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "FLAK");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "HOVERFLAK");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "LIGHT_TANK");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "HOVERCRAFT");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 80;

// heavy tanks
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "HEAVY_TANK");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "NEOTANK");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 50;

// very heavy tanks
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "MEGATANK");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 25;

// ranged land units
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "ARTILLERY");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_A_TANK_CANNON.damageTable, "ARTILLERYCRAFT");
WEAPON_A_TANK_CANNON.damageTable[idx][1] = 80;
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "ANTITANKCANNON");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "MISSILE");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "ROCKETTHROWER");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "PIPERUNNER");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 80;

idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "HOELLIUM");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 20;

// ships
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "BATTLESHIP");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 50;
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "CANNONBOAT");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "CRUISER");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "DESTROYER");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "SUBMARINE");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "LANDER");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "BLACK_BOAT");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_BATTLESHIP_CANNON.damageTable, "AIRCRAFTCARRIER");
WEAPON_BATTLESHIP_CANNON.damageTable[idx][1] = 60;
