var Constructor = function()
{
    this.loadSprites = function(building, neutral, map)
    {
        if (building.getOwnerID() >= 0 && !neutral)
        {
            building.loadSprite("blackhole+factory", false);
            building.loadSpriteV2("blackhole+factory+mask", GameEnums.Recoloring_Matrix);
        }
        else
        {
            building.loadSprite("blackhole+factory+neutral", false);
        }
        building.loadSprite("blackhole+factory+shadow+" + BUILDING.getBuildingBaseTerrain(building, map), false);
        building.loadSprite("blackhole+factory+desert_pipe", false);
    };
    this.getName = function()
    {
        return qsTr("Black Hole desert factory");
    };
};

Constructor.prototype = __ZBLACKHOLE_FACTORYBASE;
var ZBLACKHOLE_FACTORYDESERT = new Constructor();
