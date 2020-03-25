var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(building, neutral)
    {
        if (building.getOwnerID() >= 0 && !neutral)
        {
            // none neutral player
            building.loadSprite("town", false);
            building.loadSprite("town+mask", true);
        }
        else
        {
            // neutral player
            building.loadSprite("town+neutral", false);
        }
    };
    this.getConstructionList = function(building)
    {
        return ["INFANTRY", "MECH", "MOTORBIKE", "SNIPER", "RECON", "APC", "FLARE",
                "ARTILLERY", "LIGHT_TANK", "HOVERCRAFT", "FLAK", "ARTILLERYCRAFT", "HOVERFLAK",
                "ANTITANKCANNON", "HEAVY_TANK", "HEAVY_HOVERCRAFT", "NEOTANK",
                "MISSILE", "ROCKETTHROWER", "MEGATANK", "PIPERUNNER"];
    };

    this.getName = function()
    {
        return qsTr("Town");
    };

    this.getDescription = function()
    {
        return "<r>" + qsTr("Populated city. Once captured ") + "</r>" +
               "<div c='#00ff00'>" + qsTr("ground ") + "</div>" +
               "<r>" + qsTr("units can ") + "</r>" +
               "<div c='#00ff00'>" + qsTr("resupply") + "</div>" +
               "<r>" + qsTr(".") + "</r>";
    };

    this.getVisionHide = function(building)
    {
        return true;
    };
}

Constructor.prototype = BUILDING;
var TOWN = new Constructor();
