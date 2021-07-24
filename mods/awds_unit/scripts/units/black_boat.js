BLACK_BOAT.init = function(unit)
{
    unit.setAmmo1(0);
    unit.setMaxAmmo1(0);
    unit.setWeapon1ID("");
    unit.setAmmo2(0);
    unit.setMaxAmmo2(0);
    unit.setWeapon2ID("");
    unit.setFuel(60);
    unit.setMaxFuel(60);
    unit.setBaseMovementPoints(7);
    unit.setMinRange(1);
    unit.setMaxRange(1);
    unit.setVision(1);
};
BLACK_BOAT.actionList = ["ACTION_SUPPORTSINGLE_REPAIR", "ACTION_JOIN", "ACTION_LOAD", "ACTION_UNLOAD", "ACTION_WAIT", "ACTION_CO_UNIT_0", "ACTION_CO_UNIT_1"];
