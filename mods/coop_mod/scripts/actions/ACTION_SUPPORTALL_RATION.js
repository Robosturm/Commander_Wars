ACTION_SUPPORTALL_RATION.checkUnit = function(unit, x, y)
{
    if (map.onMap(x, y))
    {
        var target = map.getTerrain(x, y).getUnit();
        if (target !== null)
        {
            if (target.getOwner().isAlly(unit.getOwner()) &&
                    target !== unit)
            {
                return true;
            }
        }
    }
    return false;
};
ACTION_SUPPORTALL_RATION_MONEY.checkUnit = function(unit, x, y)
{
    if (map.onMap(x, y))
    {
        var target = map.getTerrain(x, y).getUnit();
        if (target !== null)
        {
            if (target.getOwner().isAlly(unit.getOwner()) &&
                    target !== unit)
            {
                return true;
            }
        }
    }
    return false;
};
