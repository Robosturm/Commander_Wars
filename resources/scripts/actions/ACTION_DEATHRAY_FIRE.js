var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
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
        var offset = Global[building.getBuildingID()].getActionTargetOffset(building);
        var x = building.getX() + offset.x;
        var y = building.getY() + offset.y;
        building.setFireCount(building.getFireCount() - 7);

        var fields = Global[building.getBuildingID()].getActionTargetFields(building);

        var animation = Global[building.getBuildingID()].createRayAnimation(building, x, y, fields);
        animation.setEndOfAnimationCall("ACTION_DEATHRAY_FIRE", "performPostAnimation");
        ACTION_DEATHRAY_FIRE.postAnimationFields = fields;
        ACTION_DEATHRAY_FIRE.postAnimationX = x;
        ACTION_DEATHRAY_FIRE.postAnimationY = y;
        ACTION_DEATHRAY_FIRE.postAnimationBuilding = building;
    };
    this.postAnimationFields = null;
    this.postAnimationX = null;
    this.postAnimationY = null;
    this.postAnimationBuilding = null;
    this.performPostAnimation = function(postAnimation)
    {
        var x = ACTION_DEATHRAY_FIRE.postAnimationX;
        var y = ACTION_DEATHRAY_FIRE.postAnimationY;
        var building = ACTION_DEATHRAY_FIRE.postAnimationBuilding;

        for (var i = 0; i < ACTION_DEATHRAY_FIRE.postAnimationFields.size(); i++)
        {
            var point = ACTION_DEATHRAY_FIRE.postAnimationFields.at(i);
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
        ACTION_DEATHRAY_FIRE.postAnimationFields.remove();
        ACTION_DEATHRAY_FIRE.postAnimationFields = null;
        ACTION_DEATHRAY_FIRE.postAnimationX = null;
        ACTION_DEATHRAY_FIRE.postAnimationY = null;
        ACTION_DEATHRAY_FIRE.postAnimationBuilding = null;
    }
}

Constructor.prototype = ACTION;
var ACTION_DEATHRAY_FIRE = new Constructor();
