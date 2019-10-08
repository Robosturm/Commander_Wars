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
            return true;
        }
        else
        {
            return false;
        }
    };
    this.getActionText = function()
    {
        return qsTr("Explode");
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
        cursorData.setCursor("cursor+missile");
        cursorData.setXOffset(- map.getImageSize() * 2);
        cursorData.setYOffset(- map.getImageSize() * 2);
        cursorData.setScale(1.5);
    };
    this.getStepData = function(action, data)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        data.setColor("#C8FF0000");
        data.addPoint(Qt.point(actionTargetField.x, actionTargetField.y));
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
    this.postAnimationUnit = null;
    this.postAnimationTargetX = -1;
    this.postAnimationTargetY = -1;

    this.perform = function(action)
    {
        // we need to move the unit to the target position
        var unit = action.getTargetUnit();
        var animation = Global[unit.getUnitID()].doWalkingAnimation(action);
        animation.setEndOfAnimationCall("ACTION_EXPLODE", "performPostAnimation");
        // move unit to target position
        unit.moveUnitAction(action);
        action.startReading();
        // read action data
        ACTION_EXPLODE.postAnimationUnit = unit;
        ACTION_EXPLODE.postAnimationTargetX = action.readDataInt32();
        ACTION_EXPLODE.postAnimationTargetY = action.readDataInt32();
    };
    this.performPostAnimation = function(postAnimation)
    {
        var owner = ACTION_EXPLODE.postAnimationUnit.getOwner();
        var x = ACTION_EXPLODE.postAnimationTargetX;
        var y = ACTION_EXPLODE.postAnimationTargetY;
        var fields = globals.getCircle(1, 2);
        for (var i = 0; i < fields.size(); i++)
        {
            var point = fields.at(i);
            if (map.onMap(x + point.x, y + point.y))
            {
                var terrain = map.getTerrain(x + point.x, y + point.y);
                var unit = terrain.getUnit();
                if (unit !== null)
                {
                    unit.setHp(unit.getHpRounded() - 4);
                    if (unit.getHp() <= 0)
                    {
                        // we destroyed a unit
                        map.getGameRecorder().destroyedUnit(unit.getOwner().getPlayerID());
                        unit.killUnit();
                    }
                }                
            }
        }
        fields.remove();
        ACTION_EXPLODE.postAnimationUnit.killUnit();
        var animation = GameAnimationFactory.createAnimation(x - 2, y - 3);
        animation.addSprite("explosion+black_bomb", 0, map.getImageSize() / 2, 0, 1.875);
        audio.playSound("explosion+land.wav");
        // we destroyed a unit
        map.getGameRecorder().destroyedUnit(owner.getPlayerID());
        ACTION_EXPLODE.postAnimationUnit = null;
        ACTION_EXPLODE.postAnimationTargetX = -1;
        ACTION_EXPLODE.postAnimationTargetY = -1;
    };
}

Constructor.prototype = ACTION;
var ACTION_EXPLODE = new Constructor();
