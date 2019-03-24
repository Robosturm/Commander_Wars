var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
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
    this.getActionText = function()
    {
        return qsTr("Fire");
    };
    this.getIcon = function()
    {
        return "fire";
    };
    this.isFinalStep = function(action)
    {
        return true;
    };
    this.perform = function(action)
    {
        // we need to move the unit to the target position
        var building = action.getTargetBuilding();
        var x = building.getX();
        var y = building.getY();
        building.setFireCount(building.getFireCount() - 1);
        var fields = Global[building.getBuildingID()].getActionTargetFields(building);
        var animation = null;

        for (var i = 0; i < fields.size(); i++)
        {
            var point = fields.at(i);
            var unit = map.getTerrain(x + point.x, y + point.y).getUnit();
            if ((unit !== null))
            {
                unit.setHp(unit.getHpRounded() - 5);
                if (unit.getHp() <= 0)
                {
                    unit.killUnit();
                }
            }
        }
        fields.remove();
    };
}

Constructor.prototype = ACTION;
var ACTION_LASER_FIRE = new Constructor();
