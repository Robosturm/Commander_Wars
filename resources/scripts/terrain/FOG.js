var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 0;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(FOG.getName());
    };
    this.getName = function()
    {
        return qsTr("Fog");
    };
    this.getDefense = function()
    {
        return 1;
    };
    this.getVisionHide = function()
    {
        return true;
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
		terrain.loadBaseTerrain("SEA");
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("FOG", false, false, GameEnums.Directions_East, false);
        surroundings += terrain.getSurroundings("FOG", false, false, GameEnums.Directions_West, false);
		if (surroundings === "")
		{
			var random = globals.randInt(0, 2);
			terrain.loadBaseSprite("fog+" + random.toString());
		}
		else
		{
			terrain.loadBaseSprite("fog" + surroundings);
		}        
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_fog";
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
        return qsTr("This heavy fog provides ideal hiding places<div c='00ff00'> for naval units in Fog of War.</div>");
    };
};
Constructor.prototype = TERRAIN;
var FOG = new Constructor();
