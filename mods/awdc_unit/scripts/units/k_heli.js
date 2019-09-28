K_HELI.init = function(unit)
{
    unit.setAmmo1(6);
    unit.setMaxAmmo1(6);
    unit.setWeapon1ID("WEAPON_AIR_TO_GROUND_MISSILE");

    unit.setAmmo2(-1);
    unit.setMaxAmmo2(-1);
    unit.setWeapon2ID("WEAPON_K_HELI_MG");

    unit.setFuel(100);
    unit.setMaxFuel(100);
    unit.setBaseMovementPoints(6);
    unit.setMinRange(1);
    unit.setMaxRange(1);
    unit.setVision(2);
};
K_HELI.getBaseCost = function()
{
    return 9000;
};
