ARTILLERY.init = function(unit)
{
    unit.setAmmo1(9);
    unit.setMaxAmmo1(9);
    unit.setWeapon1ID("WEAPON_CANNON");

    unit.setAmmo2(0);
    unit.setMaxAmmo2(0);
    unit.setWeapon2ID("");

    unit.setFuel(50);
    unit.setMaxFuel(50);
    unit.setBaseMovementPoints(5);
    unit.setMinRange(2);
    unit.setMaxRange(3);
    unit.setVision(1);

};
