CRUISER.init = function(unit)
{
    unit.setAmmo1(9);
    unit.setMaxAmmo1(9);
    unit.setWeapon1ID("WEAPON_ANTI_SHIP_CRUISER_MISSILE");

    unit.setAmmo2(-1);
    unit.setMaxAmmo2(-1);
    unit.setWeapon2ID("WEAPON_A_AIR_GUN");

    unit.setFuel(100);
    unit.setMaxFuel(100);
    unit.setBaseMovementPoints(6);
    unit.setMinRange(1);
    unit.setMaxRange(1);
    unit.setVision(5);
};
CRUISER.getBaseCost = function()
{
    return 18000;
};
