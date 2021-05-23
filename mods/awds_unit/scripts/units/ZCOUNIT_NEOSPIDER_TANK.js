ZCOUNIT_NEOSPIDER_TANK.init = function(unit)
{
    unit.setAmmo1(4);
    unit.setMaxAmmo1(4);
    unit.setWeapon1ID("WEAPON_NEOTANK_GUN");

    unit.setAmmo2(-1);
    unit.setMaxAmmo2(-1);
    unit.setWeapon2ID("WEAPON_NEOTANK_MG");

    unit.setFuel(50);
    unit.setMaxFuel(50);
    unit.setBaseMovementPoints(5);
    unit.setMinRange(1);
    unit.setMaxRange(1);
    unit.setVision(2);
};
ZCOUNIT_NEOSPIDER_TANK.getBaseCost = function()
{
    return 22000;
};
