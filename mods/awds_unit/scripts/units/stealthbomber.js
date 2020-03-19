STEALTHBOMBER.init = function(unit)
{
    unit.setAmmo1(6);
    unit.setMaxAmmo1(6);
    unit.setWeapon1ID("WEAPON_STEALTHROCKET");

    unit.setAmmo2(0);
    unit.setMaxAmmo2(0);
    unit.setWeapon2ID("");

    unit.setVisionHigh(999);
    unit.setFuel(60);
    unit.setMaxFuel(60);
    unit.setBaseMovementPoints(6);
    unit.setMinRange(1);
    unit.setMaxRange(1);
    unit.setVision(4);
};
STEALTHBOMBER.getBaseCost = function()
{
    return 24000;
};
STEALTHBOMBER.startOfTurn = function(unit)
{
    // pay unit upkeep
    var fuelCosts = 5;
    if (unit.getHidden())
    {
        fuelCosts += 3;
    }
    fuelCosts = fuelCosts + unit.getFuelCostModifier(Qt.point(unit.getX(), unit.getY()), fuelCosts);
    if (fuelCosts < 0)
    {
        fuelCosts = 0;
    }
    unit.setFuel(unit.getFuel() - fuelCosts);
};
