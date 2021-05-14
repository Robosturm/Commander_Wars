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
        return true;
    };
    this.getActionText = function()
    {
        return qsTr("Cloak");
    };
    this.getIcon = function()
    {
        return "dive";
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
        data.setColor("#C800FF00");
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
    this.perform = function(action)
    {
        // we need to move the unit to the target position
        ACTION_SUPPORTALL_CLOAK.postAnimationUnit = action.getTargetUnit();
        var animation = Global[ACTION_SUPPORTALL_CLOAK.postAnimationUnit.getUnitID()].doWalkingAnimation(action);
        animation.setEndOfAnimationCall("ACTION_SUPPORTALL_CLOAK", "performPostAnimation");
        // move unit to target position
        ACTION_SUPPORTALL_CLOAK.postAnimationUnit.moveUnitAction(action);
        ACTION_SUPPORTALL_CLOAK.postAnimationUnit.setHasMoved(true);
    };
    this.performPostAnimation = function(postAnimation)
    {
        var fields = globals.getCircle(0, 2);
        var x = ACTION_SUPPORTALL_CLOAK.postAnimationUnit.getX();
        var y = ACTION_SUPPORTALL_CLOAK.postAnimationUnit.getY();
        var owner = ACTION_SUPPORTALL_CLOAK.postAnimationUnit.getOwner();
        var playerId  = owner.getPlayerID();
        for (var i = 0; i < fields.size(); i++)
        {
            var point = fields.at(i);
            if (map.onMap(x + point.x, y + point.y))
            {
                var terrain = map.getTerrain(x + point.x, y + point.y);
                var unit = terrain.getUnit();
                if (unit !== null &&
                    owner === unit.getOwner())
                {
                    unit.setCloaked(1);
                    var animation = GameAnimationFactory.createAnimation(unit.getX(), unit.getY());
                    animation.addSprite("stealth", -map.getImageSize() / 2, -map.getImageSize() / 2, 0, 1.5);
                    animation.setSound("stealth.wav", 1);
                }
            }
        }
        fields.remove();
    };
    this.getDescription = function()
    {
        return qsTr("Cloaks all own units.");
    };
}

Constructor.prototype = ACTION;
var ACTION_SUPPORTALL_CLOAK = new Constructor();
