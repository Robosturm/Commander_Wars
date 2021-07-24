AIRCRAFTCARRIER.init = function(unit)
{
    unit.setAmmo1(9);
    unit.setMaxAmmo1(9);
    unit.setWeapon1ID("WEAPON_CARRIER_ANTI_AIR_MISSILE");

    unit.setAmmo2(0);
    unit.setMaxAmmo2(0);
    unit.setWeapon2ID("");

    unit.setFuel(100);
    unit.setMaxFuel(100);
    unit.setBaseMovementPoints(5);
    unit.setMinRange(3);
    unit.setMaxRange(8);
    unit.setVision(4);
};

AIRCRAFTCARRIER.getBaseCost = function()
{
    return 30000;
};
AIRCRAFTCARRIER.actionList = ["ACTION_FIRE", "ACTION_JOIN", "ACTION_LOAD", "ACTION_UNLOAD", "ACTION_WAIT", "ACTION_CO_UNIT_0", "ACTION_CO_UNIT_1"];
