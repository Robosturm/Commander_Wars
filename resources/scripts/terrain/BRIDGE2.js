var Constructor = function()
{
    this.getDefaultPalette = function()
    {
        return "palette_desert";
    };
    this.loadBaseSprite = function(terrain, currentTerrainID, map)
    {
        __BASEBRIDGE.loadSprite(terrain, "bridge_river", map)
    };
    this.getTerrainSprites = function()
    {
        return __BASEBRIDGE.getSprites("bridge_river")
    };
    this.getTerrainSprites = function()
    {
        return __BASEBRIDGE.getSprites("bridge_river")
    };
    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        return __BASEBRIDGE.getTerrainAnimationBackgroundRand(unit, terrain, defender, map, 1);
    };
    this.canBePlaced = function(x, y, map)
    {
        return __BASEBRIDGE.canBePlacedBaseId(x, y, map, "BRIDGE2");
    };
    this.getName = function()
    {
        return qsTr("Old bridge");
    };
    this.getDescription = function()
    {
        return qsTr("Bridge over river and seas which allows ground units to cross.");
    };
    this.getShowInWiki = function()
    {
        return false;
    };
};
Constructor.prototype = __BASEBRIDGE;
var BRIDGE2 = new Constructor();
