var Constructor = function()
{
    this.canBePerformed = function(action)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
        var building = action.getTargetBuilding();
        if ((unit.getHasMoved() === true) ||
            (unit.getBaseMovementCosts(actionTargetField.x, actionTargetField.y) <= 0))
        {
            return false;
        }
        if ((actionTargetField.x === targetField.x) && (actionTargetField.y === targetField.y) &&
            (building !== null))
        {
            var constructionList = building.getConstructionList();
            var co = unit.getOwner().getCO(0);
            if (((constructionList.indexOf(unit.getUnitID()) >= 0) || (building.getBuildingID() === "HQ")) &&
                (unit.getUnitCosts() / 2 <= unit.getOwner().getFunds()) &&
                (co !== null) && (co.getCOUnit() === null) &&
                (unit.getUnitRank() >= GameEnums.UnitRank_None) &&
                (unit.getOwner() === building.getOwner()))
            {
                return true;
            }
        }
        return false;
    };
    this.getActionText = function()
    {
        return qsTr("CO 1");
    };
    this.getIcon = function()
    {
        return "co0";
    };
    this.isFinalStep = function(action)
    {
        return true;
    };
    this.perform = function(action)
    {
        var unit = action.getTargetUnit();
        unit.makeCOUnit(0);
        unit.getOwner().addFunds(-unit.getUnitCosts() / 2);
    };
    this.getDescription = function()
    {
        return qsTr("Deploys your first co onto the given unit. All units inside the co range of this unit will get the buff of your first co.");
    };
}

Constructor.prototype = ACTION;
var ACTION_CO_UNIT_0 = new Constructor();
