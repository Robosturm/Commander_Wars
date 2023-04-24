var Constructor = function()
{
    this.getBuildingGroup = function()
    {
        return 6;
    };
    this.init = function (building)
    {
    };
    
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSpriteV2("destroyed_gate+E+W", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        building.loadSpriteV2("destroyed_gate+E+W+mask", GameEnums.Recoloring_Matrix);
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getName = function()
    {
        return qsTr("Destroyed gate");
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_blackholebuilding";
    };
    this.getDescription = function()
    {
        return qsTr("A destroyed gate through which all players can pass.");
    };
    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        return "fore_gate+0";
    };
    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        return Global[terrain.getTerrainID()].getTerrainAnimationBackground(unit, terrain, defender, map);
    };
}

Constructor.prototype = BUILDING;
var ZDESTROYED_GATE_E_W = new Constructor();
