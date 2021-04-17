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
        return qsTr("Heal");
    };
    this.getIcon = function()
    {
        return "heal";
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
        building.setFireCount(building.getFireCount() - 1);
        var fields = Global[building.getBuildingID()].getActionTargetFields(building);
        var animation = null;
        var damage = Global[building.getBuildingID()].getDamage(building, null);
        for (var i = 0; i < fields.size(); i++)
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
                    animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
                    animation.addSprite("power0", -map.getImageSize() * 1.27, -map.getImageSize() * 1.27, 0, 1.5);
                }
            }
        }
        if (animation !== null)
        {
            var sound = Global[building.getBuildingID()].getHealSound();
            animation.addSound(sound);
        }
        fields.remove();
    };
    this.getDescription = function()
    {
        return qsTr("Orders a crystall or monolith to heal all own units around it and also refills their ammo and fuel.");
    };
}

Constructor.prototype = ACTION;
var ACTION_CRYSTALL_HEAL = new Constructor();
