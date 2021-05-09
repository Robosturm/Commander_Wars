var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        return false;
    };
    // this action can't be performed by the player it's performed when moving over a stealthed unit by the game itself
    // it replaces the actual action that should have been performed
    this.perform = function(action)
    {
        // we need to move the unit to the target position
        var unit = action.getTargetUnit();
        var animation = Global[unit.getUnitID()].doWalkingAnimation(action);
        // move unit to target position
        unit.moveUnitAction(action);
        // disable unit commandments for this turn
        unit.setHasMoved(true);
        animation.setEndOfAnimationCall("ACTION_TRAP", "performPostAnimation");
        action.startReading();
        ACTION_TRAP.postAnimationTrapSignX = action.readDataInt32();
        ACTION_TRAP.postAnimationTrapSignY = action.readDataInt32();
    };
    this.postAnimationTrapSignX = -1;
    this.postAnimationTrapSignY = -1;
    this.performPostAnimation = function(postAnimation)
    {
        var animation = GameAnimationFactory.createAnimation(ACTION_TRAP.postAnimationTrapSignX, ACTION_TRAP.postAnimationTrapSignY);
        var width = animation.addText(qsTr("TRAP!"), map.getImageSize() / 2 + 25, 2, 1);
        animation.addBox("info", map.getImageSize() / 2, 0, width + 32, map.getImageSize(), 400);
        animation.addSprite("trap", map.getImageSize() / 2 + 8, 1, 400, 1.7);
        audio.playSound("trap.wav");
        ACTION_TRAP.postAnimationTrapSignX = -1;
        ACTION_TRAP.postAnimationTrapSignY = -1;
    };
    this.getDescription = function()
    {
        return qsTr("This action can't be disabled.");
    };
    this.isTrap = function(action, moveUnit, targetFieldUnit, targetX, targetY, previousX, previousY, moveCost)
    {
        // used to determine if a trap is in the move path.
        // the engine takes care of checking the path in the correct order and cutting the path.
        if ((targetFieldUnit !== null &&
             targetFieldUnit.isStealthed(map.getCurrentPlayer()) &&
             !targetFieldUnit.getIgnoreUnitCollision()) ||
             moveCost < 0)
        {
            return true;
        }
        return false;
    };

    this.isStillATrap = function(action, moveUnit, targetFieldUnit, targetX, targetY, previousX, previousY, moveCost)
    {
        // this function gets called to find a field at which the unit can actually stop it's movement
        // E.g. don't get trapped on a teleport tile or getting trapped on an allied unit etc.
        if (targetFieldUnit !== null || moveCost <= 0)
        {
            return true;
        }
        return false;
    };

}

Constructor.prototype = ACTION;
var ACTION_TRAP = new Constructor();
