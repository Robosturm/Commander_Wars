var Constructor = function()
{
    // called for loading the main sprite
    this.canBePerformed = function(action)
    {
        return true;
    };
    this.getActionText = function()
    {
        return "unknown";
    };
    this.getIcon = function()
    {
        return "";
    };
    this.perform = function(action)
    {
		 action.startReading();
		var name = action.readDataString()
		var unit = action.getTargetUnit();
        if (unit !== null)
        {
           unit.setCustomName(name);
        }
    };
    this.isFinalStep = function(action)
    {
        return true;
    };
}

Constructor.prototype = ACTION;
var ACTION_NICKNAME_UNIT_INTERNAL = new Constructor();
