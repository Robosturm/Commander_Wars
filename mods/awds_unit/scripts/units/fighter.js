FIGHTER.init = function(unit)
{
    unit.setAmmo1(9);
    unit.setMaxAmmo1(9);
    unit.setWeapon1ID("WEAPON_AIR_TO_AIR_MISSILE");

    unit.setAmmo2(0);
    unit.setMaxAmmo2(0);
    unit.setWeapon2ID("");

    unit.setFuel(100);
    unit.setMaxFuel(100);
    unit.setBaseMovementPoints(9);
    unit.setMinRange(1);
    unit.setMaxRange(1);
    unit.setVision(2);
};
FIGHTER.getBaseCost = function()
{
    return 20000;
};
