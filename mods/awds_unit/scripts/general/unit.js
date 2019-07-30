UNIT.transporterRefilling = function (unit)
{
    // carrier refilling and unmoving is done here
    for (var i = 0; i < unit.getLoadedUnitCount(); i++)
    {
        var transportUnit = unit.getLoadedUnit(i);
        transportUnit.refill();
        transportUnit.setHasMoved(false);
    }
};
