var Constructor = function()
{
    this.canBePerformed = function(action)
    {		
        var ret = false;
        var resellValue = map.getGameRules().getResellValue();
        if (resellValue > 0)
        {
            var units = map.getCurrentPlayer().getUnits();
            for (var i = 0; i < units.size(); i++)
            {
                var unit = units.at(i);
                if (unit.getHasMoved() === false)
                {
                    ret = true;
                    break;
                }
            }
            units.remove();
        }
        return ret;
    };
    
    this.getActionText = function()
    {
        return qsTr("Sell Unit");
    };
    this.getIcon = function()
    {
        return "destroy";
    };
    this.isFinalStep = function(action)
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
    this.perform = function(action)
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
            map.getCurrentPlayer().addFunds(funds * resellValue);
            unit.killUnit();
        }
    };
    this.getStepCursor = function(action, cursorData)
    {
        cursorData.setCursor("cursor+delete");
        cursorData.setXOffset(0);
        cursorData.setYOffset(0);
        cursorData.setScale(1.0);
    };
    this.getStepData = function(action, data)
    {
        var units = map.getCurrentPlayer().getUnits();
        for (var i = 0; i < units.size(); i++)
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
        units.remove();
    };
    this.getStepInputType = function(action)
    {
        return "FIELD";
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
