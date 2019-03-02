var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
        var building = action.getTargetBuilding();
        if ((unit.getHasMoved() === true))
        {
            return false;
        }
        if ((actionTargetField.x === targetField.x) && (actionTargetField.y === targetField.y))
        {
            var constructionList = Global[building.getBuildingID()].getConstructionList();
            var co = unit.getOwner().getCO(1);
            if (((constructionList.indexOf(unit.getUnitID()) >= 0) || (building.getBuildingID() === "HQ")) &&
                (unit.getUnitCosts() / 2 <= unit.getOwner().getFonds()) &&
                (co !== null) && (co.getCOUnit() === null) &&
                (unit.getUnitRank() <= GameEnums.UnitRank_Veteran))
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
        return "co_unit";
    };
    this.isFinalStep = function(action)
    {
        return true;
    };
    this.perform = function(action)
    {
        var unit = action.getTargetUnit();
        unit.makeCOUnit(1);
        unit.getOwner().addFonds(-unit.getUnitCosts() / 2);
    };
}

Constructor.prototype = ACTION;
var ACTION_CO_UNIT_1 = new Constructor();
