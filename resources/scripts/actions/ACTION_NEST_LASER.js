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
        building.setFireCount(0);
        var x = building.getX();
        var y = building.getY();
        var damage = Global[building.getBuildingID()].getDamage();
        var targets = Global[building.getBuildingID()].getActionTargetFields(building);
        var rotation = Global[building.getBuildingID()].getRotation(building);
        var scale = 2;
        if (rotation !== 0)
        {
            scale = 2.85;
        }
        var size = targets.size()
        var specialDestruction = map.getGameRules().getSpecialDestruction();
        for (var i = 0; i < size; i++)
        {
            var point = targets.at(i);
            if (map.onMap(x + point.x, y + point.y))
            {
                var offsetX = 0;
                if (i === 0)
                {
                    var scaleModifierX = 0;
                    var scaleModifierY = 0;
                    if (rotation < 0)
                    {
                        scaleModifierX += 0.70;
                        scaleModifierY += 0.20;
                    }
                    else if (rotation > 0)
                    {
                        offsetX = 1;
                        scaleModifierX += 0.70;
                        scaleModifierY += 0.20;
                    }
                    var animation = GameAnimationFactory.createAnimation(map, x + point.x + offsetX, y + point.y);

                    animation.addSprite("nestlaser+center", -map.getImageSize() * (2 + scaleModifierX), -map.getImageSize() * (1.76 + scaleModifierY), 0, scale);
                    animation.setRotation(90 + rotation);
                    animation.addSound("nestlaserfire.wav");
                }
                else
                {
                    if (rotation > 0)
                    {
                        offsetX = 1;
                    }
                    var animation = GameAnimationFactory.createAnimation(map, x + point.x + offsetX, y + point.y);
                    animation.addSprite("nestlaser", -map.getImageSize() * 0.4 , -map.getImageSize() * 1.45, 0, scale);
                    animation.setRotation(90 + rotation);
                    animation.addSound("nestlaserfire.wav");
                }
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
    };
    this.getName = function()
    {
        return qsTr("Fire with nest laser");
    };
    this.getDescription = function()
    {
        return qsTr("Commands the laser to shot a ray. The laser rays deal damage to all units that get hit.");
    };
}

Constructor.prototype = ACTION;
var ACTION_NEST_LASER = new Constructor();
