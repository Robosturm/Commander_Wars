var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
        if (ACTION.isEmptyFieldAndHasNotMoved(action, unit, actionTargetField, targetField, map))
        {
            if (unit.getHidden() === true)
            {
                return true;
            }
        }
        return false;
    };
    this.getActionText = function(map)
    {
        return qsTr("Unstealth");
    };
    this.getIcon = function(map)
    {
        return "unhide";
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
        animation.setEndOfAnimationCall("ACTION_UNSTEALTH", "performPostAnimation");
        // move unit to target position
        Global[unit.getUnitID()].moveUnit(unit, action, map);
        ACTION_UNSTEALTH.postAnimationUnit = unit;

    };
    this.postAnimationUnit = null;
    this.performPostAnimation = function(action, map)
    {
        // disable unit commandments for this turn
        var viewPlayer = map.getCurrentViewPlayer();
        if (viewPlayer === ACTION_UNSTEALTH.postAnimationUnit.getOwner() || viewPlayer.getFieldVisible(ACTION_UNSTEALTH.postAnimationUnit.getX(), ACTION_UNSTEALTH.postAnimationUnit.getY()))
        {
            var animation = GameAnimationFactory.createAnimation(map, ACTION_UNSTEALTH.postAnimationUnit.getX(), ACTION_UNSTEALTH.postAnimationUnit.getY());
            if (ACTION_UNSTEALTH.postAnimationUnit.getUnitID() === "SUBMARINE" ||
                    ACTION_UNSTEALTH.postAnimationUnit.getUnitID() === "ZCOUNIT_MISSILE_SUB")
            {
                animation.addSprite("undive", -map.getImageSize() / 2, -map.getImageSize() / 2, 0, 2);
                animation.setSound("undive.wav", 1,);
            }
            else
            {
                animation.addSprite("stealth", -map.getImageSize() / 2, -map.getImageSize() / 2, 0, 2);
                animation.setSound("unstealth.wav", 1,);
            }
        }

        ACTION_UNSTEALTH.postAnimationUnit.setHasMoved(true);
        ACTION_UNSTEALTH.postAnimationUnit.setHidden(false);
        ACTION_UNSTEALTH.postAnimationUnit = null;
    };
    this.getName = function()
    {
        return qsTr("Unstealth");
    };
    this.getDescription = function()
    {
        return qsTr("Unstealths a unit and makes it visible to all enemies.");
    };
}

Constructor.prototype = ACTION;
var ACTION_UNSTEALTH = new Constructor();
