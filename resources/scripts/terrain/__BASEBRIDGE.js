var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setTerrainName(BRIDGE.getName(terrain));
    };

    this.canBePlaced = function(x, y)
    {
        var terrain = map.getTerrain(x, y);
        if ((terrain.getTerrainID() === "SEA") ||
            (terrain.getTerrainID() === "RIVER") ||
            (terrain.getTerrainID() === "BRIDGE") ||
            (terrain.getTerrainID() === "BRIDGE1"))
        {
            return true;
        }
        else
        {
            return false;
        }
    };
    this.getName = function()
    {
        return qsTr("Bridge");
    };
	this.loadBaseTerrain = function(terrain)
    {
		terrain.loadBaseTerrain("RIVER");
    };
    this.loadSprite = function(terrain, style)
    {
        var surroundings = terrain.getSurroundings("RIVER,SEA,ROUGH_SEA,REAF,BEACH,FOG", false, false, GameEnums.Directions_Direct, false);
        terrain.loadBaseSprite(style + surroundings);
    };
    this.useTerrainAsBaseTerrain = function()
    {
        return true;
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
                var variables = terrain.getVariables();
                var variable = variables.getVariable("BACKGROUND_ID");
                var rand = 0;
                if (variable === null)
                {
                    rand = globals.randInt(0, 1);
                    variable = variables.createVariable("BACKGROUND_ID");
                    variable.writeDataInt32(rand);
                }
                else
                {
                    rand = variable.readDataInt32();
                }
                return "back_" + weatherModifier + "bridge+" + rand.toString();
            }
        }
    };
    this.getDescription = function()
    {
        return qsTr("Bridge over river and seas which allows ground units to cross. Bridges over sea also allows naval units to cross the field.");
    };
};
Constructor.prototype = TERRAIN;
var __BASEBRIDGE = new Constructor();
