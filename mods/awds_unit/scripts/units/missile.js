MISSILE.init = function(unit)
{
    unit.setAmmo1(6);
    unit.setMaxAmmo1(6);
    unit.setWeapon1ID("WEAPON_ANTI_AIR_MISSILE");

    unit.setAmmo2(0);
    unit.setMaxAmmo2(0);
    unit.setWeapon2ID("");

    unit.setFuel(50);
    unit.setMaxFuel(50);
    unit.setBaseMovementPoints(4);
    unit.setMinRange(3);
    unit.setMaxRange(5);
    unit.setVision(5);
};
MISSILE.getBaseCost = function()
{
    return 12000;
};
