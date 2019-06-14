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
        animation.addSprite("trap", map.getImageSize() / 2, map.getImageSize() / 3, 400);
        animation.addText(qsTr("TRAP!"), map.getImageSize() / 2 + 15, map.getImageSize() / 3, 0.7);
        audio.playSound("trap.wav");
        ACTION_TRAP.postAnimationTrapSignX = -1;
        ACTION_TRAP.postAnimationTrapSignY = -1;
    }
}

Constructor.prototype = ACTION;
var ACTION_TRAP = new Constructor();
