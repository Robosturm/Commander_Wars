ROCKETTHROWER.init = function(unit)
{
    unit.setAmmo1(5);
    unit.setMaxAmmo1(5);
    unit.setWeapon1ID("WEAPON_ROCKET_MISSILE");

    unit.setAmmo2(0);
    unit.setMaxAmmo2(0);
    unit.setWeapon2ID("");

    unit.setFuel(50);
    unit.setMaxFuel(50);
    unit.setBaseMovementPoints(5);
    unit.setMinRange(3);
    unit.setMaxRange(5);
    unit.setVision(3);
};
