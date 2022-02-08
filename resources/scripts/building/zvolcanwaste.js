var Constructor = function()
{    
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSprite("volcan+waste", false);
    };
};

Constructor.prototype = __ZVOLCANBASE;
var ZVOLCANWASTE = new Constructor();
