var Constructor = function()
{
    
    this.canBePerformed = function(action, map)
    {
        var building = action.getTargetBuilding();
        if (building.getFireCount() >= 2)
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
    this.postAnimationTargetX = -1;
    this.postAnimationTargetY = -1;
    this.postAnimationBuildingX = -1;
    this.postAnimationBuildingY = -1;
    this.radius = 2;
    this.perform = function(action, map)
    {
        // we need to move the unit to the target position
        var building = action.getTargetBuilding();
        building.setFireCount(0);
        var x = building.getX();
        var y = building.getY();
        ACTION_NEST_MORTAR.postAnimationBuildingX = x;
        ACTION_NEST_MORTAR.postAnimationBuildingY = y;
        building.setFireCount(0);
        var damage = Global[building.getBuildingID()].getDamage();
        var owner = building.getOwner();
        var fireArea = Global[building.getBuildingID()].getAbsoluteActionTargetFields(building);
        var mortarTarget = owner.getRockettarget(ACTION_NEST_MORTAR.radius, damage, 1.2, GameEnums.RocketTarget_Money, fireArea);
        ACTION_NEST_MORTAR.postAnimationTargetX = mortarTarget.x;
        ACTION_NEST_MORTAR.postAnimationTargetY = mortarTarget.y;

        var animation = GameAnimationFactory.createAnimation(map, ACTION_NEST_MORTAR.postAnimationTargetX - ACTION_NEST_MORTAR.radius, ACTION_NEST_MORTAR.postAnimationTargetY - ACTION_NEST_MORTAR.radius - 1);
        animation.addSprite("explosion+silo", -map.getImageSize() / 2, 0, 0, 2, 0);
        animation.setSound("nestmortardetonation.wav");
        animation.setEndOfAnimationCall("ACTION_NEST_MORTAR", "performPostAnimation");
    };
    this.performPostAnimation = function(postAnimation, map)
    {
        var targetX = ACTION_NEST_MORTAR.postAnimationTargetX;
        var targetY = ACTION_NEST_MORTAR.postAnimationTargetY;
        var x = ACTION_NEST_MORTAR.postAnimationBuildingX;
        var y = ACTION_NEST_MORTAR.postAnimationBuildingY;
        var building = map.getTerrain(x, y).getBuilding();
        var damage = Global[building.getBuildingID()].getDamage();
        var targets = globals.getCircle(0, ACTION_NEST_MORTAR.radius);
        var size = targets.size();
        var specialDestruction = map.getGameRules().getSpecialDestruction();
        for (var i = 0; i < size; i++)
        {
            var point = targets.at(i);
            if (map.onMap(targetX + point.x, targetY + point.y))
            {
                var unit = map.getTerrain(targetX + point.x, targetY + point.y).getUnit();
                if (unit !== null)
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
        ACTION_NEST_MORTAR.postAnimationTargetX = -1;
        ACTION_NEST_MORTAR.postAnimationTargetY = -1;
        ACTION_NEST_MORTAR.postAnimationBuildingX = -1;
        ACTION_NEST_MORTAR.postAnimationBuildingY = -1;
    };
    this.getName = function()
    {
        return qsTr("Fire with nest mortar");
    };
    this.getDescription = function()
    {
        return qsTr("Commands the mortar to fire at a 3x3 square and deals damage to all units.");
    };
}

Constructor.prototype = ACTION;
var ACTION_NEST_MORTAR = new Constructor();
