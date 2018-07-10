var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.terrainName = qsTr("Beach");
    };
	this.loadBaseTerrain = function(terrain)
    {
		terrain.loadBaseTerrain("SEA");
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundingsLand = terrain.getSurroundings("SEA", true, true, TERRAIN.Direct);
        var surroundingsBeach = terrain.getSurroundings("BEACH", false, false, TERRAIN.Direct);
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
        var surroundingsBeach = terrain.getSurroundings("BEACH", false, false, TERRAIN.Direct);
        var surroundingsBeachN = terrain.getSurroundings("BEACH", false, false, TERRAIN.North);
        var surroundingsBeachS = terrain.getSurroundings("BEACH", false, false, TERRAIN.South);
        var surroundingsLand = terrain.getSurroundings("SEA", true, true, TERRAIN.All);
        var surroundingsLandN = terrain.getSurroundings("SEA", true, true, TERRAIN.North);
        var surroundingsLandS = terrain.getSurroundings("SEA", true, true, TERRAIN.South);
        var surroundingsSea = terrain.getSurroundings("SEA", false, true, TERRAIN.Direct);

        var c = (surroundingsLand === "");
        var d = (surroundingsLandS.includes("+S") && surroundingsLand.includes("+E") && surroundingsSea.includes("+NW"));
        var e = (surroundingsLandS.includes("+S") && surroundingsLand.includes("+W") && surroundingsSea.includes("+NE"));
        var f = (surroundingsLand.includes("+W") && surroundingsLandS.includes("+S") && surroundingsSea.includes("+NE"));
        var g = (surroundingsLand.includes("+W") && surroundingsLandN.includes("+N") && surroundingsSea.includes("+SE"));
        var h = (surroundingsLandN.includes("+N") && surroundingsLand.includes("+W") && surroundingsSea.includes("+SE"));
        var i = (surroundingsLandN.includes("+N") && surroundingsLand.includes("+E") && surroundingsSea.includes("+SW"));
        var j = (surroundingsLand.includes("+E") && surroundingsLandS.includes("+S") && surroundingsSea.includes("+NW"));
        var k = (surroundingsLand.includes("+E") && surroundingsLandN.includes("+N") && surroundingsSea.includes("+SW"));
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
