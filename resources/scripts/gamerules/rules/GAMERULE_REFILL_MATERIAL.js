var Constructor = function()
{
    this.getRuleDescription = function(itemNumber)
    {
        return qsTr("Allows material to be refilled if enabled.");
    };
    this.getRuleName = function(itemNumber)
    {
        return qsTr("Material refill");
    };
};

Constructor.prototype = GAMERULE;
var GAMERULE_REFILL_MATERIAL = new Constructor();
