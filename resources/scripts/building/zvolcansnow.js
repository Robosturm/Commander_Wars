var Constructor = function()
{    
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSprite("volcan+snow", false);
    };
};

Constructor.prototype = __ZVOLCANBASE;
var ZVOLCANSNOW = new Constructor();
