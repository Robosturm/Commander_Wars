var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 0;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(SEA.getName());
    };

    this.getName = function()
    {
        return qsTr("Sea");
    };
    this.loadBaseSprite = function(terrain, currentTerrainID)
    {
        terrain.loadBaseSprite("sea");
    };
    this.loadOverlaySprite = function(terrain)
    {
        var surroundingsPlains = terrain.getSurroundings("PLAINS,SNOW,WASTE,DESERT", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsPlains !== "")
        {
            terrain.loadOverlaySprite("sea" + surroundingsPlains);
        }
        var surroundingsSnow = terrain.getSurroundings("SNOW", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsSnow !== "")
        {
            terrain.loadOverlaySprite("sea+snow" + surroundingsSnow);
        }
        var surroundingsWaste = terrain.getSurroundings("WASTE", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsWaste !== "")
        {
            terrain.loadOverlaySprite("sea+waste" + surroundingsWaste);
        }
        var surroundingsDesert = terrain.getSurroundings("DESERT", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsDesert !== "")
        {
            terrain.loadOverlaySprite("sea+desert" + surroundingsDesert);
        }

        var surroundings = terrain.getSurroundings("SEA", true, true, GameEnums.Directions_Direct, false);
        // if (surroundings !== "")
        // {
        //     terrain.loadOverlaySprite("sea" + surroundings);
        // }

        // load overlay south east
        if (!surroundings.includes("+S") && !surroundings.includes("+E"))
        {
            var surroundingsSE = terrain.getSurroundings("SEA", true, true, GameEnums.Directions_SouthEast, false);
            if (surroundingsSE !== "")
            {
                terrain.loadOverlaySprite("sea" + surroundingsSE);
            }
        }
        // load overlay north east
        if (!surroundings.includes("+N") && !surroundings.includes("+E"))
        {
            var surroundingsNE = terrain.getSurroundings("SEA", true, true, GameEnums.Directions_NorthEast, false);
            if (surroundingsNE !== "")
            {
                terrain.loadOverlaySprite("sea" + surroundingsNE);
            }
        }
        // load overlay south west
        if (!surroundings.includes("+S") && !surroundings.includes("+W"))
        {
            var surroundingsSW = terrain.getSurroundings("SEA", true, true, GameEnums.Directions_SouthWest, false);
            if (surroundingsSW !== "")
            {
                terrain.loadOverlaySprite("sea" + surroundingsSW);
            }
        }
        // load overlay north west
        if (!surroundings.includes("+N") && !surroundings.includes("+W"))
        {
            var surroundingsNW = terrain.getSurroundings("SEA", true, true, GameEnums.Directions_NorthWest, false);
            if (surroundingsNW !== "")
            {
                terrain.loadOverlaySprite("sea" + surroundingsNW);
            }
        }

        // load river overlay
        var surroundingsSea = terrain.getSurroundings("SEA", true, false, GameEnums.Directions_All, false);
        var surroundingsRiver = terrain.getSurroundings("RIVER", false, false, GameEnums.Directions_Direct, false);
        var surroundingsRiver2 = terrain.getSurroundings("RIVER", true, false, GameEnums.Directions_Direct, false, false, 1);
        // load overlay north
        if ((surroundingsRiver.includes("+N") || surroundingsRiver2.includes("+N")) &&
             surroundingsSea.includes("+E") && surroundingsSea.includes("+W"))
        {
            terrain.loadOverlaySprite("riverending+N");
        }
        // load overlay east
        if ((surroundingsRiver.includes("+E") || surroundingsRiver2.includes("+E")) &&
             surroundingsSea.includes("+S") && surroundingsSea.includes("+N"))
        {
            terrain.loadOverlaySprite("riverending+E");
        }
        // load overlay south
        if ((surroundingsRiver.includes("+S") || surroundingsRiver2.includes("+S")) &&
             surroundingsSea.includes("+E") && surroundingsSea.includes("+W"))
        {
            terrain.loadOverlaySprite("riverending+S");
        }
        // load overlay west
        if ((surroundingsRiver.includes("+W") || surroundingsRiver2.includes("+W")) &&
             surroundingsSea.includes("+S") && surroundingsSea.includes("+N"))
        {
            terrain.loadOverlaySprite("riverending+W");
        }
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_sea";
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
        return qsTr("Calm sea. Naval and air forces have good mobility.");
    };
};
Constructor.prototype = TERRAIN;
var SEA = new Constructor();
