SUBMARINE.init = function(unit)
{
    unit.setAmmo1(6);
    unit.setMaxAmmo1(6);
    unit.setWeapon1ID("WEAPON_TORPEDO");

    unit.setAmmo2(0);
    unit.setMaxAmmo2(0);
    unit.setWeapon2ID("");

    unit.setFuel(70);
    unit.setMaxFuel(70);
    unit.setBaseMovementPoints(6);
    unit.setMinRange(1);
    unit.setMaxRange(1);
    unit.setVision(5);
};
SUBMARINE.getBaseCost = function()
{
    return 20000;
};
SUBMARINE.startOfTurn = function(unit)
{
    // pay unit upkeep
    var fuelCosts = 1 + unit.getFuelCostModifier(Qt.point(unit.getX(), unit.getY()), 1);
    if (unit.getHidden())
    {
        fuelCosts += 4;
    }
    if (fuelCosts < 0)
    {
        fuelCosts = 0;
    }
    unit.setFuel(unit.getFuel() - fuelCosts);
};
