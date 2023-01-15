var Constructor = function()
{
    this.getRuleDescription = function(itemNumber)
    {
        return qsTr("If enabled material can be refilled.");
    };
    this.getRuleName = function(itemNumber)
    {
        return qsTr("Material refill");
    };
};

Constructor.prototype = GAMERULE;
var GAMERULE_REFILL_MATERIAL = new Constructor();
