var Constructor = function()
{
    this.getBuildingGroup = function()
    {
        return 1;
    };
    this.init = function (building)
    {
        building.setHp(100);
    };
    
    this.loadSprites = function(building, neutral, map)
    {
        if (building.getOwnerID() >= 0 && !neutral)
        {
            building.loadSpriteV2("forthq", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
            building.loadSpriteV2("forthq+mask", GameEnums.Recoloring_Matrix);
        }
        else
        {
            // neutral player
            building.loadSpriteV2("forthq+neutral", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), TERRAIN.getBuildingTerrainPalette(building));
            building.loadSpriteV2("forthq+mask", GameEnums.Recoloring_Matrix, 400, Qt.point(0,0), "", neutral);
        }
    };
    this.getName = function()
    {
        return qsTr("Fortified HQ");
    };
    this.getDefense = function()
    {
        return 4;
    };
    this.constructionList = ["INFANTRY", "MECH", "MOTORBIKE", "SNIPER", "RECON", "APC", "FLARE",
                             "ARTILLERY", "LIGHT_TANK", "HOVERCRAFT", "FLAK", "ARTILLERYCRAFT", "HOVERFLAK",
                             "ANTITANKCANNON", "HEAVY_TANK", "HEAVY_HOVERCRAFT", "NEOTANK",
                             "MISSILE", "ROCKETTHROWER", "MEGATANK", "PIPERUNNER"];
    this.getConstructionList = function(building)
    {
        return HQ.constructionList;
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_hq";
    };

    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        return "";
    };

    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        var player = terrain.getBuilding().getOwner();
        // get army name
        var armyName = "";
        if (player !== null)
        {
            armyName = Global.getArmyNameFromPlayerTable(player, HQ.armyData);
        }
        var weatherModifier = TERRAIN.getWeatherModifier(map);
        if (weatherModifier === "")
        {
            weatherModifier = TERRAIN.getTerrainWeatherModifier(terrain);
        }
        return "back_" + weatherModifier + "hq+" + armyName;
    };

    this.getDescription = function()
    {
        return qsTr("<r>Fortified HQ harder to capture. </r><div c='#00ff00'>Battle ends </div><r>when an army's last HQ is captured. </r><div c='#00ff00'>Ground </div><r>units can </r><div c='#00ff00'>resupply.</div>");
    };

    this.getVisionHide = function(building)
    {
        return true;
    };

    this.getRepairTypes = function(building)
    {
        return [GameEnums.UnitType_Ground, GameEnums.UnitType_Hovercraft, GameEnums.UnitType_Infantry];
    };
    this.getEditorPlacementSound = function()
    {
        return "placeHq.wav";
    };
}

Constructor.prototype = BUILDING;
var FORTHQ = new Constructor();
