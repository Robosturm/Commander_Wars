var Constructor = function()
{    
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSprite("volcan+snow", false);
    };
    this.getName = function()
    {
        return qsTr("Snowy volcano");
    };
    this.getVision = function(building)
    {
        return -1;
    };
};

Constructor.prototype = __ZVOLCANBASE;
var ZVOLCANSNOW = new Constructor();
