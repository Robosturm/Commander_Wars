APC.init = function(unit)
{
    unit.setAmmo1(0);
    unit.setMaxAmmo1(0);
    unit.setWeapon1ID("");

    unit.setAmmo2(0);
    unit.setMaxAmmo2(0);
    unit.setWeapon2ID("APC");

    unit.setFuel(70);
    unit.setMaxFuel(70);
    unit.setBaseMovementPoints(6);
    unit.setMinRange(1);
    unit.setMaxRange(1);
    unit.setVision(1);
};
APC.getActions = function()
{
    // returns a string id list of the actions this unit can perform
    return "ACTION_LOAD,ACTION_UNLOAD,ACTION_JOIN,ACTION_RATION,ACTION_WAIT,ACTION_CO_UNIT_0,ACTION_CO_UNIT_1";
};
