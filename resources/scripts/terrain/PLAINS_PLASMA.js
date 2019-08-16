var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(PLAINS_PLASMA.getName());
    };
    this.getName = function()
    {
        return qsTr("Plains");
    };
	this.getDefense = function()
    {
        return 1;
    };
    this.loadBaseSprite = function(terrain, currentTerrainID)
    {
        var surroundings = terrain.getSurroundings("PLAINS_PLASMA", false, false, GameEnums.Directions_Direct);
        terrain.loadBaseSprite("plains+plasma" + surroundings);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_plains";
    };
    this.getDescription = function()
    {
        return qsTr("Ground units move easily on this terrain.");
    };
};
Constructor.prototype = TERRAIN;
var PLAINS_PLASMA = new Constructor();
