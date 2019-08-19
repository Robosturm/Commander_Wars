var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        return true;
    };
    
    this.getActionText = function()
    {
        return qsTr("Delete Unit");
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
                data.addPoint(Qt.point(unit.getX(), unit.getY()));
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
}

Constructor.prototype = ACTION;
var ACTION_DELETE_UNIT = new Constructor();
