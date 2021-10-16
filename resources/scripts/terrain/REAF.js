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
        terrain.setTerrainName(REAF.getName());
    };
    this.getName = function()
    {
        return qsTr("Reaf");
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
		terrain.loadBaseTerrain("SEA");
    };
    this.loadBaseSprite = function(terrain)
    {
		var random = globals.randInt(0, 3);
        if (random > 0)
        {
            terrain.loadBaseSprite("reaf+" + random.toString() + "+N+E+S+W");
        }
        else
        {
            terrain.loadBaseSprite("reaf+N+E+S+W");
        }
    };
    this.getDefense = function()
    {
        return 2;
    };
    this.getVisionHide = function()
    {
        return true;
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
    this.getMiniMapIcon = function()
    {
        return "minimap_reaf";
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
        return qsTr("<r>This spiky reaf provides ideal </r><div c='#00ff00'>hiding places</div><r> for naval units in Fog of War.</r>");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["reaf+N+E+S+W",
                "reaf+1+N+E+S+W",
                "reaf+2+N+E+S+W",
                "reaf+3+N+E+S+W"];
    };
};
Constructor.prototype = TERRAIN;
var REAF = new Constructor();
