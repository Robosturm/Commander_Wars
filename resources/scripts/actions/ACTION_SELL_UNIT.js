var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {		
        var ret = false;
        var resellValue = map.getGameRules().getResellValue();
        if (resellValue > 0)
        {
            var units = map.getCurrentPlayer().getUnits();
            var size = units.size();
            for (var i = 0; i < size; i++)
            {
                var unit = units.at(i);
                if (unit.getHasMoved() === false)
                {
                    ret = true;
                    break;
                }
            }
        }
        return ret;
    };
    
    this.getActionText = function(map)
    {
        return qsTr("Sell unit");
    };
    this.getIcon = function(map)
    {
        return "destroy";
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
    this.perform = function(action, map)
    {
        action.startReading();
        // read action data
        var x = action.readDataInt32();
        var y = action.readDataInt32();
        var unit = map.getTerrain(x, y).getUnit();
        if (unit !== null)
        {
            var resellValue = map.getGameRules().getResellValue();
			var funds = unit.getUnitCosts() * unit.getHp() / 10;
            unit.getOwner().addFunds(funds * resellValue);
            unit.killUnit();
        }
    };
    this.getStepCursor = function(action, cursorData, map)
    {
        cursorData.setCursor("cursor+delete");
        cursorData.setXOffset(0);
        cursorData.setYOffset(0);
        cursorData.setScale(1.0);
    };
    this.getStepData = function(action, data, map)
    {
        var units = map.getCurrentPlayer().getUnits();
        var size = units.size();
        for (var i = 0; i < size; i++)
        {
            var unit = units.at(i);
            if (unit.getHasMoved() === false)
            {
				var x = unit.getX();
				var y = unit.getY();
				var building = map.getTerrain(x, y).getBuilding();
				if (building !== null &&
					building.getOwner() === unit.getOwner())
				{
					var constructionList = building.getConstructionList();
					if (constructionList.indexOf(unit.getUnitID()) >= 0)
					{
						data.addPoint(Qt.point(x, y));
					}
				}                
            }
        }
        data.setShowZData(false);
        data.setColor("#C8FF0000");
    };
    this.getStepInputType = function(action, map)
    {
        return "FIELD";
    };
    this.getName = function(map)
    {
        return qsTr("Sell unit");
    };
    this.getDescription = function()
	{
        return qsTr("Sells a unit for a fraction of it's current value.");
    };
    this.getBannedByDefault = function()
    {
        return true;
    };
};

Constructor.prototype = ACTION;
var ACTION_SELL_UNIT = new Constructor();
