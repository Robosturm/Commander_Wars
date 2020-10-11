var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        for (var i = 0; i < map.getPlayerCount(); i++)
        {
            var player = map.getPlayer(i);
            if (player !== currentPlayer &&
                currentPlayer.isAlly(player))
            {
                return true;
            }
        }
        return false;
    };
    
    this.getActionText = function()
    {
        return qsTr("Transfer Unit");
    };
    this.getIcon = function()
    {
        return "next_player";
    };
    this.isFinalStep = function(action)
    {
        if (action.getInputStep() === 2)
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
		var player = action.getCosts();
        var unit = map.getTerrain(x, y).getUnit();
        unit.setOwner(map.getPlayer(player));
    };
    this.getStepCursor = function(action, cursorData)
    {
		cursorData.setCursor("cursor+edit");
		cursorData.setXOffset(0);
		cursorData.setYOffset(0);
		cursorData.setScale(1.0);
    };
    this.getStepData = function(action, data)
    {
		if (action.getInputStep() === 1)
        {
			var currentPlayer = map.getCurrentPlayer();
            for (var i = 0; i < map.getPlayerCount(); i++)
            {
				var player = map.getPlayer(i);
				if (player !== currentPlayer &&
					currentPlayer.isAlly(player))
				{
					data.addData(qsTr("Player ") + (i + 1).toString(), "PLAYER" + (i + 1).toString(), "capture", i, true);
				}
			}
		}
		else
		{
			var units = map.getCurrentPlayer().getUnits();
			for (var i = 0; i < units.size(); i++)
			{
				var unit = units.at(i);
				if (unit.getHasMoved() === false &&
					unit.getUnitRank() >= GameEnums.UnitRank_None)
				{
					var x = unit.getX();
					var y = unit.getY();				
					data.addPoint(Qt.point(x, y));             
				}
			}
			data.setShowZData(false);
			data.setColor("#FF00FF00");
			units.remove();
		}
    };
    this.getStepInputType = function(action)
    {
		if (action.getInputStep() === 1)
        {
			return "MENU";
		}
		else
		{
			return "FIELD";			
		}
    };
	this.getDescription = function()
	{
		return qsTr("Transfers a unit to another allied player.");
    };	
};

Constructor.prototype = ACTION;
var ACTION_TRANSFER_UNIT = new Constructor();
