var Constructor = function()
{
    this.canBePerformed = function(action, map)
    {
        var units = map.getCurrentPlayer().getUnits();
        var ret = false;
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
        return ret;
    };
    
    this.getActionText = function(map)
    {
        return qsTr("Delete Unit");
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
                data.addPoint(Qt.point(unit.getX(), unit.getY()));
            }
        }
        data.setShowZData(false);
        data.setColor("#C8FF0000");
    };
    this.getStepInputType = function(action, map)
    {
        return "FIELD";
    };
}

Constructor.prototype = ACTION;
var ACTION_DELETE_UNIT = new Constructor();
