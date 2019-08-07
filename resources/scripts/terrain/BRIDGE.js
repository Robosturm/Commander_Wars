var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(BRIDGE.getName());
    };

    this.getName = function()
    {
        return qsTr("Bridge");
    };
	this.loadBaseTerrain = function(terrain)
    {
		terrain.loadBaseTerrain("RIVER");
    };
    this.loadBaseSprite = function(terrain, currentTerrainID)
    {
        var surroundings = terrain.getSurroundings("RIVER,SEA,ROUGH_SEA,REAF,BEACH", false, false, GameEnums.Directions_Direct, false);
        terrain.loadBaseSprite("bridge" + surroundings);
    };
    this.useTerrainAsBaseTerrain = function()
    {
        return true;
    };
    this.canBePlaced = function(x, y)
    {
        var terrain = map.getTerrain(x, y);
        if ((terrain.getTerrainID() === "SEA") ||
            (terrain.getTerrainID() === "RIVER") ||
            (terrain.getTerrainID() === "BRIDGE"))
        {
            return true;
        }
        else
        {
            return false;
        }

    };
    this.getMiniMapIcon = function()
    {
        return "minimap_bridge";
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_bridge";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_bridge";
    };
    this.getDescription = function()
    {
        return qsTr("Bridge over river and seas which allows ground units to cross. Bridges over sea also allows naval units to cross the field.");
    };
};
Constructor.prototype = TERRAIN;
var BRIDGE = new Constructor();
