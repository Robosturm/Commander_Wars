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
    this.postAnimationActionFields = null;
    this.postAnimationActionX = -1;
    this.postAnimationActionY = -1;
    this.perform = function(action, map)
    {
        // we need to move the unit to the target position
        var building = action.getTargetBuilding();
        var x = building.getX();
        var y = building.getY();

        building.setFireCount(building.getFireCount() - 1);
        var fields = Global[building.getBuildingID()].getActionTargetFields(building);
        var animation = GameAnimationFactory.createAnimation(map, x, y);
        animation.addSprite("laserray+center+start", -map.getImageSize() * 1, -map.getImageSize() * 1, 0, 2);

        var animation2 = GameAnimationFactory.createAnimation(map, x, y);
        animation2.addSprite("laserray+center", -map.getImageSize() * 1, -map.getImageSize() * 1, 0, 2);
        animation2.addSound("laserFire.wav");
        animation.queueAnimation(animation2);
        animation2.setEndOfAnimationCall("ACTION_LASER_FIRE", "performPostAnimation");
        var size = fields.size();
        for (var i = 0; i < size; i++)
        {
            var point = fields.at(i);
            if (map.onMap(x + point.x, y + point.y))
            {
                if (point.x === 0)
                {
                    if (point.y > 1)
                    {
                        animation2 = GameAnimationFactory.createAnimation(map, x + point.x, y + point.y);
                        animation2.addSprite("laserray", -map.getImageSize() * 0.4 , -map.getImageSize() * 1.45, 0, 2);
                        animation2.setRotation(90);
                        animation.queueAnimation(animation2);
                    }
                    else if (point.y < -1)
                    {
                        animation2 = GameAnimationFactory.createAnimation(map, x + point.x, y + point.y);
                        animation2.addSprite("laserray", 0, -map.getImageSize() * 1.45, 0, 2);
                        animation2.setRotation(90);
                        animation.queueAnimation(animation2);
                    }
                }
                else
                {
                    if (point.x > 1)
                    {
                        animation2 = GameAnimationFactory.createAnimation(map, x + point.x, y + point.y);
                        animation2.addSprite("laserray", -map.getImageSize() * 0.15, -map.getImageSize() * 0.6, 0, 2);
                        animation.queueAnimation(animation2);
                    }
                    else if (point.x < -1)
                    {
                        animation2 = GameAnimationFactory.createAnimation(map, x + point.x, y + point.y);
                        animation2.addSprite("laserray", 0, -map.getImageSize() * 0.6, 0, 2);
                        animation.queueAnimation(animation2);
                    }
                }


            }
        }
        ACTION_LASER_FIRE.postAnimationActionX = x;
        ACTION_LASER_FIRE.postAnimationActionY = y;
    };
    this.performPostAnimation = function(postAnimation, map)
    {
        var x = ACTION_LASER_FIRE.postAnimationActionX;
        var y = ACTION_LASER_FIRE.postAnimationActionY;
        var building = map.getTerrain(x, y).getBuilding();
        var damage = Global[building.getBuildingID()].getDamage();
        var fields = Global[building.getBuildingID()].getActionTargetFields(building);
        var size = fields.size();
        var specialDestruction = map.getGameRules().getSpecialDestruction();
        for (var i = 0; i < size; i++)
        {
            var point = fields.at(i);
            if (map.onMap(x + point.x, y + point.y))
            {
                var unit = map.getTerrain(x + point.x, y + point.y).getUnit();
                if ((unit !== null))
                {
                    var newHp = unit.getHpRounded() - damage;
                    if (!specialDestruction && newHp <= 0.1)
                    {
                        newHp = 0.1;
                    }
                    unit.setHp(newHp);
                    if (unit.getHp() <= 0)
                    {
                        unit.killUnit();
                    }
                }
            }
        }
        ACTION_LASER_FIRE.postAnimationActionX = -1;
        ACTION_LASER_FIRE.postAnimationActionY = -1;
    };
    this.getName = function()
    {
        return qsTr("Fire with laser");
    };
    this.getDescription = function()
    {
        return qsTr("Commands the laser to shot four laser rays. The laser rays deal damage to all units that get hit by one of the rays.");
    };
}

Constructor.prototype = ACTION;
var ACTION_LASER_FIRE = new Constructor();
