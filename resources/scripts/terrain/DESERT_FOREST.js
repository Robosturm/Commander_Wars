var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 2;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(DESERT_FOREST.getName());
    };
    this.getName = function()
    {
        return qsTr("Desert Forest");
    };
    this.getDefense = function()
    {
        return 2;
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
		terrain.loadBaseTerrain("DESERT");
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("DESERT_FOREST", false, false, GameEnums.Directions_East, false);
        surroundings += terrain.getSurroundings("DESERT_FOREST", false, false, GameEnums.Directions_West, false);
        terrain.loadBaseSprite("desert_forest" + surroundings);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_desert_forest";
    };
    this.getVisionHide = function()
    {
        return true;
    };
    this.getFirerangeModifier = function(terrain, unit)
    {
        return -1;
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
        return qsTr("In Fog of War conditions, the woods provide ground unit hiding places.<div c='00ff00'> It reduces the firerange of indirect units by 1.</div>");
    };
};
Constructor.prototype = TERRAIN;
var DESERT_FOREST = new Constructor();
