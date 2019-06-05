PIPERUNNER.init = function(unit)
{
    unit.setAmmo1(9);
    unit.setMaxAmmo1(9);
    unit.setWeapon1ID("WEAPON_PIPERUNNER_MISSILE");

    unit.setAmmo2(0);
    unit.setMaxAmmo2(0);
    unit.setWeapon2ID("");

    unit.setFuel(99);
    unit.setMaxFuel(99);
    unit.setBaseMovementPoints(9);
    unit.setMinRange(2);
    unit.setMaxRange(5);
    unit.setVision(4);
};
PIPERUNNER.getBaseCost = function()
{
    return 20000;
};
