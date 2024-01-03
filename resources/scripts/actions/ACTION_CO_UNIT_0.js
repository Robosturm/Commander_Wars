var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        var unit = action.getTargetUnit();
        var actionTargetField = action.getActionTarget();
        var targetField = action.getTarget();
        var building = action.getTargetBuilding();
        if (unit === null || unit.getHasMoved() === true)
        {
            return false;
        }
        if ((actionTargetField.x === targetField.x) && (actionTargetField.y === targetField.y) &&
            (building !== null))
        {
            var constructionList = building.getConstructionList();
            var co = unit.getOwner().getCO(0);
            if (((constructionList.indexOf(unit.getUnitID()) >= 0) ||
                 BUILDING.isHq(building)) &&
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
    this.getActionText = function(map)
    {
        return qsTr("CO 1");
    };
    this.getIcon = function(map)
    {
        return "co0";
    };
    this.isFinalStep = function(action, map)
    {
        return true;
    };
    this.perform = function(action, map)
    {
        var unit = action.getTargetUnit();
        unit.makeCOUnit(0);
        unit.getOwner().addFunds(-unit.getUnitCosts() / 2);
    };
    this.getName = function()
    {
        return qsTr("CO 1");
    };
    this.getDescription = function()
    {
        return qsTr("Deploys your first co onto the given unit. All units inside the CO range of this unit will get the buff of your first CO.");
    };
}

Constructor.prototype = ACTION;
var ACTION_CO_UNIT_0 = new Constructor();
