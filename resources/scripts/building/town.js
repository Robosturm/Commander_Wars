var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(building, neutral)
    {
        if (building.getOwnerID() >= 0 && !neutral)
        {
            // none neutral player
            building.loadSprite("town", false);
            building.loadSpriteV2("town+mask", GameEnums.Recoloring_Table);
        }
        else
        {
            // neutral player
            building.loadSprite("town+neutral", false);
        }
    };
    this.constructionList = ["INFANTRY", "MECH", "MOTORBIKE", "SNIPER", "RECON", "APC", "FLARE",
                            "ARTILLERY", "LIGHT_TANK", "HOVERCRAFT", "FLAK", "ARTILLERYCRAFT", "HOVERFLAK",
                            "ANTITANKCANNON", "HEAVY_TANK", "HEAVY_HOVERCRAFT", "NEOTANK",
                            "MISSILE", "ROCKETTHROWER", "MEGATANK", "PIPERUNNER"];
    this.getConstructionList = function(building)
    {
        return TOWN.constructionList;
    };

    this.getName = function()
    {
        return qsTr("Town");
    };

    this.getDescription = function()
    {
        return qsTr("<r>" + "Populated city. Once captured </r><div c='#00ff00'>ground </div><r>units can </r><div c='#00ff00'>resupply</div><r>.</r>");
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
var TOWN = new Constructor();
