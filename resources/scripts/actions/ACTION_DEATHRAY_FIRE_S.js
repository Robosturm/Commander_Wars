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

        var animation = GameAnimationFactory.createAnimation(x - 1, y - 1);
        animation.addSprite("deathray_start_loading", 0, 0, 0, 1.5);
        var animation2 = GameAnimationFactory.createAnimation(x - 1, y - 1);
        animation2.addSprite("deathray_start", 0, 0, 0, 1.5);
        animation.queueAnimation(animation2);

        for (var i = 0; i < fields.size(); i++)
        {
            var point = fields.at(i);
            if (map.onMap(x + point.x, y + point.y))
            {
                var unit = map.getTerrain(x + point.x, y + point.y).getUnit();
                if ((unit !== null) &&
                    (building.getOwner().isEnemyUnit(unit)))
                {
                    unit.setHp(unit.getHpRounded() - 8);
                    if (unit.getHp() <= 0)
                    {
                        unit.killUnit();
                    }
                }
                if ((point.x === -1) && ((point.y) % 2 === 0))
                {
                    animation2 = GameAnimationFactory.createAnimation(x + point.x, y + point.y);
                    animation2.addSprite("deathray", 0, -map.getImageSize() * 0.085, 0, 1.5);
                    animation.queueAnimation(animation2);
                }
            }
        }
        fields.remove();
    };
}

Constructor.prototype = ACTION;
var ACTION_DEATHRAY_FIRE_S = new Constructor();
