var Constructor = function()
{

    terrain.setHasFlowDirection(true);

    this.loadBaseSprite = function(terrain, currentTerrainID, map)
    {
        __BASEBRIDGE.loadSprite(terrain, "bridge+style1", map)
    };
    this.getTerrainSprites = function()
    {
        return __BASEBRIDGE.getSprites("bridge+style1")
    };
    this.getTerrainSprites = function()
    {
        return __BASEBRIDGE.getSprites("bridge+style1")
    };
    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        return __BASEBRIDGE.getTerrainAnimationBackgroundRand(unit, terrain, defender, map, 0);
    };

    this.canBePlaced = function(x, y, map)
    {
        return __BASEBRIDGE.canBePlacedBaseId(x, y, map, "BRIDGE");
    };
    this.getName = function()
    {
        return qsTr("Ship bridge");
    };
    this.getDescription = function()
    {
        return qsTr("Bridge over river and seas which allows ground units to cross. Bridges over sea also allows naval units to cross the field.");
    };
};
Constructor.prototype = __BASEBRIDGE;
var BRIDGE = new Constructor();
