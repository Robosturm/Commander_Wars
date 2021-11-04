var Constructor = function()
{
    this.getTerrainGroup = function()
    {
        return 4;
    };
    // loader for stuff which needs C++ Support
    this.init = function (terrain)
    {
        terrain.setVisionHigh(1);
        terrain.setTerrainName(RUIN.getName());
    };
    this.getName = function()
    {
        return qsTr("Waste Ruin");
    };
    this.getDefense = function()
    {
        return 0;
    };
    this.loadBaseTerrain = function(terrain, currentTerrainID)
    {
        if (currentTerrainID === "PLAINS")
        {
            terrain.loadBaseTerrain("PLAINS");
        }
        else if (currentTerrainID === "DESERT")
        {
            terrain.loadBaseTerrain("DESERT");
        }
        else if (currentTerrainID === "SNOW")
        {
            terrain.loadBaseTerrain("SNOW");
        }
        else
        {
            terrain.loadBaseTerrain("WASTE");
        }
    };
    this.getVisionHide = function()
    {
        return true;
    };
    this.loadBaseSprite = function(terrain)
    {
        var random = globals.randInt(0, 2);
        terrain.loadBaseSprite("waste_ruin+" + random.toString());
    };
    this.getMiniMapIcon = function()
    {
        return "minimap_waste_ruin";
    };
    this.getDescription = function()
    {
        return qsTr("<r>In Fog of War conditions, the ruins provide ground unit </r><div c='#00ff00'>hiding places.</div><r> Snowy terrain rough to cross. Waste terrain with reduced defense but clear view. In Fog of War, unit's gain </r><div c='#00ff00'>vision +1.</div>");
    };

    this.getTerrainSprites = function()
    {
        // array of sprites that can be selected as fix sprites for this terrain
        return ["waste_ruin+0",
                "waste_ruin+1",
                "waste_ruin+2"];
    };
    this.getTerrainAnimationForeground = function(unit, terrain)
    {
        return "";
    };
    this.getTerrainAnimationBackground = function(unit, terrain)
    {
        return "back_wasteruin";
    };
};
Constructor.prototype = TERRAIN;
var WASTE_RUIN = new Constructor();
