ZCOUNIT_TANK_HUNTER.init = function(unit)
{
    unit.setAmmo1(8);
    unit.setMaxAmmo1(8);
    unit.setWeapon1ID("WEAPON_HEAVY_TANK_GUN");

    unit.setAmmo2(-1);
    unit.setMaxAmmo2(-1);
    unit.setWeapon2ID("WEAPON_HEAVY_TANK_MG");

    unit.setFuel(60);
    unit.setMaxFuel(60);
    unit.setBaseMovementPoints(5);
    unit.setMinRange(1);
    unit.setMaxRange(1);
    unit.setVision(1);
};
