MECH.init = function(unit)
{
    unit.setAmmo1(-1);
    unit.setMaxAmmo1(-1);
    unit.setWeapon1ID("WEAPON_MECH_MG");

    unit.setAmmo2(3);
    unit.setMaxAmmo2(3);
    unit.setWeapon2ID("WEAPON_BAZOOKA");

    unit.setFuel(70);
    unit.setMaxFuel(70);
    unit.setBaseMovementPoints(2);
    unit.setMinRange(1);
    unit.setMaxRange(1);
    unit.setVision(2);
};
MECH.getBaseCost = function()
{
    return 3000;
};
