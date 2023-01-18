var Constructor = function()
{
    
    this.canBePerformed = function(action, map)
    {
        return true;
    };
    this.getActionText = function(map)
    {
        return qsTr("Unknown");
    };
    this.getIcon = function(map)
    {
        return "nickname";
    };
    this.perform = function(action, map)
    {
		 action.startReading();
		var name = action.readDataString()
		var unit = action.getTargetUnit();
        if (unit !== null)
        {
           unit.setCustomName(name);
        }
    };
    this.isFinalStep = function(action, map)
    {
        return true;
    };
    this.getName = function(map)
    {
        return qsTr("Game: Nickname unit");
    };
    this.getDescription = function()
    {
        return qsTr("Allows you to give a unit a special nickname.");
    };
}

Constructor.prototype = ACTION;
var ACTION_NICKNAME_UNIT_INTERNAL = new Constructor();
