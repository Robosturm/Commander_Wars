ACTION_SUPPORTSINGLE_IRON_SHIELD.getDefenseFields = function(action)
{
    var targetField = action.getActionTarget();
    var targetFields = [Qt.point(targetField.x + 1, targetField.y),
                        Qt.point(targetField.x - 1, targetField.y),
                        Qt.point(targetField.x,     targetField.y - 1),
                        Qt.point(targetField.x,     targetField.y + 1)];
    var unit = action.getTargetUnit();
    var ret = [];
    for (var i = 0; i < targetFields.length; i++)
    {
        if (map.onMap(targetFields[i].x, targetFields[i].y))
        {
            var terrain = map.getTerrain(targetFields[i].x, targetFields[i].y);
            var repairUnit = terrain.getUnit();
            if (repairUnit !== null &&
                    repairUnit.getOwner().isAlly(unit.getOwner()) &&
                    repairUnit !== unit)
            {
                ret.push(targetFields[i]);
            }
        }
    }
    return ret;
};

ACTION_SUPPORTSINGLE_IRON_SHIELD.performPostAnimation = function(postAnimation)
{
    var terrain = map.getTerrain(ACTION_SUPPORTSINGLE_IRON_SHIELD.postAnimationTargetX, ACTION_SUPPORTSINGLE_IRON_SHIELD.postAnimationTargetY);
    var defenseUnit = terrain.getUnit();
    var animation = GameAnimationFactory.createAnimation(ACTION_SUPPORTSINGLE_IRON_SHIELD.postAnimationTargetX, ACTION_SUPPORTSINGLE_IRON_SHIELD.postAnimationTargetY);
    var width = animation.addText(qsTr("DEFENSE"), map.getImageSize() / 2 + 25, 2, 1);
    animation.addBox("info", map.getImageSize() / 2, 0, width + 32, map.getImageSize(), 400);
    animation.addSprite("defense", map.getImageSize() / 2 + 8, 1, 400, 1.7);
    var shieldOwner = ACTION_SUPPORTSINGLE_IRON_SHIELD.postAnimationUnit.getOwner();
    var targetOwner = defenseUnit.getOwner();
    if (targetOwner === ACTION_SUPPORTSINGLE_IRON_SHIELD.postAnimationUnit.getOwner())
    {
        defenseUnit.addDefensiveBonus(200, 1);
    }
    else if (targetOwner.isAlly(ACTION_SUPPORTSINGLE_IRON_SHIELD.postAnimationUnit.getOwner()))
    {
        defenseUnit.addDefensiveBonus(200, 2);
        // store unit id for each unit that got a defence buff this may stack. So an id may be multiple times in the same array and get the same debuf more than once
        var variables = shieldOwner.getVariables();
        var boostVariable = variables.createVariable("COOP_MOD_DEFENSEBOOST_UNITS");
        var boosts = boostVariable.readDataListInt32();
        var unitId = defenseUnit.getUniqueID();
        boosts.push(unitId);
        boostVariable.writeDataListInt32(boosts);
    }
    var playerId  = ACTION_SUPPORTSINGLE_IRON_SHIELD.postAnimationUnit.getOwner().getPlayerID();
    defenseUnit.loadIcon("iron_shield", map.getImageSize() / 2, map.getImageSize() / 2, 1, playerId)
    ACTION_SUPPORTSINGLE_IRON_SHIELD.postAnimationUnit.setHasMoved(true);
    ACTION_SUPPORTSINGLE_IRON_SHIELD.postAnimationUnit = null;
    ACTION_SUPPORTSINGLE_IRON_SHIELD.postAnimationTargetX = -1;
    ACTION_SUPPORTSINGLE_IRON_SHIELD.postAnimationTargetY = -1;
};
