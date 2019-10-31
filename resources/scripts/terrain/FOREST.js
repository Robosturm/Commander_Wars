var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
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
        return "<r>" + qsTr("In Fog of War conditions, the woods provide ground unit") + "</r>" +
               " <div c='#00ff00'>" + qsTr("hiding places.") + "</div>";
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["forest.png",
                "forest+E",
                "forest+E+W",
                "forest+W"];
    };
};
Constructor.prototype = TERRAIN;
var FOREST = new Constructor();
