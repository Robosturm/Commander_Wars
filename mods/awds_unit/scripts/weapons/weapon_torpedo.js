var idx = 0;
idx = getIndexOf1(WEAPON_TORPEDO.damageTable, "BATTLESHIP");
WEAPON_TORPEDO.damageTable[idx][1] = 65;
idx = getIndexOf1(WEAPON_TORPEDO, "CANNONBOAT");
WEAPON_TORPEDO.damageTable[idx][1] = 95;

idx = getIndexOf1(WEAPON_TORPEDO, "CRUISER");
WEAPON_TORPEDO.damageTable[idx][1] = 25;
idx = getIndexOf1(WEAPON_TORPEDO, "DESTROYER");
WEAPON_TORPEDO.damageTable[idx][1] = 55;
idx = getIndexOf1(WEAPON_TORPEDO, "SUBMARINE");
WEAPON_TORPEDO.damageTable[idx][1] = 55;

idx = getIndexOf1(WEAPON_TORPEDO, "LANDER");
WEAPON_TORPEDO.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_TORPEDO, "BLACK_BOAT");
WEAPON_TORPEDO.damageTable[idx][1] = 95;
idx = getIndexOf1(WEAPON_TORPEDO, "AIRCRAFTCARRIER");
WEAPON_TORPEDO.damageTable[idx][1] = 75;
