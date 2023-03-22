var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    this.getDefaultPalette = function()
    {
        return "palette_clear";
    };
    this.init = function (terrain)
    {
        if (terrain.getPalette() === "")
        {
            terrain.setPalette(PLAINS.getDefaultPalette());
        }
        terrain.setTerrainName(PLAINS.getName());
    };
    this.getName = function()
    {
        return qsTr("Plains");
    };
    this.getDefense = function()
    {
        return 1;
    };
    this.loadBaseSprite = function(terrain, currentTerrainID, map)
    {
        var random = globals.randInt(0, 33);
        if (random < 12)
        {
            terrain.loadBaseSprite("plains+" + random.toString());
        }
        else
        {
            terrain.loadBaseSprite("plains+" + (random - 12).toString());
        }
    };
    this.loadOverlaySprite = function(terrain, map)
    {
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
            terrain.loadOverlaySprite("shadow_plains");
        }
    };

    this.getMiniMapIcon = function()
    {
        return "minimap_plains";
    };
    this.getDescription = function()
    {
        return qsTr("Ground units move easily on this terrain.");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["plains+0",
                "plains+1",
                "plains+2",
                "plains+3",
                "plains+4",
                "plains+5",
                "plains+6",
                "plains+7",
                "plains+8",
                "plains+9",
                "plains+10",
                "plains+11",
                "plains+12",
                "plains+13",
                "plains+14",
                "plains+15",
                "plains+16",
                "plains+17",
                "plains+18",
                "plains+19",
                "plains+20",
                "plains+21"];
    };

    this.getOverlayTerrainSprites = function(map)
    {
        return ["shadow_plains"]
    };
};
Constructor.prototype = TERRAIN;
var PLAINS = new Constructor();
