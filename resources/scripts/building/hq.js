var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(building, neutral)
    {
        if (building.getOwnerID() >= 0 && !neutral)
        {
            // get army name
            var armyName = building.getOwner().getArmy().toLowerCase();
            // bh and bg have the same sprites
            if (armyName === "bg")
            {
                armyName = "bh"
            }
            building.loadSprite("hq+" + armyName, false);
            building.loadSpriteV2("hq+" + armyName + "+mask", GameEnums.Recoloring_Table);
        }
        else
        {
            // neutral player
            building.loadSprite("hq+neutral", false);
        }
    };
    this.getName = function()
    {
        return qsTr("HQ");
    };
    this.addCaptureAnimationBuilding = function(animation, building, startPlayer, capturedPlayer)
    {
        var player = building.getOwner();
        // get army name
        var armyName = "os";
        if (player !== null)
        {
            armyName = player.getArmy().toLowerCase();
        }
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        if ((armyName !== "os") &&
            (armyName !== "yc") &&
            (armyName !== "ge") &&
            (armyName !== "bm") &&
            (armyName !== "bh"))
        {
            armyName = "os";
        }
        animation.addBuildingSprite("hq+" + armyName + "+mask", startPlayer , capturedPlayer, GameEnums.Recoloring_Table);
        animation.addBuildingSprite("hq+" + armyName, startPlayer , capturedPlayer, GameEnums.Recoloring_None);
    };
    this.getDefense = function()
    {
        return 4;
    };
    this.getConstructionList = function(building)
    {
        return ["INFANTRY", "MECH", "MOTORBIKE", "SNIPER", "RECON", "APC", "FLARE",
                "ARTILLERY", "LIGHT_TANK", "HOVERCRAFT", "FLAK", "ARTILLERYCRAFT", "HOVERFLAK",
                "ANTITANKCANNON", "HEAVY_TANK", "HEAVY_HOVERCRAFT", "NEOTANK",
                "MISSILE", "ROCKETTHROWER", "MEGATANK", "PIPERUNNER"];
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_hq";
    };

    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        var player = terrain.getBuilding().getOwner();
        // get army name
        var armyName = "os";
        if (player !== null)
        {
            armyName = player.getArmy().toLowerCase();
        }
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        if ((armyName !== "os") &&
            (armyName !== "yc") &&
            (armyName !== "ge") &&
            (armyName !== "bm") &&
            (armyName !== "bh"))
        {
            armyName = "os";
        }
        return "fore_hq+" + armyName;
    };

    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        var player = terrain.getBuilding().getOwner();
        // get army name
        var armyName = "os";
        if (player !== null)
        {
            armyName = player.getArmy().toLowerCase();
        }
        if (armyName === "bg")
        {
            armyName = "bh"
        }
        if ((armyName !== "os") &&
            (armyName !== "yc") &&
            (armyName !== "ge") &&
            (armyName !== "bm") &&
            (armyName !== "bh"))
        {
            armyName = "os";
        }
        return "back_hq+" + armyName;
    };

    this.getDescription = function()
    {
        return "<div c='#00ff00'>"+ qsTr("Battle ends ") + "</div>" +
               "<r>" + qsTr("when an army's last HQ is captured. ") + "</r>" +
               "<div c='#00ff00'>" + qsTr("Ground ") + "</div>" +
               "<r>" + qsTr("units can ") + "</r>" +
               "<div c='#00ff00'>" + qsTr("resupply.") + "</div>";
    };

    this.getVisionHide = function(building)
    {
        return true;
    };

    this.getRepairTypes = function(building)
    {
        return [GameEnums.UnitType_Ground, GameEnums.UnitType_Hovercraft, GameEnums.UnitType_Infantry];
    };
}

Constructor.prototype = BUILDING;
var HQ = new Constructor();
