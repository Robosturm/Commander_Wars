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
            building.loadSprite("hq+" + armyName + "+mask", true);
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
    this.addCaptureAnimationBuilding = function(animation, building, startColor, capturedColor)
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
        animation.addBuildingSprite("hq+" + armyName + "+mask", startColor , capturedColor, true);
        animation.addBuildingSprite("hq+" + armyName, startColor , capturedColor, false);
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
        return qsTr("<div c='00ff00'>Battle ends </div>when an army's last HQ is captured. <div c='00ff00'>Ground </div>units can <div c='00ff00'>resupply.</div>");
    };
}

Constructor.prototype = BUILDING;
var HQ = new Constructor();
