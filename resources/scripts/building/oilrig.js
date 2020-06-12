var Constructor = function()
{
    // called for loading the main sprite
    this.loadSprites = function(building, neutral)
    {
        if (building.getOwnerID() >= 0 && !neutral)
        {
            // none neutral player
            building.loadSprite("oilrig", false);
            building.loadSpriteV2("oilrig+mask", GameEnums.Recoloring_Table);
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

    this.getVisionHide = function(building)
    {
        return true;
    };
}

Constructor.prototype = BUILDING;
var OILRIG = new Constructor();
