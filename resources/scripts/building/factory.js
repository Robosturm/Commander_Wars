var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(building, neutral)
    {
        if (building.getOwnerID() >= 0 && !neutral)
        {
            // none neutral player
            building.loadSprite("factory", false);
            building.loadSprite("factory+mask", true);
        }
        else
        {
            // neutral player
            building.loadSprite("factory+neutral", false);
        }
    };
    this.getName = function()
    {
        return qsTr("Base");
    };
    this.addCaptureAnimationBuilding = function(animation, building, startColor, capturedColor)
    {
        animation.addBuildingSprite("factory+mask", startColor , capturedColor, true);
        animation.addBuildingSprite("factory", startColor , capturedColor, false);
    };

    this.getActions = function()
    {
        // returns a string id list of the actions this building can perform
        return "ACTION_BUILD_UNITS";
    };

    this.getConstructionList = function(building)
    {
        return ["INFANTRY", "MECH", "MOTORBIKE", "SNIPER", "RECON", "APC", "FLARE",
                "ARTILLERY", "LIGHT_TANK", "HOVERCRAFT", "FLAK", "ARTILLERYCRAFT", "HOVERFLAK",
                "ANTITANKCANNON", "HEAVY_TANK", "HEAVY_HOVERCRAFT", "NEOTANK",
                "MISSILE", "ROCKETTHROWER", "MEGATANK", "PIPERUNNER"];
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_factory";
    };

    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_factory";
    };

    this.getDescription = function()
    {
        return "<r>" + qsTr("Once captured can be used for ") + "</r>" +
               "<div c='#00ff00'>" + qsTr("production and resupplying ") + "</div>" +
               "<r>" + qsTr("of ") + "</r>" +
               "<div c='#00ff00'>" + qsTr("ground ") + "</div>" +
               "<r>" + qsTr("units.") + "</r>";
    };
}

Constructor.prototype = BUILDING;
var FACTORY = new Constructor();
