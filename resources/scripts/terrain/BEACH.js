var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(qsTr("Beach"));
    };
	this.loadBaseTerrain = function(terrain)
    {
		terrain.loadBaseTerrain("SEA");
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundingsLand = terrain.getSurroundings("SEA", true, true, GameEnums.Directions_Direct);
        var surroundingsBeach = terrain.getSurroundings("BEACH", false, false, GameEnums.Directions_Direct, false);
        if (surroundingsLand !== "")
        {
            if (surroundingsLand === "+S" && surroundingsBeach.includes("+N"))
            {
                surroundingsBeach = surroundingsBeach.replace("+N", "");
            }
            if (surroundingsLand === "+N" && surroundingsBeach.includes("+S"))
            {
                surroundingsBeach = surroundingsBeach.replace("+S", "");
            }
            if (surroundingsLand ==="+E" && surroundingsBeach.includes("+W"))
            {
                surroundingsBeach = surroundingsBeach.replace("+W", "");
            }
            if (surroundingsLand ==="+W" && surroundingsBeach.includes("+E"))
            {
                surroundingsBeach = surroundingsBeach.replace("+E", "");
            }
            terrain.loadBaseSprite("beach" + surroundingsBeach + "+land" + surroundingsLand);
        }
        else
        {
            terrain.loadBaseSprite("beach+land+N");
        }
    };
    this.canBePlaced = function(x, y)
    {
        var terrain = map.getTerrain(x, y);
        var surroundingsLand = terrain.getSurroundings("SEA", true, true, GameEnums.Directions_Direct);
        var surroundingsSea = terrain.getSurroundings("SEA", false, true, GameEnums.Directions_All);

        var c = (surroundingsLand === "");
        var d = (surroundingsLand.includes("+S") && surroundingsLand.includes("+E") && surroundingsSea.includes("+NW"));
        var e = (surroundingsLand.includes("+S") && surroundingsLand.includes("+W") && surroundingsSea.includes("+NE"));
        var f = (surroundingsLand.includes("+W") && surroundingsLand.includes("+S") && surroundingsSea.includes("+NE"));
        var g = (surroundingsLand.includes("+W") && surroundingsLand.includes("+N") && surroundingsSea.includes("+SE"));
        var h = (surroundingsLand.includes("+N") && surroundingsLand.includes("+W") && surroundingsSea.includes("+SE"));
        var i = (surroundingsLand.includes("+N") && surroundingsLand.includes("+E") && surroundingsSea.includes("+SW"));
        var j = (surroundingsLand.includes("+E") && surroundingsLand.includes("+S") && surroundingsSea.includes("+NW"));
        var k = (surroundingsLand.includes("+E") && surroundingsLand.includes("+N") && surroundingsSea.includes("+SW"));
        if (c || d || e || f || g || h || i || j || k)
        {
            return false;
        }
            else
        {
            return true;
        }
    }

};
Constructor.prototype = TERRAIN;
var BEACH = new Constructor();
