var Constructor = function()
{    
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSprite("volcan", false);
    };
    this.getName = function()
    {
        return qsTr("Volcan");
    };
    this.getVision = function(building)
    {
        return -1;
    };
};

Constructor.prototype = __ZVOLCANBASE;
var ZVOLCAN = new Constructor();
