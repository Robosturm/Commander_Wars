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
        return qsTr("Heal");
    };
    this.getIcon = function(map)
    {
        return "heal";
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
        building.setFireCount(building.getFireCount() - 1);
        var fields = Global[building.getBuildingID()].getActionTargetFields(building);
        var animation = null;
        var damage = Global[building.getBuildingID()].getDamage(building, null);
        var size = fields.size();
        for (var i = 0; i < size; i++)
        {
            var point = fields.at(i);
            if (map.onMap(x + point.x, y + point.y))
            {
                var unit = map.getTerrain(x + point.x, y + point.y).getUnit();
                if ((unit !== null) &&
                        (unit.getOwner() === building.getOwner()))
                {
                    unit.refill();
                    unit.setHp(unit.getHpRounded() - damage);
                    animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
                    animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 2);
                }
            }
        }
        if (animation !== null)
        {
            var sound = Global[building.getBuildingID()].getHealSound();
            animation.addSound(sound);
        }
    };
    this.getDescription = function()
    {
        return qsTr("Orders a crystall or monolith to heal all own units around it and also refills their ammo and fuel.");
    };
}

Constructor.prototype = ACTION;
var ACTION_CRYSTALL_HEAL = new Constructor();
