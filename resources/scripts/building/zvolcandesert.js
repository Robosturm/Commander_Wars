var Constructor = function()
{    
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSprite("volcan+desert", false);
    };
};

Constructor.prototype = __ZVOLCANBASE;
var ZVOLCANDESERT = new Constructor();
