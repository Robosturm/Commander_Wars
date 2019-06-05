MOTORBIKE.init = function(unit)
{
    unit.setAmmo1(-1);
    unit.setMaxAmmo1(-1);
    unit.setWeapon1ID("WEAPON_MOTORBIKE_MG");

    unit.setAmmo2(0);
    unit.setMaxAmmo2(0);
    unit.setWeapon2ID("");

    unit.setFuel(70);
    unit.setMaxFuel(70);
    unit.setBaseMovementPoints(5);
    unit.setMinRange(1);
    unit.setMaxRange(1);
    unit.setVision(2);
};
MOTORBIKE.getBaseCost = function()
{
    return 3000;
};
