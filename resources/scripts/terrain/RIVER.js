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
        terrain.loadBaseSprite("river+0+land+N+E+S+W");
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
    this.getFlowTiles = function()
    {
        return ["RIVER", "BRIDGE"];
    };
    this.updateFlowSprites = function(terrain, pPfs)
    {
        var flowData = pPfs.getFlowData();
        var length = flowData.size()
        for (var i  = 0; i < length; ++i)
        {
            var pos = flowData.getPosition(i);
            var flow = flowData.getFlowString(i);
            var currentTerrain = map.getTerrain(pos.x, pos.y);
            if (currentTerrain.getID() === "RIVER")
            {
                currentTerrain.unloadSprites();
                currentTerrain.loadBaseTerrainSprites();
                RIVER.loadSpriteFromFlowData(currentTerrain, pos, flow);
            }
        }
    };
    this.loadSpriteFromFlowData = function(terrain, pos, flow)
    {
        var surroundingsDirect = terrain.getSurroundings("RIVER,BRIDGE,SEA", false, true, GameEnums.Directions_Direct);
        if (surroundingsDirect === "+N+E+S+W" ||
            surroundingsDirect === "+E+S+W" ||
            surroundingsDirect === "+N+S+W" ||
            surroundingsDirect === "+N+E+W" ||
            surroundingsDirect === "+N+E+S")
        {
            var random = globals.randInt(0, 1);
            terrain.loadBaseSprite("river+" + random.toString() + "+land" + surroundingsDirect);
        }
        else
        {
            var surroundingsDiagonal = terrain.getSurroundings("RIVER,BRIDGE,SEA", false, true, GameEnums.Directions_Diagnonal);
            if (surroundingsDirect.includes("+N"))
            {
                surroundingsDiagonal = surroundingsDiagonal.replace("+NE", "").replace("+NW", "");
            }
            if (surroundingsDirect.includes("+S"))
            {
                surroundingsDiagonal = surroundingsDiagonal.replace("+SE", "").replace("+SW", "");
            }
            if (surroundingsDirect.includes("+E"))
            {
                surroundingsDiagonal = surroundingsDiagonal.replace("+SE", "").replace("+NE", "");
            }
            if (surroundingsDirect.includes("+W"))
            {
                surroundingsDiagonal = surroundingsDiagonal.replace("+SW", "").replace("+NW", "");
            }
            var animName = "river" + flow + "+land" + surroundingsDirect + surroundingsDiagonal;
            if (terrain.existsResAnim(animName))
            {
                terrain.loadBaseSprite(animName);
            }
        }
    };
};
Constructor.prototype = TERRAIN;
var RIVER = new Constructor();
