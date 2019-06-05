ANTITANKCANNON.init = function(unit)
{
    unit.setAmmo1(6);
    unit.setMaxAmmo1(6);
    unit.setWeapon1ID("WEAPON_A_TANK_CANNON");

    unit.setAmmo2(0);
    unit.setMaxAmmo2(0);
    unit.setWeapon2ID("");

    unit.setFuel(50);
    unit.setMaxFuel(50);
    unit.setBaseMovementPoints(4);
    unit.setMinRange(1);
    unit.setMaxRange(3);
    unit.setVision(2);

};
