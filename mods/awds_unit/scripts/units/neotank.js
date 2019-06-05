NEOTANK.init = function(unit)
{
    unit.setAmmo1(9);
    unit.setMaxAmmo1(9);
    unit.setWeapon1ID("WEAPON_NEOTANK_GUN");

    unit.setAmmo2(-1);
    unit.setMaxAmmo2(-1);
    unit.setWeapon2ID("WEAPON_NEOTANK_MG");

    unit.setFuel(99);
    unit.setMaxFuel(99);
    unit.setBaseMovementPoints(6);
    unit.setMinRange(1);
    unit.setMaxRange(1);
    unit.setVision(1);
};
NEOTANK.getBaseCost = function()
{
    return 22000;
};
