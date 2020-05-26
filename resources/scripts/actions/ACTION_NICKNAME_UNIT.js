var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
		var units = map.getCurrentPlayer().getUnits();
		var ret = (units.size() > 0);
		units.remove();
        return ret;
    };
    
    this.getActionText = function()
    {
        return qsTr("Nickname Unit");
    };
    this.getIcon = function()
    {
        return "help";
    };
    this.isFinalStep = function(action)
    {
		action.setIsLocal(true);
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
            map.nicknameUnit(x, y);
        }
    };
    this.getStepData = function(action, data)
    {
        var units = map.getCurrentPlayer().getUnits();
        for (var i = 0; i < units.size(); i++)
        {
            var unit = units.at(i);
            data.addPoint(Qt.point(unit.getX(), unit.getY()));
        }
        data.setShowZData(false);
        data.setColor("#C8FF0000");
        units.remove();
    };
    this.getStepInputType = function(action)
    {
        return "FIELD";
    };
}

Constructor.prototype = ACTION;
var ACTION_NICKNAME_UNIT = new Constructor();
