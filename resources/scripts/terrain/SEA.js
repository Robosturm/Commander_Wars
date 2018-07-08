var Constructor = function()
{
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.terrainName = qsTr("Sea");
    };

    this.loadBaseSprite = function(terrain)
    {
        terrain.loadBaseSprite("sea");
    };
    this.loadOverlaySprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("SEA,RIVER", true, true, TERRAIN.Direct);
        if (surroundings !== "")
        {
            terrain.loadOverlaySprite("sea" + surroundings);
        }
        // load overlay north east
        if (!surroundings.includes("+N") && !surroundings.includes("+E"))
        {
            var surroundingsNE = terrain.getSurroundings("SEA", true, true, TERRAIN.NorthEast);
            if (surroundingsNE !== "")
            {
                terrain.loadOverlaySprite("sea" + surroundingsNE);
            }
        }
        // load overlay south east
        if (!surroundings.includes("+S") && !surroundings.includes("+E"))
        {
            var surroundingsSE = terrain.getSurroundings("SEA", true, true, TERRAIN.SouthEast);
            if (surroundingsSE !== "")
            {
                terrain.loadOverlaySprite("sea" + surroundingsSE);
            }
        }
        // load overlay south west
        if (!surroundings.includes("+S") && !surroundings.includes("+W"))
        {
            var surroundingsSW = terrain.getSurroundings("SEA", true, true, TERRAIN.SouthWest);
            if (surroundingsSW !== "")
            {
                terrain.loadOverlaySprite("sea" + surroundingsSW);
            }
        }
        // load overlay north west
        if (!surroundings.includes("+N") && !surroundings.includes("+W"))
        {
            var surroundingsNW = terrain.getSurroundings("SEA", true, true, TERRAIN.NorthWest);
            if (surroundingsNW !== "")
            {
                terrain.loadOverlaySprite("sea" + surroundingsNW);
            }
        }
		// load river overlay
		surroundings = terrain.getSurroundings("RIVER", true, true, TERRAIN.Direct);
		// load overlay north
        if (surroundings.includes("+N"))
        {
            terrain.loadOverlaySprite("riverending+N");
        }
        // load overlay east
        if (surroundings.includes("+E"))
        {
            terrain.loadOverlaySprite("riverending+E");
        }
		// load overlay south
        if (surroundings.includes("+S"))
        {
            terrain.loadOverlaySprite("riverending+S");
        }
		// load overlay west
        if (surroundings.includes("+W"))
        {
            terrain.loadOverlaySprite("riverending+W");
        }
    };
};
Constructor.prototype = TERRAIN;
var SEA = new Constructor();
