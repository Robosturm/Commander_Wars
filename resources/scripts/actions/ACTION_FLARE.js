var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
        if ((unit.getHasMoved() === true) ||
                (unit.getBaseMovementCosts(actionTargetField.x, actionTargetField.y) <= 0))
        {
            return false;
        }
        if ((actionTargetField.x === targetField.x) && (actionTargetField.y === targetField.y) ||
                (action.getMovementTarget() === null))
        {
            if (unit.hasAmmo1())
            {
                return true;
            }
        }
    };
    this.getActionText = function()
    {
        return qsTr("Flare");
    };
    this.getIcon = function()
    {
        return "flareshot";
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
    this.getStepInputType = function(action)
    {
        return "FIELD";
    };

    this.getMinRange = function()
    {
        return 1;
    }

    this.getMaxRange = function()
    {
        return 5;
    }
    this.getUnfogRange = function()
    {
        return 2;
    }

    this.getStepData = function(action, data)
    {
        var unit = action.getTargetUnit();
        var targetField = action.getTarget();
        var actionTargetField = action.getActionTarget();
        var fields = globals.getCircle(ACTION_FLARE.getMinRange(),
                                       ACTION_FLARE.getMaxRange());
        data.setColor("#C8FF0000");
        data.setZLabelColor("#ff4500");
        data.setShowZData(false);
        // check all fields we can attack
        for (var i = 0; i < fields.size(); i++)
        {
            var x = fields.at(i).x + actionTargetField.x;
            var y = fields.at(i).y + actionTargetField.y;
            if (map.onMap(x, y))
            {
                data.addPoint(Qt.point(x, y));
            }
        }
        fields.remove();
    };
    this.postAnimationTargetX = -1;
    this.postAnimationTargetY = -1;
    this.postAnimationPlayer = null;
    this.perform = function(action)
    {
        // we need to move the unit to the target position
        var unit = action.getTargetUnit();
        var animation = Global[unit.getUnitID()].doWalkingAnimation(action);
        animation.setEndOfAnimationCall("ACTION_FLARE", "performPostAnimation");
        // move unit to target position
        unit.moveUnitAction(action);
        // disable unit commandments for this turn
        unit.setHasMoved(true);
        unit.reduceAmmo1(1);
        action.startReading();
        // read action data
        ACTION_FLARE.postAnimationTargetX = action.readDataInt32();
        ACTION_FLARE.postAnimationTargetY = action.readDataInt32();
        ACTION_FLARE.postAnimationPlayer = unit.getOwner();
    };
    this.performPostAnimation = function(postAnimation)
    {
        var animation = GameAnimationFactory.createAnimation(ACTION_FLARE.postAnimationTargetX, ACTION_FLARE.postAnimationTargetY);
        animation.addSprite("flare_explosion", -map.getImageSize() - 36, -map.getImageSize() - 37, 0, 1.5);
        animation.addSound("flare_launch.wav");
        animation.addSound("flare_explosion.wav", 1, "resources/sounds/", 300);
        var fields = globals.getCircle(0, ACTION_FLARE.getUnfogRange());
        for (var i = 0; i < fields.size(); i++)
        {
            var x = fields.at(i).x + ACTION_FLARE.postAnimationTargetX;
            var y = fields.at(i).y + ACTION_FLARE.postAnimationTargetY;
            if (map.onMap(x, y))
            {
                ACTION_FLARE.postAnimationPlayer.addVisionField(x, y, 1, true);
            }
        }
        ACTION_FLARE.postAnimationTargetX = -1;
        ACTION_FLARE.postAnimationTargetY = -1;
        ACTION_FLARE.postAnimationPlayer = null;
    };
    this.getStepCursor = function(action, cursorData)
    {
        cursorData.setCursor("cursor+missile");
        cursorData.setXOffset(- map.getImageSize() * 2);
        cursorData.setYOffset(- map.getImageSize() * 2);
        cursorData.setScale(1.5);
    };
    this.getDescription = function()
    {
        return qsTr("Fires a flare rocket at the given location. The flare rocket will reveal all terrains in an area around the target during fog of war.");
    };
}

Constructor.prototype = ACTION;
var ACTION_FLARE = new Constructor();
