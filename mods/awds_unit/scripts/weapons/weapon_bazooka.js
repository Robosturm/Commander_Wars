WEAPON_BAZOOKA.getEnviromentDamage = function(enviromentId)
{
    return 15;
};
var idx = 0;
idx = getIndexOf1(WEAPON_BAZOOKA.damageTable, "APC");
WEAPON_BAZOOKA.damageTable[idx][1] = 75;
idx = getIndexOf1(WEAPON_BAZOOKA.damageTable, "FLARE");
WEAPON_BAZOOKA.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_BAZOOKA.damageTable, "RECON");
WEAPON_BAZOOKA.damageTable[idx][1] = 85;

// tanks
idx = getIndexOf1(WEAPON_BAZOOKA.damageTable, "FLAK");
WEAPON_BAZOOKA.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_BAZOOKA.damageTable, "HOVERFLAK");
WEAPON_BAZOOKA.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_BAZOOKA.damageTable, "LIGHT_TANK");
WEAPON_BAZOOKA.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_BAZOOKA.damageTable, "HOVERCRAFT");
WEAPON_BAZOOKA.damageTable[idx][1] = 55;

// heavy tanks
idx = getIndexOf1(WEAPON_BAZOOKA.damageTable, "HEAVY_HOVERCRAFT");
WEAPON_BAZOOKA.damageTable[idx][1] = 15;
idx = getIndexOf1(WEAPON_BAZOOKA.damageTable, "HEAVY_TANK");
WEAPON_BAZOOKA.damageTable[idx][1] = 15;
idx = getIndexOf1(WEAPON_BAZOOKA.damageTable, "NEOTANK");
WEAPON_BAZOOKA.damageTable[idx][1] = 15;

// very heavy tanks
idx = getIndexOf1(WEAPON_BAZOOKA.damageTable, "MEGATANK");
WEAPON_BAZOOKA.damageTable[idx][1] = 5;

idx = getIndexOf1(WEAPON_BAZOOKA.damageTable, "HOELLIUM");
WEAPON_BAZOOKA.damageTable[idx][1] = 30;

// ranged land units
idx = getIndexOf1(WEAPON_BAZOOKA.damageTable, "ARTILLERY");
WEAPON_BAZOOKA.damageTable[idx][1] = 70;
idx = getIndexOf1(WEAPON_BAZOOKA.damageTable, "ANTITANKCANNON");
WEAPON_BAZOOKA.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_BAZOOKA.damageTable, "MISSILE");
WEAPON_BAZOOKA.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_BAZOOKA.damageTable, "ROCKETTHROWER");
WEAPON_BAZOOKA.damageTable[idx][1] = 85;
idx = getIndexOf1(WEAPON_BAZOOKA.damageTable, "PIPERUNNER");
WEAPON_BAZOOKA.damageTable[idx][1] = 55;
