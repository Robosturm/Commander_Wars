var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 1;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setVisionHigh(1);
        terrain.setTerrainName(RUIN.getName());
    };
    this.getName = function()
    {
        return qsTr("Ruin");
    };
    this.getDefense = function()
    {
        return 1;
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
        else
        {
            terrain.loadBaseTerrain("PLAINS");
        }
    };
    this.getVisionHide = function()
    {
        return true;
    };
    this.loadBaseSprite = function(terrain)
    {
        var random = globals.randInt(0, 2);
        terrain.loadBaseSprite("ruin+" + random.toString());
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_ruin";
    };
    this.getDescription = function()
    {
        return qsTr("<r>In Fog of War conditions, the ruins provide ground unit </r><div c='#00ff00'>hiding places.</div>");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["ruin+0",
                "ruin+1",
                "ruin+2"];
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        var weatherModifier = "";
        var baseId = terrain.getBaseTerrainID();
        if (baseId === "DESERT")
        {
            weatherModifier = "desert";
        }
        else
        {
            weatherModifier = TERRAIN.getWeatherModifier();
        }

        return "back_" + weatherModifier + "ruin";
    };
};
Constructor.prototype = TERRAIN;
var RUIN = new Constructor();
