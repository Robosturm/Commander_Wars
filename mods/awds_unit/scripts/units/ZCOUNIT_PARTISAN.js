ZCOUNIT_PARTISAN.init = function(unit)
{
    unit.setAmmo1(-1);
    unit.setMaxAmmo1(-1);
    unit.setWeapon1ID("WEAPON_INFANTRY_MG");

    unit.setAmmo2(1);
    unit.setMaxAmmo2(1);
    unit.setWeapon2ID("WEAPON_FIREBOMB");

    unit.setFuel(100);
    unit.setMaxFuel(100);
    unit.setBaseMovementPoints(3);
    unit.setMinRange(1);
    unit.setMaxRange(1);
    unit.setVision(2);
    unit.setCursorInfoRange(3);
};
