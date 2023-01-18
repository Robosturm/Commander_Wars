var Constructor = function()
{    
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSprite("volcan+waste", false);
    };
    this.getName = function()
    {
        return qsTr("Waste volcan");
    };
};

Constructor.prototype = __ZVOLCANBASE;
var ZVOLCANWASTE = new Constructor();
