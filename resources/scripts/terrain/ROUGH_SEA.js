var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Rough Sea"));
    };
	this.loadBaseTerrain = function(terrain)
    {
		terrain.loadBaseTerrain("SEA");
    };
    this.loadBaseSprite = function(terrain)
    {
        terrain.loadBaseSprite("rough_sea+N+E+S+W");
    };
    this.getDefense = function()
    {
        return 2;
    };
    this.canBePlaced = function(x, y)
    {
        var terrain = map.getTerrain(x, y);
        if (terrain.getBaseTerrainID() === "SEA")
        {
            // it's a sea nice
            var surroundings = terrain.getSurroundings("SEA", true, false, GameEnums.Directions_All);
            // we need sea all around us :)

            if (surroundings === "+N+NE+E+SE+S+SW+W+NW")
            {
                return true;
            }
        }
            return false;
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_rough_sea";
    };
};
Constructor.prototype = TERRAIN;
var ROUGH_SEA = new Constructor();
