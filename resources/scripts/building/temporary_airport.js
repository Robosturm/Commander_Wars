var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(building, neutral)
    {
        if (building.getOwnerID() >= 0 && !neutral)
        {
            // none neutral player
            building.loadSprite("temporary_airport", false);
            building.loadSprite("temporary_airport+mask", true);
        }
        else
        {
            // neutral player
            building.loadSprite("temporary_airport+neutral", false);
        }
    };

    this.addCaptureAnimationBuilding = function(animation, building, startColor, capturedColor)
    {
        animation.addBuildingSprite("airport+mask", startColor , capturedColor, true);
        animation.addBuildingSprite("airport", startColor , capturedColor, false);
    };
    this.getName = function()
    {
        return qsTr("Temporary Airport");
    };
    this.getActions = function()
    {
        // returns a string id list of the actions this building can perform
        return "";
    };
    this.getBaseIncome = function()
    {
        return 0;
    };

    this.getConstructionList = function(building)
    {
        return ["T_HELI", "K_HELI", "DUSTER", "TRANSPORTPLANE", "BOMBER", "FIGHTER", "STEALTHBOMBER", "BLACK_BOMB"];
    };

    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "fore_airport";
    };

    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_airport";
    };
    this.getDefense = function()
    {
        return 1;
    };

    this.getDescription = function()
    {
        return qsTr("Airport made by APC. <div c='00ff00'>Resupplying <div c='ffffff'>of <div c='00ff00'>air <div c='ffffff'>units is possible.");
    };
}

Constructor.prototype = BUILDING;
var TEMPORARY_AIRPORT = new Constructor();
