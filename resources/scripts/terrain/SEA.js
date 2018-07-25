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
        var surroundings = terrain.getSurroundings("SEA", true, true, TERRAIN.Direct, false);
        if (surroundings !== "")
        {
            terrain.loadOverlaySprite("sea" + surroundings);
        }
        // load river overlay
        var surroundingsSea = terrain.getSurroundings("SEA", true, false, TERRAIN.All, false);
        var surroundingsRiver = terrain.getSurroundings("RIVER", false, false, TERRAIN.Direct, false);
        // load overlay north
        if (surroundingsRiver.includes("+N") && surroundingsSea.includes("+E") && surroundingsSea.includes("+W"))
        {
            terrain.loadOverlaySprite("riverending+N");
        }
        // load overlay east
        if (surroundingsRiver.includes("+E") && surroundingsSea.includes("+S") && surroundingsSea.includes("+N"))
        {
            terrain.loadOverlaySprite("riverending+E");
        }
        // load overlay south
        if (surroundingsRiver.includes("+S") && surroundingsSea.includes("+E") && surroundingsSea.includes("+W"))
        {
            terrain.loadOverlaySprite("riverending+S");
        }
        // load overlay west
        if (surroundingsRiver.includes("+W") && surroundingsSea.includes("+S") && surroundingsSea.includes("+N"))
        {
            terrain.loadOverlaySprite("riverending+W");
        }
        // load overlay south east
        if (!surroundings.includes("+S") && !surroundings.includes("+E"))
        {
            var surroundingsSE = terrain.getSurroundings("SEA", true, true, TERRAIN.SouthEast, false);
            if (surroundingsSE !== "")
            {
                terrain.loadOverlaySprite("sea" + surroundingsSE);
            }
        }
        // load overlay north east
        if (!surroundings.includes("+N") && !surroundings.includes("+E"))
        {
            var surroundingsNE = terrain.getSurroundings("SEA", true, true, TERRAIN.NorthEast, false);
            if (surroundingsNE !== "")
            {
                terrain.loadOverlaySprite("sea" + surroundingsNE);
            }
        }
        // load overlay south west
        if (!surroundings.includes("+S") && !surroundings.includes("+W"))
        {
            var surroundingsSW = terrain.getSurroundings("SEA", true, true, TERRAIN.SouthWest, false);
            if (surroundingsSW !== "")
            {
                terrain.loadOverlaySprite("sea" + surroundingsSW);
            }
        }
        // load overlay north west
        if (!surroundings.includes("+N") && !surroundings.includes("+W"))
        {
            var surroundingsNW = terrain.getSurroundings("SEA", true, true, TERRAIN.NorthWest, false);
            if (surroundingsNW !== "")
            {
                terrain.loadOverlaySprite("sea" + surroundingsNW);
            }
        }

    };
};
Constructor.prototype = TERRAIN;
var SEA = new Constructor();
