var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 3;
    };
    this.getDefaultPalette = function()
    {
        return "palette_snow";
    };
    this.init = function (terrain)
    {
        if (terrain.getPalette() === "")
        {
            terrain.setPalette(SNOW.getDefaultPalette());
        }
        terrain.setTerrainName(SNOW.getName());
    };
    this.getName = function()
    {
        return qsTr("Snow");
    };
    this.getDefense = function()
    {
        return 1;
    };
    this.loadBaseSprite = function(terrain, map)
    {
        var random = globals.randInt(0, 7);
        terrain.loadBaseSprite("snow+" + random.toString());
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_snow";
    };
    this.loadOverlaySprite = function(terrain, map)
    {
        var surroundingsPlains = terrain.getSurroundings("PLAINS", true, false, GameEnums.Directions_Direct, false);
        if (surroundingsPlains.includes("+N"))
        {
            terrain.loadOverlaySprite("plains+N", -1, -1, terrain.getNeighbourPalette(GameEnums.Directions_North, "PLAINS"));
        }
        if (surroundingsPlains.includes("+E"))
        {
            terrain.loadOverlaySprite("plains+E", -1, -1, terrain.getNeighbourPalette(GameEnums.Directions_East, "PLAINS"));
        }
        if (surroundingsPlains.includes("+S"))
        {
            terrain.loadOverlaySprite("plains+S", -1, -1, terrain.getNeighbourPalette(GameEnums.Directions_South, "PLAINS"));
        }
        if (surroundingsPlains.includes("+W"))
        {
            terrain.loadOverlaySprite("plains+W", -1, -1, terrain.getNeighbourPalette(GameEnums.Directions_West, "PLAINS"));
        }
        var x = terrain.getX();
        var y = terrain.getY();
        var highTerrain = terrain.getSurroundings(TERRAIN.getHighTerrains(), true, false, GameEnums.Directions_West, false);
        if (map.onMap(x - 1, y))
        {
            var building = map.getTerrain(x - 1, y).getBuilding();
            if (building !== null &&
                    building.getBuildingWidth() === 1 &&
                    building.getBuildingHeigth() === 1)
            {
                highTerrain = "+W";
            }
        }

        if (highTerrain !== "")
        {
            terrain.loadOverlaySprite("shadow_snow");
        }
    };
    this.getDescription = function()
    {
        return qsTr("Snowy terrain rough to cross.");
    };
    this.getTerrainAnimationForeground = function(unit, terrain, defender, map)
    {
        var variables = terrain.getAnimationVariables();
        var variable = variables.getVariable("FOREGROUND_ID");
        var rand = 0;
        if (variable === null)
        {
            rand = globals.randInt(0, 3);
            variable = variables.createVariable("FOREGROUND_ID");
            variable.writeDataInt32(rand);
        }
        else
        {
            rand = variable.readDataInt32();
        }
        return "fore_snowplains+" + rand.toString();
    };
    this.getTerrainAnimationBackground = function(unit, terrain, defender, map)
    {
        var id = TERRAIN.getTerrainAnimationId(terrain, map);
        return TERRAIN.getTerrainBackgroundId(id, "snow");
    };
    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["snow+0",
                "snow+1",
                "snow+2",
                "snow+3",
                "snow+4",
                "snow+5",
                "snow+6",
                "snow+7",];
    };
    this.getOverlayTerrainSprites = function(map)
    {
        return ["shadow_snow",
                "plains+N",
                "plains+E",
                "plains+S",
                "plains+W",]
    };
};
Constructor.prototype = TERRAIN;
var SNOW = new Constructor();
