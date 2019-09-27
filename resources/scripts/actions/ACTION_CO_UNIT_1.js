var Constructor = function()
{
    // called for loading the main sprite
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
            var co = unit.getOwner().getCO(1);
            if (((constructionList.indexOf(unit.getUnitID()) >= 0) || (building.getBuildingID() === "HQ")) &&
                (unit.getUnitCosts() / 2 <= unit.getOwner().getFunds()) &&
                (co !== null) && (co.getCOUnit() === null) &&
                (unit.getUnitRank() <= GameEnums.UnitRank_Veteran) &&
                (unit.getOwner() === building.getOwner()))
            {
                return true;
            }
        }
        return false;
    };
    this.getActionText = function()
    {
        return qsTr("CO 2");
    };
    this.getIcon = function()
    {
        return "co1";
    };
    this.isFinalStep = function(action)
    {
        return true;
    };
    this.perform = function(action)
    {
        var unit = action.getTargetUnit();
        unit.makeCOUnit(1);
        unit.getOwner().addFunds(-unit.getUnitCosts() / 2);
    };
}

Constructor.prototype = ACTION;
var ACTION_CO_UNIT_1 = new Constructor();
