var Constructor = function()
{
    this.init = function (building)
    {
    };
    
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSprite("destroyed_gate+N+S", false);
        building.loadSpriteV2("destroyed_gate+N+S+mask", GameEnums.Recoloring_Matrix);
        building.loadSprite("gate+N+S+shadow+" + BUILDING.getBuildingBaseTerrain(building, map), false);
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
        return qsTr("A destroyed gate through a wall can be crossed by players.");
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
var ZDESTROYED_GATE_N_S = new Constructor();
