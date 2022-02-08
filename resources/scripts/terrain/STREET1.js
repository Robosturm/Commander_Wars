var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    this.baseTerrainId = "PLAINS";
    this.loadBaseSprite = function(terrain, map)
    {
        __BASESTREET.loadBase(terrain, "street+style0", map)
    };
    this.getTerrainSprites = function()
    {
        return __BASESTREET.getSprites("street+style0")
    };

    this.loadOverlaySprite = function(terrain, map)
    {
        var x = terrain.getX();
        var y = terrain.getY();
        var highTerrain = terrain.getSurroundings(TERRAIN.getHighTerrains(), true, false, GameEnums.Directions_West, false);
        if (map.onMap(x - 1, y))
        {
            var building = map.getTerrain(x - 1, y).getBuilding();
            if (building !== null &&
                building.getBuildingWidth() === 1 &&
                building.getBuildingHeigth() === 1)
            {
                highTerrain = "+W";
            }
        }
        if (highTerrain !== "")
        {
            var surroundings = __BASESTREET.getStreetSurrounding(terrain);
            terrain.loadOverlaySprite("street+style0" + surroundings + "+shadow");
        }
    };
};
Constructor.prototype = __BASESTREET;
var STREET1 = new Constructor();
