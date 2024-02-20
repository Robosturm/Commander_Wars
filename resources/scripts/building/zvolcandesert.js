var Constructor = function()
{    
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSprite("volcan+desert", false);
    };
    this.getName = function()
    {
        return qsTr("Desert volcan");
    };
    this.getVision = function(building)
    {
        return -1;
    };
};

Constructor.prototype = __ZVOLCANBASE;
var ZVOLCANDESERT = new Constructor();
