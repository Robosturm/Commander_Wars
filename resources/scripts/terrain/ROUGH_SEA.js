var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Rough Sea"));
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
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
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_sea";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_sea";
    };
    this.getDescription = function()
    {
        return qsTr("Movement of naval units is impeded but air units are not affected.");
    };
};
Constructor.prototype = TERRAIN;
var ROUGH_SEA = new Constructor();
