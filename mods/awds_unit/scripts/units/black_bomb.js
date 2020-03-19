BLACK_BOMB.init = function(unit)
{
    unit.setAmmo1(0);
    unit.setMaxAmmo1(0);
    unit.setWeapon1ID("");

    unit.setAmmo2(0);
    unit.setMaxAmmo2(0);
    unit.setWeapon2ID("");

    unit.setVisionHigh(999);
    unit.setFuel(45);
    unit.setMaxFuel(45);
    unit.setBaseMovementPoints(9);
    unit.setMinRange(1);
    unit.setMaxRange(1);
    unit.setVision(1);
};
BLACK_BOMB.getBaseCost = function()
{
    return 25000;
};
