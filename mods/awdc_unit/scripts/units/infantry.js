INFANTRY.init = function(unit)
{
    unit.setAmmo1(10);
    unit.setMaxAmmo1(10);
    unit.setWeapon1ID("WEAPON_INFANTRY_MG");

    unit.setAmmo2(0);
    unit.setMaxAmmo2(0);
    unit.setWeapon2ID("");

    unit.setFuel(100);
    unit.setMaxFuel(100);
    unit.setBaseMovementPoints(3);
    unit.setMinRange(1);
    unit.setMaxRange(1);
    unit.setVision(2);
};
INFANTRY.getBaseCost = function()
{
    return 1500;
};
