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
        return "icon_fire";
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

        var owner = building.getOwner();
        var talonGunAnimation = GameAnimationFactory.createAnimation(x, y);
        talonGunAnimation.addSprite("talon+gun+fire", -map.getImageSize() * 1.0, -map.getImageSize() * 1.6, 0, 1.33);
        talonGunAnimation.addSpriteAnimTable("talon+gun+fire+mask", -map.getImageSize() * 1.0, -map.getImageSize() * 1.6, owner, 0, 1.33, 1.33, 0, 0, GameEnums.Recoloring_Matrix);
        var fields = Global[building.getBuildingID()].getActionTargetFields(building);
        var animation = null;
        for (var i = 0; i < fields.size(); i++)
        {
            var point = fields.at(i);
            if (map.onMap(x + point.x, y + point.y))
            {
                var unit = map.getTerrain(x + point.x, y + point.y).getUnit();
                if ((unit !== null) &&
                    (owner.isEnemyUnit(unit)))
                {
                    var damage = Global[building.getBuildingID()].getDamage(building, unit);
                    unit.setHp(unit.getHpRounded() - damage);
                    animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
                    animation.addSprite("talon+gun+hit", -map.getImageSize() * 1.1, -map.getImageSize() * 1.5, 0, 1.33);
                    talonGunAnimation.queueAnimation(animation);
                    if (unit.getHp() <= 0)
                    {
                        unit.killUnit();
                    }
                }
            }
        }
        fields.remove();
    };
    this.getDescription = function()
    {
        return qsTr("Fire with Talon Gun");
    };
}

Constructor.prototype = ACTION;
var ACTION_TALON_GUN = new Constructor();
