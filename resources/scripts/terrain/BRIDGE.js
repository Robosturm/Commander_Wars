var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(BRIDGE.getName());
    };

    this.getName = function()
    {
        return qsTr("Bridge");
    };
	this.loadBaseTerrain = function(terrain)
    {
		terrain.loadBaseTerrain("RIVER");
    };
    this.loadBaseSprite = function(terrain, currentTerrainID)
    {
        var surroundings = terrain.getSurroundings("RIVER,SEA,ROUGH_SEA,REAF,BEACH,FOG", false, false, GameEnums.Directions_Direct, false);
        terrain.loadBaseSprite("bridge" + surroundings);
    };
    this.useTerrainAsBaseTerrain = function()
    {
        return true;
    };
    this.canBePlaced = function(x, y)
    {
        var terrain = map.getTerrain(x, y);
        if ((terrain.getTerrainID() === "SEA") ||
            (terrain.getTerrainID() === "RIVER") ||
            (terrain.getTerrainID() === "BRIDGE"))
        {
            return true;
        }
        else
        {
            return false;
        }
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_bridge";
    };

    this.getTerrainAnimationForeground = function(unit, terrain, defender)
    {
        return TERRAIN.getFactoryForeground(terrain);
    };

    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        var id = TERRAIN.getTerrainAnimationId(terrain);
        var weatherModifier = TERRAIN.getWeatherModifier();
        switch (id)
        {
            case "SEA":
            case "BEACH":
            case "FOG":
            case "REAF":
            case "ROUGH_SEA":
            {
                return "back_" + weatherModifier + "bridge+sea"
            }
            default:
            {
                var rand = globals.randInt(0, 1);
                return "back_" + weatherModifier + "bridge+" + rand.toString();
            }
        }
    };
    this.getDescription = function()
    {
        return qsTr("Bridge over river and seas which allows ground units to cross. Bridges over sea also allows naval units to cross the field.");
    };
    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["bridge",
                "bridge+E",
                "bridge+E+S",
                "bridge+E+S+W",
                "bridge+E+W",
                "bridge+N",
                "bridge+N+E",
                "bridge+N+E+S",
                "bridge+N+E+S+W",
                "bridge+N+E+W",
                "bridge+N+S",
                "bridge+N+S+W",
                "bridge+N+W",
                "bridge+S",
                "bridge+S+W",
                "bridge+W"];
    };
};
Constructor.prototype = TERRAIN;
var BRIDGE = new Constructor();
