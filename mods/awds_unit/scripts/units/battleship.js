BATTLESHIP.init = function(unit)
{
    unit.setAmmo1(9);
    unit.setMaxAmmo1(9);
    unit.setWeapon1ID("WEAPON_BATTLESHIP_CANNON");

    unit.setAmmo2(0);
    unit.setMaxAmmo2(0);
    unit.setWeapon2ID("");

    unit.setFuel(100);
    unit.setMaxFuel(100);
    unit.setBaseMovementPoints(5);
    unit.setMinRange(2);
    unit.setMaxRange(6);
    unit.setVision(2);
};

BATTLESHIP.getBaseCost = function()
{
    return 28000;
};

BATTLESHIP.canMoveAndFire = function()
{
    return false;
};

BATTLESHIP.canCounterOnRangeAttacks = function(unit, defX, defY, attacker, atkX, atkY, pAction, luckMode, map)
{
    return false;
};
