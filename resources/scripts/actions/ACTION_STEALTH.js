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
            if (unit.getHidden() === false)
            {
                return true;
            }
        }
        return false;
    };
    this.getActionText = function()
    {
        return qsTr("Stealth");
    };
    this.getIcon = function()
    {
        return "dive";
    };
    this.isFinalStep = function(action)
    {
        return true;
    };
    this.perform = function(action)
    {
        // we need to move the unit to the target position
        var unit = action.getTargetUnit();
        var animation = Global[unit.getUnitID()].doWalkingAnimation(action);
        animation.setEndOfAnimationCall("ACTION_STEALTH", "performPostAnimation");
        // move unit to target position
        unit.moveUnitAction(action);
        ACTION_STEALTH.postAnimationUnit = unit;

    };
    this.postAnimationUnit = null;
    this.performPostAnimation = function(action)
    {
        // disable unit commandments for this turn
        var animation = GameAnimationFactory.createAnimation(ACTION_STEALTH.postAnimationUnit.getX(), ACTION_STEALTH.postAnimationUnit.getY());
        if (ACTION_STEALTH.postAnimationUnit.getUnitID() === "SUBMARINE" ||
            ACTION_STEALTH.postAnimationUnit.getUnitID() === "ZCOUNIT_MISSILE_SUB")
        {
            animation.addSprite("dive", -map.getImageSize() / 2, -map.getImageSize() / 2, 0, 1.5);
            animation.setSound("dive.wav", 1);
        }
        else
        {
            animation.addSprite("stealth", -map.getImageSize() / 2, -map.getImageSize() / 2, 0, 1.5);
            animation.setSound("stealth.wav", 1);
        }
        ACTION_STEALTH.postAnimationUnit.setHasMoved(true);
        ACTION_STEALTH.postAnimationUnit.setHidden(true);
        ACTION_STEALTH.postAnimationUnit = null;
    };
    this.getDescription = function()
    {
        return "<r>" + qsTr("Stealths a unit and makes it invisible to all enemies. A stealthed unit is only visible when a unit is nearby it. In addition only units of the same Unit Type ") + "</r>" +
                "<div c='#00ff00'>" + qsTr(" Ground, Naval or Air") + "</div>" +
                "<r>" + qsTr(" can attack it. In most cases stealthed units have a higher fuel consumption per turn, while beeing stealthed.") + "</r>";
    };
}

Constructor.prototype = ACTION;
var ACTION_STEALTH = new Constructor();
