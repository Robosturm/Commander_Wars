var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
        if (ACTION.isEmptyFieldAndHasNotMoved(action, unit, actionTargetField, targetField, map))
        {
            if (unit.getHidden() === false)
            {
                return true;
            }
        }
        return false;
    };
    this.getActionText = function(map)
    {
        return qsTr("Stealth");
    };
    this.getIcon = function(map)
    {
        return "dive";
    };
    this.isFinalStep = function(action, map)
    {
        return true;
    };
    this.perform = function(action, map)
    {
        // we need to move the unit to the target position
        var unit = action.getTargetUnit();
        var animation = Global[unit.getUnitID()].doWalkingAnimation(action, map);
        animation.setEndOfAnimationCall("ACTION_STEALTH", "performPostAnimation");
        // move unit to target position
        Global[unit.getUnitID()].moveUnit(unit, action, map);
        ACTION_STEALTH.postAnimationUnit = unit;

    };
    this.postAnimationUnit = null;
    this.performPostAnimation = function(action, map)
    {
        // disable unit commandments for this turn
        var animation = GameAnimationFactory.createAnimation(map, ACTION_STEALTH.postAnimationUnit.getX(), ACTION_STEALTH.postAnimationUnit.getY());
        if (ACTION_STEALTH.postAnimationUnit.getUnitID() === "SUBMARINE" ||
            ACTION_STEALTH.postAnimationUnit.getUnitID() === "ZCOUNIT_MISSILE_SUB")
        {
            animation.addSprite("dive", -map.getImageSize() / 2, -map.getImageSize() / 2, 0, 2);
            animation.setSound("dive.wav", 1);
        }
        else
        {
            animation.addSprite("stealth", -map.getImageSize() / 2, -map.getImageSize() / 2, 0, 2);
            animation.setSound("stealth.wav", 1);
        }
        ACTION_STEALTH.postAnimationUnit.setHasMoved(true);
        ACTION_STEALTH.postAnimationUnit.setHidden(true);
        ACTION_STEALTH.postAnimationUnit = null;
    };
    this.getName = function()
    {
        return qsTr("Stealth");
    };
    this.getDescription = function()
    {
        return qsTr("<r>Stealths a unit and makes it invisible to all enemies. A stealthed unit is only visible when an enemy unit is adjacent. In addition, only direct-attack units can target a stealthed </r><div c='#00ff00'>air unit</div><r>. A </r><div c='#00ff00'>naval unit</div><r> can only be attacked by a direct-attack </r><div c='#00ff00'>naval unit</div><r>. In most cases stealthed units have a higher fuel consumption per turn, while beeing stealthed.</r>");
    };
}

Constructor.prototype = ACTION;
var ACTION_STEALTH = new Constructor();
