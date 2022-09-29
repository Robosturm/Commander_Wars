var Constructor = function()
{
    
    this.canBePerformed = function(action, map)
    {
        var building = action.getTargetBuilding();
        if (building.getFireCount() >= 1)
        {
            return true;
        }
        else
        {
            return false;
        }
    };
    this.getActionText = function(map)
    {
        return qsTr("Fire");
    };
    this.getIcon = function(map)
    {
        return "icon_fire";
    };
    this.isFinalStep = function(action, map)
    {
        return true;
    };
    this.perform = function(action, map)
    {
        // we need to move the unit to the target position
        var building = action.getTargetBuilding();
        var x = building.getX();
        var y = building.getY();
        var damage = Global[building.getBuildingID()].getDamage();
        var targets = Global[building.getBuildingID()].getActionTargetFields(building);
        var size = targets.size()
        for (var i = 0; i < size; i++)
        {
            var point = targets.at(i);
            if (map.onMap(x + point.x, y + point.y))
            {
                var unit = map.getTerrain(x + point.x, y + point.y).getUnit();
                if ((unit !== null))
                {
                    unit.setHp(unit.getHpRounded() - damage);
                    if (unit.getHp() <= 0)
                    {
                        unit.killUnit();
                    }
                }
            }
        }
    };
    this.getDescription = function()
    {
        return qsTr("Commands the laser to shot a ray. The laser rays deal damage to all units that get hit.");
    };
}

Constructor.prototype = ACTION;
var ACTION_NEST_LASER = new Constructor();
