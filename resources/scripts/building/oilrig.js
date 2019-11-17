var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(building, neutral)
    {
        if (building.getOwnerID() >= 0 && !neutral)
        {
            // none neutral player
            building.loadSprite("oilrig", false);
            building.loadSprite("oilrig+mask", true);
        }
        else
        {
            // neutral player
            building.loadSprite("oilrig+neutral", false);
        }
    };
    this.getBaseTerrain = function(building)
    {
        return ["SEA"];
    };
	this.canBuildingBePlaced = function(terrain, building)
    {
		if (terrain.getTerrainID() === "SEA")
		{
			return true;
		}
		return false;
	};
    this.getBaseIncome = function()
    {
        return 3000;
    };
    this.getName = function()
    {
        return qsTr("Oil Rig");
    };

    this.getDescription = function()
    {
        return qsTr("Oil rig. Once captured produces a lot of funds, but can't repair units.");
    };
}

Constructor.prototype = BUILDING;
var OILRIG = new Constructor();
