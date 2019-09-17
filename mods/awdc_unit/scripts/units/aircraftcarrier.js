AIRCRAFTCARRIER.init = function(unit)
{
    unit.setAmmo1(4);
    unit.setMaxAmmo1(4);
    unit.setWeapon1ID("WEAPON_CARRIER_ANTI_AIR_MISSILE");

    unit.setAmmo2(0);
    unit.setMaxAmmo2(0);
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
