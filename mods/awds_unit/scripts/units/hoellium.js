HOELLIUM.init = function(unit)
{
    unit.setAmmo1(0);
    unit.setMaxAmmo1(0);
    unit.setWeapon1ID("");

    unit.setAmmo2(0);
    unit.setMaxAmmo2(0);
    unit.setWeapon2ID("");

    unit.setFuel(100);
    unit.setMaxFuel(100);
    unit.setBaseMovementPoints(1);
    unit.setMinRange(1);
    unit.setMaxRange(1);
    unit.setVision(1);
    unit.setIgnoreUnitCollision(true);
};
