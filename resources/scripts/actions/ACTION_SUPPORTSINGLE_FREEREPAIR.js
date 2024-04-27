var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
        if (ACTION.isEmptyFieldAndHasNotMoved(action, unit, actionTargetField, targetField, map) &&
            ACTION_SUPPORTSINGLE_FREEREPAIR.getRepairFields(action, map).length > 0)
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
        return qsTr("Repair");
    };
    this.getIcon = function(map)
    {
        return "repair";
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
        var fields = ACTION_SUPPORTSINGLE_FREEREPAIR.getRepairFields(action, map);
        for (var i3 = 0; i3 < fields.length; i3++)
        {
            data.addPoint(Qt.point(fields[i3].x, fields[i3].y));
        }
    };
    this.getRepairFields = function(action, map)
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
        animation.setEndOfAnimationCall("ACTION_SUPPORTSINGLE_FREEREPAIR", "performPostAnimation");
        // move unit to target position
        Global[unit.getUnitID()].moveUnit(unit, action, map);
        // disable unit commandments for this turn
        action.startReading();
        // read action data
        ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationUnit = unit;
        ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationTargetX = action.readDataInt32();
        ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationTargetY = action.readDataInt32();
    };
    this.performPostAnimation = function(postAnimation, map)
    {
        var refillRule = map.getGameRules().getGameRule("GAMERULE_REFILL_MATERIAL");
        var refillMaterial = (typeof refillRule === 'undefined' || refillRule === null); // an existing rule equals it's set
        var terrain = map.getTerrain(ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationTargetX, ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationTargetY);
        var repairUnit = terrain.getUnit();
        if (!repairUnit.isStealthed(map.getCurrentViewPlayer()))
        {
            var animation = GameAnimationFactory.createAnimation(map, ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationTargetX, ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationTargetY);
            var width = animation.addText(qsTr("REPAIR"), map.getImageSize() / 2 + 25, -2, 1);
            animation.addBox("info", map.getImageSize() / 2, 0, width + 36, map.getImageSize(), 400);
            animation.addSprite("repair", map.getImageSize() / 2 + 4, 4, 400, 2);
        }
        repairUnit.refill(refillMaterial);
        repairUnit.setHp(repairUnit.getHp() + 1);
        ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationUnit.setHasMoved(true);
        ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationUnit = null;
        ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationTargetX = -1;
        ACTION_SUPPORTSINGLE_FREEREPAIR.postAnimationTargetY = -1;
    };
    this.getName = function()
    {
        return qsTr("Free repair");
    };
    this.getDescription = function()
    {
        return qsTr("Repairs and refills the target unit. The repair costs don't have to be paid.");
    };
}

Constructor.prototype = ACTION;
var ACTION_SUPPORTSINGLE_FREEREPAIR = new Constructor();
