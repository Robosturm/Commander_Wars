var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 3;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
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
		terrain.loadBaseSprite("snow");
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
            terrain.loadOverlaySprite("plains+N");
        }
        if (surroundingsPlains.includes("+E"))
        {
            terrain.loadOverlaySprite("plains+E");
        }
        if (surroundingsPlains.includes("+S"))
        {
            terrain.loadOverlaySprite("plains+S");
        }
        if (surroundingsPlains.includes("+W"))
        {
            terrain.loadOverlaySprite("plains+W");
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
        var variables = terrain.getVariables();
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
