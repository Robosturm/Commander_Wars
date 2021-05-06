ZCOUNIT_ROYAL_GUARD.init = function(unit)
{
    unit.setAmmo1(5);
    unit.setMaxAmmo1(5);
    unit.setWeapon1ID("WEAPON_HEAVY_TANK_GUN");

    unit.setAmmo2(-1);
    unit.setMaxAmmo2(-1);
    unit.setWeapon2ID("WEAPON_HEAVY_TANK_MG");

    unit.setFuel(50);
    unit.setMaxFuel(50);
    unit.setBaseMovementPoints(5);
    unit.setMinRange(1);
    unit.setMaxRange(1);
    unit.setVision(2);
};
