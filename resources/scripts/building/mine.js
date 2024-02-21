var Constructor = function()
{
    this.getBuildingGroup = function()
    {
        return 1;
    };
    
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSpriteV2("mine+mask", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), "", neutral);
        if (building.getOwnerID() >= 0 && !neutral)
        {
            building.loadSpriteV2("mine", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        }
        else
        {
            building.loadSpriteV2("mine+neutral", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
        }
    };
    this.getBaseIncome = function()
    {
        return 3000;
    };
    this.getName = function()
    {
        return qsTr("Mine");
    };

    this.getDescription = function()
    {
        return qsTr("Ore mine. Once captured produces a lot of funds, but can't repair units.");
    };

    this.getVisionHide = function(building)
    {
        return true;
    };
    this.getVision = function(building, map)
    {
        return map.getGameRules().getBuildingVision();
    };
}

Constructor.prototype = BUILDING;
var MINE = new Constructor();
