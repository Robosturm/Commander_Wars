var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        var building = action.getTargetBuilding();
        if (building.getFireCount() >= 7)
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
        var offset = Global[building.getBuildingID()].getActionTargetOffset(building);
        var x = building.getX() + offset.x;
        var y = building.getY() + offset.y;
        building.setFireCount(building.getFireCount() - 7);

        var fields = Global[building.getBuildingID()].getActionTargetFields(building);

        var animation = Global[building.getBuildingID()].createRayAnimation(building, x, y, fields, map);
        animation.setEndOfAnimationCall("ACTION_DEATHRAY_FIRE", "performPostAnimation");
        animation.addSound("deathray_fire.wav");
        ACTION_DEATHRAY_FIRE.postAnimationX = x;
        ACTION_DEATHRAY_FIRE.postAnimationY = y;
        ACTION_DEATHRAY_FIRE.postAnimationBuilding = building;
    };
    this.postAnimationFields = null;
    this.postAnimationX = null;
    this.postAnimationY = null;
    this.postAnimationBuilding = null;
    this.performPostAnimation = function(postAnimation, map)
    {
        var x = ACTION_DEATHRAY_FIRE.postAnimationX;
        var y = ACTION_DEATHRAY_FIRE.postAnimationY;
        var building = ACTION_DEATHRAY_FIRE.postAnimationBuilding;
        var fields = Global[building.getBuildingID()].getActionTargetFields(building);
        var size = fields.size();
        for (var i = 0; i < size; i++)
        {
            var point = fields.at(i);
            if (map.onMap(x + point.x, y + point.y))
            {
                var unit = map.getTerrain(x + point.x, y + point.y).getUnit();
                var damage = Global[building.getBuildingID()].getDamage(building, unit);
                if ((unit !== null) &&
                    (building.getOwner().isEnemyUnit(unit)))
                {
                    unit.setHp(unit.getHpRounded() - damage);
                    if (unit.getHp() <= 0)
                    {
                        unit.killUnit();
                    }
                }
            }
        }
        ACTION_DEATHRAY_FIRE.postAnimationX = null;
        ACTION_DEATHRAY_FIRE.postAnimationY = null;
        ACTION_DEATHRAY_FIRE.postAnimationBuilding = null;
    };
    this.getName = function()
    {
        return qsTr("Fire with death ray");
    };
    this.getDescription = function()
    {
        return qsTr("Orders a death ray to attack all enemy units in front of it, dealing devastating damage to them. This order has a cooldown of 7 days.");
    };
}

Constructor.prototype = ACTION;
var ACTION_DEATHRAY_FIRE = new Constructor();
