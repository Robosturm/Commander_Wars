ZCOUNIT_SMUGGLER.init = function(unit)
{
    unit.setAmmo1(-1);
    unit.setMaxAmmo1(-1);
    unit.setWeapon1ID("WEAPON_RECON_MG");

    unit.setAmmo2(1);
    unit.setMaxAmmo2(1);
    unit.setWeapon2ID("WEAPON_CARGO");

    unit.setFuel(80);
    unit.setMaxFuel(80);
    unit.setBaseMovementPoints(8);
    unit.setMinRange(1);
    unit.setMaxRange(1);
    unit.setVision(2);
};
