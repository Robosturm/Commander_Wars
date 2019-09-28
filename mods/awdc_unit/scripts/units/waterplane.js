WATERPLANE.init = function(unit)
{
    unit.setAmmo1(3);
    unit.setMaxAmmo1(3);
    unit.setWeapon1ID("WEAPON_WATERPLANE_MISSILE");

    unit.setAmmo2(0);
    unit.setMaxAmmo2(0);
    unit.setWeapon2ID("");

    unit.setFuel(50);
    unit.setMaxFuel(50);
    unit.setBaseMovementPoints(7);
    unit.setMinRange(1);
    unit.setMaxRange(1);
    unit.setVision(3);
};
