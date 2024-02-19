MECH.init = function(unit)
{
    unit.setAmmo1(3);
    unit.setMaxAmmo1(3);
    unit.setWeapon1ID("WEAPON_BAZOOKA");

    unit.setAmmo2(-1);
    unit.setMaxAmmo2(-1);
    unit.setWeapon2ID("WEAPON_MECH_MG");

    unit.setFuel(70);
    unit.setMaxFuel(70);
    unit.setBaseMovementPoints(2);
    unit.setMinRange(1);
    unit.setMaxRange(1);
    unit.setVision(2);
};
MECH.getBaseCost = function()
{
    return 2500;
};
