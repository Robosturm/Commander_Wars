LIGHT_TANK.init = function(unit)
{
    unit.setAmmo1(6);
    unit.setMaxAmmo1(6);
    unit.setWeapon1ID("WEAPON_LIGHT_TANK_GUN");

    unit.setAmmo2(-1);
    unit.setMaxAmmo2(-1);
    unit.setWeapon2ID("WEAPON_LIGHT_TANK_MG");

    unit.setFuel(70);
    unit.setMaxFuel(70);
    unit.setBaseMovementPoints(6);
    unit.setMinRange(1);
    unit.setMaxRange(1);
    unit.setVision(2);
};
