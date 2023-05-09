var Constructor = function()
{
    this.getBuildingGroup = function()
    {
        return 3;
    };
    this.init = function (building)
    {
        building.setHp(-1);
        building.setAlwaysVisble(true);
    };
    
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSpriteV2("fortress+mask", GameEnums.Recoloring_Matrix);
        if (building.getOwnerID() >= 0 && !neutral)
        {
            building.loadSpriteV2("fortress", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        }
        else
        {
            building.loadSpriteV2("fortress+neutral", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        }
    };
    this.getDefense = function(building)
    {
        return 0;
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getName = function()
    {
        return qsTr("Fortress");
    };
    this.getBuildingWidth = function()
    {
        // one field width default for most buildings
        return 4;
    };
    this.getBuildingHeigth = function()
    {
        // one field heigth default for most buildings
        return 4;
    };
    this.canBuildingBePlaced = function(terrain, building, map)
    {
        return BUILDING.canLargeBuildingPlaced(terrain, building, ZFORTRESS.getBuildingWidth(), ZFORTRESS.getBuildingHeigth(), map);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_blackholebuilding";
    };
    this.getDescription = function()
    {
        return qsTr("Black Hole Fortress that defends a position has no other effects.");
    };
}

Constructor.prototype = BUILDING;
var ZFORTRESS = new Constructor();
