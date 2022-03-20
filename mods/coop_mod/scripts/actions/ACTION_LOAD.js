ACTION_LOAD.canBePerformed = function(action, map)
{
    var unit = action.getTargetUnit();
    var actionTargetField = action.getActionTarget();
    var targetField = action.getTarget();
    var targetUnit = action.getMovementTarget();
    var transportTerrain = action.getMovementTerrain();

    if ((unit.getHasMoved() === true) ||
        (unit.getBaseMovementCosts(actionTargetField.x, actionTargetField.y) <= 0))
    {
        return false;
    }
    if (((actionTargetField.x !== targetField.x) || (actionTargetField.y !== targetField.y)) &&
            (targetUnit !== null))
    {
        if ((targetUnit.getOwner().isAlly(unit.getOwner())) &&
            (targetUnit.getTransportUnits().indexOf(unit.getUnitID()) >= 0) &&
            (targetUnit.getLoadedUnitCount() < targetUnit.getLoadingPlace())) {

            var unitID = targetUnit.getUnitID();
            var terrainId = transportTerrain.getID();
            if (unitID === "TRANSPORTPLANE")
            {
                if ((terrainId === "AIRPORT") ||
                    (terrainId === "TEMPORARY_AIRPORT") ||
                    (terrainId === "SNOW_STREET") ||
                    (terrainId === "STREET") ||
                    (terrainId === "STREET1"))
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else if (unitID === "LANDER" ||
                     unitID === "BLACK_BOAT" ||
                     unitID === "CANNONBOAT")
            {
                if ((terrainId === "BRIDGE") ||
                    (terrainId === "BRIDGE1") ||
                    (terrainId === "BRIDGE2"))
                {
                    return false;
                }
            }
            return true;
        }
    }
    return false;

};
