ZCOUNIT_COMMANDO.init = function(unit)
{
    unit.setAmmo1(3);
    unit.setMaxAmmo1(3);
    unit.setWeapon1ID("WEAPON_BAZOOKA");

    unit.setAmmo2(-1);
    unit.setMaxAmmo2(-1);
    unit.setWeapon2ID("WEAPON_RECON_MG");

    unit.setFuel(99);
    unit.setMaxFuel(99);
    unit.setBaseMovementPoints(4);
    unit.setMinRange(1);
    unit.setMaxRange(1);
    unit.setVision(3);
};
