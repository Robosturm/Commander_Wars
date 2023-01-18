var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
        if (ACTION.isEmptyFieldAndHasNotMoved(action, unit, actionTargetField, targetField, map))
        {
            return true;
        }
        return false;
    };
    this.getActionText = function(map)
    {
        return qsTr("Cloak");
    };
    this.getIcon = function(map)
    {
        return "dive";
    };
    this.getStepInputType = function(action, map)
    {
        return "FIELD";
    };
    this.getStepCursor = function(action, cursorData, map)
    {
        cursorData.setCursor("cursor+missile");
        cursorData.setXOffset(- map.getImageSize() * 2);
        cursorData.setYOffset(- map.getImageSize() * 2);
        cursorData.setScale(2);
    };
    this.getStepData = function(action, data, map)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        data.setColor("#C800FF00");
        data.addPoint(Qt.point(actionTargetField.x, actionTargetField.y));
    };
    this.isFinalStep = function(action, map)
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
    this.perform = function(action, map)
    {
        // we need to move the unit to the target position
        ACTION_SUPPORTALL_CLOAK.postAnimationUnit = action.getTargetUnit();
        var animation = Global[ACTION_SUPPORTALL_CLOAK.postAnimationUnit.getUnitID()].doWalkingAnimation(action, map);
        animation.setEndOfAnimationCall("ACTION_SUPPORTALL_CLOAK", "performPostAnimation");
        // move unit to target position
        ACTION_SUPPORTALL_CLOAK.postAnimationUnit.moveUnitAction(action);
        ACTION_SUPPORTALL_CLOAK.postAnimationUnit.setHasMoved(true);
    };
    this.performPostAnimation = function(postAnimation, map)
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
                    var animation = GameAnimationFactory.createAnimation(map, unit.getX(), unit.getY());
                    animation.addSprite("stealth", -map.getImageSize() / 2, -map.getImageSize() / 2, 0, 2);
                    animation.setSound("stealth.wav", 1);
                }
            }
        }
    };
    this.getName = function()
    {
        return qsTr("Cloak");
    };
    this.getDescription = function()
    {
        return qsTr("Cloaks all own units in a 2x2 square. A cloaked unit is only visible when a unit is nearby it, but can be attacked normally.");
    };
}

Constructor.prototype = ACTION;
var ACTION_SUPPORTALL_CLOAK = new Constructor();
