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
        var offset = Global[building.getBuildingID()].getActionTargetOffset(building);
        var x = building.getX() + offset.x;
        var y = building.getY() + offset.y;
        building.setFireCount(building.getFireCount() - 1);

        var owner = building.getOwner();
        var talonGunAnimation = GameAnimationFactory.createAnimation(map, x, y);
        talonGunAnimation.addSprite("talon+gun+fire", -map.getImageSize() * 1.0, -map.getImageSize() * 1.6, 0, 1.33);
        talonGunAnimation.addSpriteAnimTable("talon+gun+fire+mask", -map.getImageSize() * 1.0, -map.getImageSize() * 1.6, owner, 0, 1.33, 1.33, 0, 0, GameEnums.Recoloring_Matrix);
        var fields = Global[building.getBuildingID()].getActionTargetFields(building);
        var animation = null;
        var size = fields.size();
        for (var i = 0; i < size; i++)
        {
            var point = fields.at(i);
            if (map.onMap(x + point.x, y + point.y))
            {
                var unit = map.getTerrain(x + point.x, y + point.y).getUnit();
                if ((unit !== null) &&
                    (owner.isEnemyUnit(unit)))
                {
                    var damage = Global[building.getBuildingID()].getDamage(building, unit);
                    animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
                    animation.addSprite("talon+gun+hit", -map.getImageSize() * 1.1, -map.getImageSize() * 1.5, 0, 1.33);
                    talonGunAnimation.queueAnimation(animation);
                    animation.writeDataInt32(x + point.x);
                    animation.writeDataInt32(y + point.y);
                    animation.writeDataInt32(damage);
                    animation.setEndOfAnimationCall("ANIMATION", "postAnimationDamageKill");
                }
            }
        }
        var talonGunInAnimation = GameAnimationFactory.createAnimation(map, x, y);
        talonGunInAnimation.addSprite("talon+gun+in", -map.getImageSize() * 1.0, -map.getImageSize() * 1.6, 0, 1.33);
        talonGunInAnimation.addSpriteAnimTable("talon+gun+in+mask", -map.getImageSize() * 1.0, -map.getImageSize() * 1.6, owner, 0, 1.33, 1.33, 0, 0, GameEnums.Recoloring_Matrix);
        talonGunAnimation.queueAnimation(talonGunInAnimation);
    };
    this.performPostAnimation = function(postAnimation, map)
    {

    };
    this.getDescription = function()
    {
        return qsTr("Fire with Talon Gun");
    };
}

Constructor.prototype = ACTION;
var ACTION_TALON_GUN = new Constructor();
