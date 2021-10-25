var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(RIVER.getName());
        terrain.setHasFlowDirection(true);
    };
    this.getName = function()
    {
        return qsTr("River");
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        if (currentTerrainID === "SNOW")
        {
            terrain.loadBaseTerrain("SNOW");
        }
        else if (currentTerrainID === "DESERT")
        {
            terrain.loadBaseTerrain("DESERT");
        }
        else if (currentTerrainID === "WASTE")
        {
            terrain.loadBaseTerrain("WASTE");
        }
        else
        {
            terrain.loadBaseTerrain("PLAINS");
        }
    };
    this.loadBaseSprite = function(terrain)
    {
        var surroundings = terrain.getSurroundings("RIVER,BRIDGE,SEA", false, false, GameEnums.Directions_Direct);
        var surroundingsSea = terrain.getSurroundings("SEA", true, false, GameEnums.Directions_All);
        //
        if (!(surroundingsSea.includes("+N+NE") && surroundingsSea.includes("+NW")))
        {
            var surroundingsN = terrain.getSurroundings("SEA", true, false, GameEnums.Directions_North);
            if (surroundingsN === "+N")
            {
                surroundings = surroundings.replace("+N", "");
            }
        }

        if (!(surroundingsSea.includes("SE+S+SW")))
        {
            var surroundingsS = terrain.getSurroundings("SEA", true, false, GameEnums.Directions_South);
            if (surroundingsS === "+S")
            {
                surroundings = surroundings.replace("+S", "");
            }
        }

        if ((surroundingsSea.includes("+E") ))
        {
            if (!(surroundingsSea.includes("NE+E+SE")))
            {
                surroundings = surroundings.replace("+E", "");
            }
        }
        if ((surroundingsSea.includes("+W") && !(surroundingsSea.includes("SW+W+NW"))))
        {
            surroundings = surroundings.replace("+W", "");
        }
        terrain.loadBaseSprite("river" + surroundings);
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_river";
    };
    this.getDescription = function()
    {
        return qsTr("Most infantry and Hovercraft units are the only ground unit that can cross rivers.");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return [];
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        var rand = globals.randInt(0, 1);        
        var weatherModifier = TERRAIN.getWeatherModifier();
        var baseId = terrain.getBaseTerrainID();        
        return "back_" + weatherModifier + "river+" + rand.toString();
    };
    this.updateFlowSprites = function(terrain, pPfs)
    {
        var flowData = pPfs.getFlowData();
        GameConsole.print(flowData.size(), 1);
        GameConsole.print(flowData.getCosts(0), 1);
    };
};
Constructor.prototype = TERRAIN;
var RIVER = new Constructor();
