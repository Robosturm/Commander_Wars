var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
        if (ACTION.isEmptyFieldAndHasNotMoved(action, unit, actionTargetField, targetField, map) &&
            ACTION_SUPPORTSINGLE_IRON_SHIELD.getDefenseFields(action, map).length > 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    };
    this.getActionText = function(map)
    {
        return qsTr("Iron Shield");
    };
    this.getIcon = function(map)
    {
        return "defenseStar";
    };
    this.getStepInputType = function(action, map)
    {
        return "FIELD";
    };
    this.getStepData = function(action, data, map)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        data.setColor("#C800FF00");
        var fields = ACTION_SUPPORTSINGLE_IRON_SHIELD.getDefenseFields(action, map);
        for (var i3 = 0; i3 < fields.length; i3++)
        {
            data.addPoint(Qt.point(fields[i3].x, fields[i3].y));
        }
    };
    this.getDefenseFields = function(action, map)
    {
        var targetField = action.getActionTarget();
        var targetFields = [Qt.point(targetField.x + 1, targetField.y),
                            Qt.point(targetField.x - 1, targetField.y),
                            Qt.point(targetField.x,     targetField.y - 1),
                            Qt.point(targetField.x,     targetField.y + 1)];
        // check all neighbour terrains
        var unit = action.getTargetUnit();
        var ret = [];
        for (var i = 0; i < targetFields.length; i++)
        {
            if (map.onMap(targetFields[i].x, targetFields[i].y))
            {
                var terrain = map.getTerrain(targetFields[i].x, targetFields[i].y);
                var repairUnit = terrain.getUnit();
                // can we repair the unit?
                if (repairUnit !== null &&
                    repairUnit.getOwner() === unit.getOwner() &&
                    repairUnit !== unit)
                {
                    ret.push(targetFields[i]);
                }
            }
        }
        return ret;
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
    this.postAnimationTargetX = -1;
    this.postAnimationTargetY = -1;
    this.perform = function(action, map)
    {
        // we need to move the unit to the target position
        var unit = action.getTargetUnit();
        var animation = Global[unit.getUnitID()].doWalkingAnimation(action, map);
        animation.setEndOfAnimationCall("ACTION_SUPPORTSINGLE_IRON_SHIELD", "performPostAnimation");
        // move unit to target position
        Global[unit.getUnitID()].moveUnit(unit, action, map);
        // disable unit commandments for this turn
        action.startReading();
        // read action data
        ACTION_SUPPORTSINGLE_IRON_SHIELD.postAnimationUnit = unit;
        ACTION_SUPPORTSINGLE_IRON_SHIELD.postAnimationTargetX = action.readDataInt32();
        ACTION_SUPPORTSINGLE_IRON_SHIELD.postAnimationTargetY = action.readDataInt32();
    };
    this.performPostAnimation = function(postAnimation, map)
    {
        var terrain = map.getTerrain(ACTION_SUPPORTSINGLE_IRON_SHIELD.postAnimationTargetX, ACTION_SUPPORTSINGLE_IRON_SHIELD.postAnimationTargetY);
        var defenseUnit = terrain.getUnit();
        if (!defenseUnit.isStealthed(map.getCurrentViewPlayer()))
        {
            var animation = GameAnimationFactory.createAnimation(map, ACTION_SUPPORTSINGLE_IRON_SHIELD.postAnimationTargetX, ACTION_SUPPORTSINGLE_IRON_SHIELD.postAnimationTargetY);
            var width = animation.addText(qsTr("DEFENSE"), map.getImageSize() / 2 + 25, -2, 1);
            animation.addBox("info", map.getImageSize() / 2, 0, width + 36, map.getImageSize(), 400);
            animation.addSprite("defense", map.getImageSize() / 2 + 4, 4, 400, 2);
        }
        defenseUnit.addDefensiveBonus(200);
        var playerId  = ACTION_SUPPORTSINGLE_IRON_SHIELD.postAnimationUnit.getOwner().getPlayerID();
        defenseUnit.loadIcon("iron_shield", map.getImageSize() / 2, map.getImageSize() / 2, 1, playerId)        
        ACTION_SUPPORTSINGLE_IRON_SHIELD.postAnimationUnit.setHasMoved(true);
        ACTION_SUPPORTSINGLE_IRON_SHIELD.postAnimationUnit = null;
        ACTION_SUPPORTSINGLE_IRON_SHIELD.postAnimationTargetX = -1;
        ACTION_SUPPORTSINGLE_IRON_SHIELD.postAnimationTargetY = -1;
    };
    this.getName = function()
    {
        return qsTr("Iron shield");
    };
    this.getDescription = function()
    {
        return qsTr("Shields an allied unit. This highly increases the defense of the unit until the start of the next turn.");
    };
}

Constructor.prototype = ACTION;
var ACTION_SUPPORTSINGLE_IRON_SHIELD = new Constructor();
