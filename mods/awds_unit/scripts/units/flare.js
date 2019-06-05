FLARE.init = function(unit)
{
    unit.setAmmo1(3);
    unit.setMaxAmmo1(3);
    unit.setWeapon1ID("WEAPON_FLARE");

    unit.setAmmo2(-1);
    unit.setMaxAmmo2(-1);
    unit.setWeapon2ID("WEAPON_FLARE_MG");

    unit.setFuel(60);
    unit.setMaxFuel(60);
    unit.setBaseMovementPoints(5);
    unit.setMinRange(1);
    unit.setMaxRange(1);
    unit.setVision(2);
};
