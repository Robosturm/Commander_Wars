AIRCRAFTCARRIER.init = function(unit)
{
    unit.setAmmo1(-1);
    unit.setMaxAmmo1(-1);
    unit.setWeapon1ID("WEAPON_CARRIER_ANTI_AIR_MISSILE");

    unit.setAmmo2(4);
    unit.setMaxAmmo2(4);
    unit.setWeapon2ID("");

    unit.setFuel(100);
    unit.setMaxFuel(100);
    unit.setBaseMovementPoints(5);
    unit.setMinRange(1);
    unit.setMaxRange(1);
    unit.setVision(4);
};

AIRCRAFTCARRIER.getBaseCost = function()
{
    return 28000;
};

AIRCRAFTCARRIER.startOfTurnOldAWDCUnit = AIRCRAFTCARRIER.startOfTurn;
AIRCRAFTCARRIER.startOfTurn = function (unit, map) {
    AIRCRAFTCARRIER.startOfTurnOldAWDCUnit(unit, map);
    var size = unit.getLoadedUnitCount();
    for (var i = 0; i < size; i++)
    {
        var transportUnit = unit.getLoadedUnit(i);
        transportUnit.setHasMoved(false);
    }
}

AIRCRAFTCARRIER.actionList = ["ACTION_FIRE", "ACTION_JOIN", "ACTION_LOAD", "ACTION_UNLOAD_LAUNCH", "ACTION_BUILD_WATERPLANE", "ACTION_WAIT", "ACTION_CO_UNIT_0", "ACTION_CO_UNIT_1"];