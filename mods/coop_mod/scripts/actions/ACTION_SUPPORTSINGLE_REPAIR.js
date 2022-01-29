ACTION_SUPPORTSINGLE_REPAIR.getRepairFields = function(action, map)
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
