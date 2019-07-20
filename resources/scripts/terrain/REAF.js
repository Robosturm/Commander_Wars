var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Reaf"));
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
		terrain.loadBaseTerrain("SEA");
    };
    this.loadBaseSprite = function(terrain)
    {
		var random = globals.randInt(0, 3);
        if (random > 0)
        {
            terrain.loadBaseSprite("reaf+" + random.toString() + "+N+E+S+W");
        }
        else
        {
            terrain.loadBaseSprite("reaf+N+E+S+W");
        }
    };
    this.getDefense = function()
    {
        return 2;
    };
    this.getVisionHide = function()
    {
        return true;
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
        return "minimap_reaf";
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_sea";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_sea";
    };
};
Constructor.prototype = TERRAIN;
var REAF = new Constructor();
