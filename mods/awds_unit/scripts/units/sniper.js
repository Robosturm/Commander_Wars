SNIPER.init = function(unit)
{
    unit.setAmmo1(10);
    unit.setMaxAmmo1(10);
    unit.setWeapon1ID("WEAPON_SNIPER");

    unit.setAmmo2(0);
    unit.setMaxAmmo2(0);
    unit.setWeapon2ID("");

    unit.setFuel(100);
    unit.setMaxFuel(100);
    unit.setBaseMovementPoints(3);
    unit.setMinRange(2);
    unit.setMaxRange(3);
    unit.setVision(2);
};
SNIPER.getBaseCost = function()
{
    return 3000;
};
