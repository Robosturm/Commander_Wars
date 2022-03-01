var Constructor = function()
{    
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSprite("volcan", false);
    };
};

Constructor.prototype = __ZVOLCANBASE;
var ZVOLCAN = new Constructor();
