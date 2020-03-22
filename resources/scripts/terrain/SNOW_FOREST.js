var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 3;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setVisionHigh(1);
        terrain.setTerrainName(SNOW_FOREST.getName());
    };
    this.getName = function()
    {
        return qsTr("Snowy Forest");
    };
    this.getDefense = function()
    {
        return 2;
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        terrain.loadBaseTerrain("SNOW");
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("SNOW_FOREST", false, false, GameEnums.Directions_East, false);
        surroundings += terrain.getSurroundings("SNOW_FOREST", false, false, GameEnums.Directions_West, false);
        terrain.loadBaseSprite("snow_forest" + surroundings);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_snow_forest";
    };
    this.getVisionHide = function()
    {
        return true;
    };

    this.getTerrainAnimationBase = function(unit, terrain)
    {
        return "base_forest";
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_forest";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_forest";
    };
    this.getDescription = function()
    {
        return "<r>" + qsTr("In Fog of War conditions, the snowy woods provide ground unit ") + "</r>" +
                "<div c='#00ff00'>" + qsTr("hiding places.") + "</div>" +
                "<r>" + qsTr(" It's hard for ground units to cross this terrain.") + "</r>";
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["snow_forest.png",
                "snow_forest+E",
                "snow_forest+E+W",
                "snow_forest+W"];
    };
    this.getTerrainAnimationBase = function(unit, terrain)
    {
        return "base_snowforest";
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_snowforest";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_snowforest";
    };
};
Constructor.prototype = TERRAIN;
var SNOW_FOREST = new Constructor();
