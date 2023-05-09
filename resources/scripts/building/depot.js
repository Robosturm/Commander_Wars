var Constructor = function()
{
    this.getBuildingGroup = function()
    {
        return 2;
    };
    this.constructionList = ["INFANTRY", "MECH", "MOTORBIKE", "SNIPER", "RECON", "APC", "FLARE",
                            "ARTILLERY", "LIGHT_TANK", "HOVERCRAFT", "FLAK", "ARTILLERYCRAFT", "HOVERFLAK",
                            "ANTITANKCANNON", "HEAVY_TANK", "HEAVY_HOVERCRAFT", "NEOTANK",
                            "MISSILE", "ROCKETTHROWER", "MEGATANK", "PIPERUNNER"];
    this.getConstructionList = function(building)
    {
        return DEPOT.constructionList;
    };
    this.getRepairTypes = function(building)
    {
        return [GameEnums.UnitType_Ground, GameEnums.UnitType_Hovercraft, GameEnums.UnitType_Infantry];
    };
    this.startOfTurn = function(building, map)
    {
        BUILDING.replenishUnit(building, map, 0, 0.5, 0.5, 0.5, true);
    };
    this.loadSprites = function(building, neutral, map)
    {
        building.loadSpriteV2("depot+mask", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), "", neutral);
        building.loadSpriteV2("depot", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
    };
    this.getBaseIncome = function()
    {
        return 0;
    };
    this.getDefense = function()
    {
        return 2;
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_depot";
    };

    this.getName = function()
    {
        return qsTr("Depot");
    };

    this.getDescription = function()
    {
        return qsTr("<r>A civilian cargo depot that provides adequate cover and </r><div c='#00ff00'>ground </div><r>units can </r><div c='#00ff00'>resupply</div><r> half their supplies each turn.</r>.");
    };

    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        return "";
    };

    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        var weatherModifier = TERRAIN.getWeatherModifier(map);
        if (weatherModifier === "")
        {
            weatherModifier = TERRAIN.getTerrainWeatherModifier(terrain);
        }
        return "back_" + weatherModifier + "depot";
    };
}

Constructor.prototype = BUILDING;
var DEPOT = new Constructor();
