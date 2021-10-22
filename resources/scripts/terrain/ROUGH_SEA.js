var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 0;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setVisionHigh(1);
        terrain.setTerrainName(ROUGH_SEA.getName());
    };
    this.getName = function()
    {
        return qsTr("Rough Sea");
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
		terrain.loadBaseTerrain("SEA");
    };
    this.loadBaseSprite = function(terrain)
    {
        terrain.loadBaseSprite("rough_sea+N+E+S+W");
    };
    this.getDefense = function()
    {
        return 2;
    };
    this.canBePlaced = function(x, y)
    {
        var terrain = map.getTerrain(x, y);
        if (terrain.getBaseTerrainID() === "SEA")
        {
            // it's a sea nice
            var surroundings = terrain.getSurroundings("SEA", true, false, GameEnums.Directions_Direct);
            // we need sea all around us :)
            if (surroundings === "+N+E+S+W")
            {
                return true;
            }
        }
        return false;
    };
    this.loadOverlaySprite = function(terrain)
    {
        // load coast overlay
        var surroundings = terrain.getSurroundings("SEA", true, true, GameEnums.Directions_Direct, false);
        // load overlay south east
        if (!surroundings.includes("+S") && !surroundings.includes("+E"))
        {
            var surroundingsSE = terrain.getSurroundings("SEA", true, true, GameEnums.Directions_SouthEast, false);
            if (surroundingsSE !== "")
            {
                terrain.loadOverlaySprite("sea+overlay" + surroundingsSE);
            }
        }
        // load overlay north east
        if (!surroundings.includes("+N") && !surroundings.includes("+E"))
        {
            var surroundingsNE = terrain.getSurroundings("SEA", true, true, GameEnums.Directions_NorthEast, false);
            if (surroundingsNE !== "")
            {
                terrain.loadOverlaySprite("sea+overlay" + surroundingsNE);
            }
        }
        // load overlay south west
        if (!surroundings.includes("+S") && !surroundings.includes("+W"))
        {
            var surroundingsSW = terrain.getSurroundings("SEA", true, true, GameEnums.Directions_SouthWest, false);
            if (surroundingsSW !== "")
            {
                terrain.loadOverlaySprite("sea+overlay" + surroundingsSW);
            }
        }
        // load overlay north west
        if (!surroundings.includes("+N") && !surroundings.includes("+W"))
        {
            var surroundingsNW = terrain.getSurroundings("SEA", true, true, GameEnums.Directions_NorthWest, false);
            if (surroundingsNW !== "")
            {
                terrain.loadOverlaySprite("sea+overlay" + surroundingsNW);
            }
        }
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_rough_sea";
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        var weatherModifier = TERRAIN.getWeatherModifier();
        return "back_" + weatherModifier +"sea";
    };
    this.getTerrainAnimationMoveSpeed = function()
    {
        return 1;
    };
    this.getDescription = function()
    {
        return qsTr("Movement of naval units is impeded but air units are not affected.");
    };
};
Constructor.prototype = TERRAIN;
var ROUGH_SEA = new Constructor();
