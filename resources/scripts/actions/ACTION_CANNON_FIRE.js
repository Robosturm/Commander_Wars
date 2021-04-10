var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        var building = action.getTargetBuilding();
        if (building.getFireCount() >= 1)
        {
            if (ACTION_CANNON_FIRE.getTargets(building).length > 0)
            {
                return true;
            }
        }
        return false;
    };
    this.getActionText = function()
    {
        return qsTr("Fire");
    };
    this.getIcon = function()
    {
        return "fire";
    };
    this.getStepInputType = function(action)
    {
        return "FIELD";
    };
    this.getStepCursor = function(action, cursorData)
    {
        cursorData.setCursor("cursor+attack");
        cursorData.setXOffset(- map.getImageSize() / 3);
        cursorData.setYOffset(- map.getImageSize() / 3);
        cursorData.setScale(1.5);
    };
    this.isFinalStep = function(action)
    {
        if (action.getInputStep() === 1)
        {
            return true;
        }
        else
        {
            return false;
        }
    };
    this.getTargets = function(building)
    {
        var offset = Global[building.getBuildingID()].getActionTargetOffset(building);
        var x = building.getX() + offset.x;
        var y = building.getY() + offset.y;
        var fields = Global[building.getBuildingID()].getActionTargetFields(building);
        var ret = [];
        for (var i = 0; i < fields.size(); i++)
        {
            var point = fields.at(i);
            if (map.onMap(x + point.x, y + point.y))
            {
                var unit = map.getTerrain(x + point.x, y + point.y).getUnit();
                if ((unit !== null) &&
                    (building.getOwner().isEnemyUnit(unit)))
                {
                    if (building.getOwner().getFieldVisible(unit.getX(), unit.getY()))
                    {
                        if (!unit.isStealthed(building.getOwner()))
                        {
                            ret.push(Qt.point(x + point.x, y + point.y));
                        }
                    }
                }
            }
        }
        fields.remove();
        return ret;
    };
    this.getStepData = function(action, data)
    {
        var building = action.getTargetBuilding();
        var targets = ACTION_CANNON_FIRE.getTargets(building);
        for (var i = 0; i < targets.length; i++)
        {
            var damage = Global[building.getBuildingID()].getDamage(building,
                                                                    map.getTerrain(targets[i].x, targets[i].y).getUnit());
            data.addPoint(Qt.point(targets[i].x, targets[i].y));
            data.addZInformation(damage * 10);
        }
        data.setColor("#C8FF0000");
        data.setZLabelColor("#ff4500");
        data.setZLabelText(qsTr("Damage"))
        data.setShowZData(true);
    };

    this.perform = function(action)
    {
        // we need to move the unit to the target position
        var building = action.getTargetBuilding();
        building.setFireCount(building.getFireCount() - 1);
        action.startReading();
        var targetX = action.readDataInt32();
        var targetY = action.readDataInt32();
        ACTION_CANNON_FIRE.postAnimationBuilding = building;
        ACTION_CANNON_FIRE.postAnimationUnit = map.getTerrain(targetX, targetY).getUnit();
        var animation = Global[building.getBuildingID()].getShotAnimation(building);
        var animation2 = GameAnimationFactory.createAnimation(targetX, targetY, 70);
        animation2.addSprite("blackhole_shot", -map.getImageSize() * 0.5, -map.getImageSize() * 0.5, 0, 1.5);
        animation2.setSound("minicanon_hit.wav");
        animation2.setEndOfAnimationCall("ACTION_CANNON_FIRE", "performPostAnimation");
        animation.queueAnimation(animation2);
    };
    var postAnimationUnit = null;
    var postAnimationBuilding = null;
    this.performPostAnimation = function()
    {
        var damage = Global[ACTION_CANNON_FIRE.postAnimationBuilding.getBuildingID()].getDamage(ACTION_CANNON_FIRE.postAnimationBuilding,
                                                                                                ACTION_CANNON_FIRE.postAnimationUnit);
        ACTION_CANNON_FIRE.postAnimationUnit.setHp(ACTION_CANNON_FIRE.postAnimationUnit.getHpRounded() - damage);
        if (ACTION_CANNON_FIRE.postAnimationUnit.getHp() <= 0)
        {
            ACTION_CANNON_FIRE.postAnimationUnit.killUnit();
        }
        postAnimationUnit = null;
        postAnimationBuilding = null;
    };
    this.getDescription = function()
    {
        return qsTr("Orders a minicannon or black cannon to attack a single unit to deal some damage to it.");
    };
}

Constructor.prototype = ACTION;
var ACTION_CANNON_FIRE = new Constructor();
