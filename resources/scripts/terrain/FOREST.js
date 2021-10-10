var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setVisionHigh(1);
        terrain.setTerrainName(FOREST.getName());
    };
    this.getName = function()
    {
        return qsTr("Forest");
    };
    this.getDefense = function()
    {
        return 2;
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
		terrain.loadBaseTerrain("PLAINS");
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("FOREST", false, false, GameEnums.Directions_East, false);
        surroundings += terrain.getSurroundings("FOREST", false, false, GameEnums.Directions_West, false);
        terrain.loadBaseSprite("forest" + surroundings);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_forest";
    };
    this.getVisionHide = function()
    {
        return true;
    };

    this.getDescription = function()
    {
        return qsTr("<r>In Fog of War conditions, the woods provide ground unit</r><div c='#00ff00'>hiding places.</div>");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["forest.png",
                "forest+E",
                "forest+E+W",
                "forest+W"];
    };

    this.getTerrainAnimationBase = function(unit, terrain)
    {
        var weatherModifier = TERRAIN.getWeatherModifier();
        return "base_" + weatherModifier + "forest";
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        var rand = globals.randInt(0, 2);
        var weatherModifier = TERRAIN.getWeatherModifier();
        return "back_" + weatherModifier + "forest+" + rand.toString();
    };
};
Constructor.prototype = TERRAIN;
var FOREST = new Constructor();
