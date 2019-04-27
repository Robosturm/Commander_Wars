var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(building)
    {
        if (building.getOwnerID() >= 0)
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
        return ["INFANTRY", "MECH", "MOTORBIKE", "RECON", "APC", "FLARE",
                "ARTILLERY", "LIGHT_TANK", "FLAK", "ANTITANKCANNON",
                "HEAVY_TANK", "MISSILE", "ROCKETTHROWER", "MEGATANK"];
    };

    this.getName = function()
    {
        return qsTr("Town");
    };
}

Constructor.prototype = BUILDING;
var TOWN = new Constructor();
