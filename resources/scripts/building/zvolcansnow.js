var Constructor = function()
{    
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSprite("volcan+snow", false);
    };
    this.getName = function()
    {
        return qsTr("Snowy volcan");
    };
};

Constructor.prototype = __ZVOLCANBASE;
var ZVOLCANSNOW = new Constructor();
