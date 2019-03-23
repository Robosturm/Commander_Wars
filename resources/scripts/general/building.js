// this is the base class for terrain
var BUILDING =
{

    // loader for stuff which needs C++ Support
    init : function (building)
    {
    },
    // returns the defense of this terrain
    getDefense : function()
    {
        return 3;
    },

    // vision bonus for units created by this building
    getVisionBonus : function()
    {
        return 0;
    },

    // called for loading the main sprites
    loadSprites : function(building)
    {
    },
    // the terrain on which a building can be placed
    // if the current terrain isn't in the list. it'll be replaced by the first :)
    getBaseTerrain : function(building)
    {
        return "PLAINS,STREET"
    },

    addCaptureAnimationBuilding : function(animation, building, startColor, capturedColor)
    {
        animation.addBuildingSprite("town+mask", startColor , capturedColor, true);
        animation.addBuildingSprite("town", startColor , capturedColor, false);
    },

    canBuildingBePlaced : function(terrainID)
    {
        if ((terrainID === "PLAINS") || (terrainID === "STREET"))
        {
            return true;
        }
        else
        {
            return false;
        }
    },

    getBuildList : function()
    {
        return "";
    },

    getBaseIncome : function()
    {
        return 1000;
    },

    getConstructionList : function()
    {
        return [];
    },

    getActions : function()
    {
        return "";
    },

    startOfTurn : function(building)
    {
        BUILDING.replenishUnit(building);
    },

    getOffensiveBonus : function(building)
    {
        return 0;
    },

    getDefensiveBonus : function(building)
    {
        return 0;
    },

    replenishUnit: function(building)
    {
        // default impl replenishes our units
        // gets called at the start of a turn
        var constructionList = Global[building.getBuildingID()].getConstructionList();
        var unit = building.getTerrain().getUnit();
        if ((unit !== null) &&
            (unit.getOwner() === building.getOwner()) &&
            (constructionList.indexOf(unit.getUnitID()) > 0))
        {
            // our unit and a repairable one
            // replenish it
            unit.refill();
            var repairAmount = 2 + unit.getRepairBonus(Qt.point(unit.getX(), unit.getY()));
            UNIT.repairUnit(unit, repairAmount);

        }
    },

    getMiniMapIcon : function()
    {
        return "minimap_building";
    },
};
